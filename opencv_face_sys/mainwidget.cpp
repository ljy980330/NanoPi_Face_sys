#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //去除边框
    //this->setWindowFlags(Qt::FramelessWindowHint);

    this->resize(800,480);

    //open usb-wifi
//    QProcess *proc = new QProcess(this);
    //proc->start("nmcli r wifi on");
//    proc->start("nmcli dev wifi connect \"CMCC - AUTO\" password \"Jn_408408\" ifname wlx000f009b0394");

    //初始化背景图片
    BackGroudImage.load("./image/background1.jpg");

    //支持Qt中文字库
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //使用自定义结构体在信号槽中传递时需要先注册，否则会报错
    qRegisterMetaType<CamerInfo_Data>("CamerInfo_Data");
    qRegisterMetaType<SysInfo_Data>("SysInfo_Data");
    qRegisterMetaType<RTC_Data>("RTC_Data");
    qRegisterMetaType<Weather_Data>("Weather_Data");

    //添加欢迎界面到主界面并显示
    welcome_widget_ui = new WelcomeWidget(this);
    connect(welcome_widget_ui,&WelcomeWidget::welcome_end_sig,this,&MainWidget::camer_widget_setup);

    ui->ui_stackedWidget->addWidget(welcome_widget_ui);
    ui->ui_stackedWidget->setCurrentWidget(welcome_widget_ui);

    //初始化所有界面
    camer_widget_ui = new CamerWidget;
    ui->ui_stackedWidget->addWidget(camer_widget_ui);

    weather_widget_ui = new WeatherWidget;
    ui->ui_stackedWidget->addWidget(weather_widget_ui);

    sysinfo_widget_ui = new SysInfoWidget;
    ui->ui_stackedWidget->addWidget(sysinfo_widget_ui);

    //启动线程
    sys_info_thread = new SysInfo_thread;
    sys_info_thread->start();

    camer_info_thread = new CamerInfo_thread;
    camer_info_thread->start();

    //启动rtc时间定时器
    rtc_timer = new QTimer(this);
    connect(rtc_timer,&QTimer::timeout,this,&MainWidget::rtc_update);
    connect(this,&MainWidget::rtc_time_sig,camer_widget_ui,&CamerWidget::rtc_updata);
    connect(this,&MainWidget::rtc_time_sig,weather_widget_ui,&WeatherWidget::rtc_updata);
    rtc_timer->start(1000);

    //天气相关初始化
    url_weather_now = "https://api.seniverse.com/v3/weather/now.json?key=kyccvwzmf1uh8q0g&location=ip&language=zh-Hans&unit=c";
    url_weather_daily = "https://api.seniverse.com/v3/weather/daily.json?key=kyccvwzmf1uh8q0g&location=ip&language=zh-Hans&unit=c&start=0&days=5";
    url_weather_life = "https://api.seniverse.com/v3/life/suggestion.json?key=kyccvwzmf1uh8q0g&location=ip&language=zh-Hans";

    m_NetManger_now = new QNetworkAccessManager;
    m_NetManger_daily = new QNetworkAccessManager;
    m_NetManger_life = new QNetworkAccessManager;

    connect(m_NetManger_now,&QNetworkAccessManager::finished,this,&MainWidget::finishedSlot_now);
    connect(m_NetManger_daily,&QNetworkAccessManager::finished,this,&MainWidget::finishedSlot_daily);
    connect(m_NetManger_life,&QNetworkAccessManager::finished,this,&MainWidget::finishedSlot_life);

    connect(this,&MainWidget::weather_info_sig,camer_widget_ui,&CamerWidget::get_weather_info);
    connect(this,&MainWidget::weather_info_sig,weather_widget_ui,&WeatherWidget::get_weather_info);

    NetWorkFlag = false;
    weather_timer = new QTimer(this);
    connect(weather_timer,&QTimer::timeout,this,&MainWidget::weather_update);
//    weather_update();
    weather_timer->start(1000);

    //close时可以进入析构函数
    this->setAttribute(Qt::WA_DeleteOnClose);
}

MainWidget::~MainWidget()
{
    //删除界面指针，防止内存泄漏
    if(welcome_widget_ui != NULL)
    {
        ui->ui_stackedWidget->removeWidget(welcome_widget_ui);
        delete welcome_widget_ui;
        welcome_widget_ui = NULL;
    }

    if(camer_widget_ui != NULL)
    {
        ui->ui_stackedWidget->removeWidget(camer_widget_ui);
        delete camer_widget_ui;
        camer_widget_ui = NULL;
    }

    if(sysinfo_widget_ui != NULL)
    {
        ui->ui_stackedWidget->removeWidget(sysinfo_widget_ui);
        delete sysinfo_widget_ui;
        sysinfo_widget_ui = NULL;
    }

    //删除线程指针，防止内存泄漏
    if(sys_info_thread->isRunning())
    {
        sys_info_thread->stop();
        sys_info_thread->wait();
        sys_info_thread->quit();
    }
    delete sys_info_thread;

    if(camer_info_thread->isRunning())
    {
        camer_info_thread->stop();
        camer_info_thread->wait();
        camer_info_thread->quit();
    }
    delete camer_info_thread;

    // weather pointer
    weather_timer->stop();
    if(m_NetManger_now != NULL)
    {
        delete m_NetManger_now;
    }
    if(m_NetManger_daily != NULL)
    {
        delete m_NetManger_daily;
    }
    if(m_NetManger_life != NULL)
    {
        delete m_NetManger_life;
    }

    delete ui;
}

/***********************************************
 *      Slots functions
 **********************************************/
void MainWidget::camer_widget_setup()
{
    connect(camer_widget_ui,&CamerWidget::mousePress_sig,this,&MainWidget::camer_widget_weather_press);
    connect(camer_info_thread,&CamerInfo_thread::camer_info_sig,camer_widget_ui,&CamerWidget::get_camer_info);
    connect(sys_info_thread,&SysInfo_thread::sys_info_sig,sysinfo_widget_ui,&SysInfoWidget::get_sys_info);

    ui->ui_stackedWidget->setCurrentWidget(camer_widget_ui);

    //主界面启动后将欢迎界面删除
    if(welcome_widget_ui != NULL)
    {
        ui->ui_stackedWidget->removeWidget(welcome_widget_ui);
        delete welcome_widget_ui;
        welcome_widget_ui = NULL;
    }
}

/***********************************************
 *      UI change slots functions
 **********************************************/
void MainWidget::camer_widget_weather_press(int flag)
{
    //如果点击了信息框区域，跳转至系统信息详情界面显示
    if(flag == 2)
    {
        disconnect(camer_info_thread,&CamerInfo_thread::camer_info_sig,camer_widget_ui,&CamerWidget::get_camer_info);
        connect(sysinfo_widget_ui,&SysInfoWidget::sysinfo_widget_close_sig,this,&MainWidget::sysinfo_widget_close);
        sysinfo_widget_ui->sysinfo_widget_reset();
        ui->ui_stackedWidget->setCurrentWidget(sysinfo_widget_ui);
    }

    //如果点击了天气框区域，跳转至天气详情界面显示
    if(flag == 3)
    {
        disconnect(camer_info_thread,&CamerInfo_thread::camer_info_sig,camer_widget_ui,&CamerWidget::get_camer_info);
        connect(weather_widget_ui,&WeatherWidget::weather_widget_close_sig,this,&MainWidget::weather_widget_close);

        ui->ui_stackedWidget->setCurrentWidget(weather_widget_ui);
    }
}

//系统信息窗口关闭信号
void MainWidget::sysinfo_widget_close()
{
    //解除系统信息窗口的信号
    disconnect(sysinfo_widget_ui,&SysInfoWidget::sysinfo_widget_close_sig,this,&MainWidget::sysinfo_widget_close);

    connect(camer_info_thread,&CamerInfo_thread::camer_info_sig,camer_widget_ui,&CamerWidget::get_camer_info);

    //切换至默认窗口显示
    ui->ui_stackedWidget->setCurrentWidget(camer_widget_ui);
}

//天气详情窗口关闭信号
void MainWidget::weather_widget_close()
{
    //解除天气详情窗口的信号
    disconnect(weather_widget_ui,&WeatherWidget::weather_widget_close_sig,this,&MainWidget::weather_widget_close);

    connect(camer_info_thread,&CamerInfo_thread::camer_info_sig,camer_widget_ui,&CamerWidget::get_camer_info);

    //切换至默认窗口显示
    ui->ui_stackedWidget->setCurrentWidget(camer_widget_ui);
}

void MainWidget::rtc_update()
{
    date = QDate::currentDate();
    switch (date.dayOfWeek())
    {
        case 1: rtc_data.week = "星期一";break;
        case 2: rtc_data.week = "星期二";break;
        case 3: rtc_data.week = "星期三";break;
        case 4: rtc_data.week = "星期四";break;
        case 5: rtc_data.week = "星期五";break;
        case 6: rtc_data.week = "星期六";break;
        case 7: rtc_data.week = "星期日";break;
    }

    date_time = QDateTime::currentDateTime();
    rtc_data.time_text = date_time.toString(tr("MM/dd hh:mm:ss"));

    emit rtc_time_sig(rtc_data);
}

/***********************************************
 *      Event functions
 **********************************************/
void MainWidget::paintEvent(QPaintEvent *)
{
    //重绘界面
    QPainter paint(this);

    //如果背景不为空则填充背景
    if(!BackGroudImage.isNull())
    {
        paint.drawPixmap(QRect(0,0,this->width(),this->height()),BackGroudImage);
    }
}

void MainWidget::resizeEvent(QResizeEvent *)
{
    this->update();
}

/***********************************************
 *      weather functions
 **********************************************/
void MainWidget::weather_update()
{
    static bool FirstFlagTrue = false;
    if((NetWorkFlag >= 3) && (FirstFlagTrue == false))
    {
        FirstFlagTrue = true;
        weather_timer->start(60000);
    }
    flag = 0;
    m_Reply_now = m_NetManger_now->get(QNetworkRequest(url_weather_now));
    m_Reply_life = m_NetManger_life->get(QNetworkRequest(url_weather_life));
    m_Reply_daily = m_NetManger_daily->get(QNetworkRequest(url_weather_daily));

//    qDebug() << "weather_update  NetWorkFlag " << NetWorkFlag;
}

void MainWidget::finishedSlot_now(QNetworkReply *)
{
    m_Reply_now->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    m_Reply_now->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (m_Reply_now->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_Reply_now->readAll();
        json_analysis_now(bytes);
        NetWorkFlag++;
    }
    else
    {
        qDebug() << m_Reply_now->errorString();
    }

    m_Reply_now->deleteLater();
}

void MainWidget::finishedSlot_life(QNetworkReply *)
{
    m_Reply_life->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    m_Reply_life->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (m_Reply_life->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_Reply_life->readAll();
        json_analysis_life(bytes);
        NetWorkFlag++;
    }
    else
    {
        qDebug() << m_Reply_life->errorString();
    }

    m_Reply_life->deleteLater();
}

void MainWidget::finishedSlot_daily(QNetworkReply *)
{
    m_Reply_daily->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    m_Reply_daily->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (m_Reply_daily->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_Reply_daily->readAll();
        json_analysis_daliy(bytes);
        NetWorkFlag++;
    }
    else
    {
        qDebug() << m_Reply_daily->errorString();
    }

    m_Reply_daily->deleteLater();
}

void MainWidget::json_analysis_now(QByteArray data)
{
    QByteArray block;
    block = data;
    //qDebug() << "接收 : " << block;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(block, &json_error);

    //qDebug() << "parse_doucment : " << parse_doucment;
    //下面是json格式的解析
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            //开始解析json对象
            QJsonObject jsonObject = parse_doucment.object();
            if(jsonObject.contains("results"))
            {
                QJsonValue results_value = jsonObject.take("results");
                if(results_value.isArray()) //判断他是不是json数组
                {
                    QJsonArray results_array = results_value.toArray();
                    QJsonObject results_object = results_array.at(0).toObject();
                    //qDebug() << "results_object : " << results_object;

                    //提取last_update
                    if(results_object.contains("last_update"))
                    {
                        weather_data.weather_now.last_update = results_object.take("last_update").toString();
                        //qDebug() << "last_update : " << last_update;
                    }

                    //提取location
                    if(results_object.contains("location"))
                    {
                        QJsonValue location_value = results_object.take("location");
                        //qDebug() << "location_value : " << location_value;
                        QJsonObject location_object = location_value.toObject();
                        //提取country
                        if(location_object.contains("country"))
                        {
                            weather_data.weather_now.country = location_object.take("country").toString();
                            //qDebug() << "country : " << country;
                        }
                        //提取id
                        if(location_object.contains("id"))
                        {
                            weather_data.weather_now.id = location_object.take("id").toString();
                            //qDebug() << "id : " << id;
                        }
                        //提取name
                        if(location_object.contains("name"))
                        {
                            weather_data.weather_now.name = location_object.take("name").toString();
                            //qDebug() << "name : " << name;
                        }
                        //提取path
                        if(location_object.contains("path"))
                        {
                            weather_data.weather_now.path = location_object.take("path").toString();
                            //qDebug() << "path : " << path;
                        }
                        //提取timezone
                        if(location_object.contains("timezone"))
                        {
                            weather_data.weather_now.timezone = location_object.take("timezone").toString();
                            //qDebug() << "timezone : " << timezone;
                        }
                        //提取timezone_offset
                        if(location_object.contains("timezone_offset"))
                        {
                            weather_data.weather_now.timezone_offset = location_object.take("timezone_offset").toString();
                            //qDebug() << "timezone_offset : " << timezone_offset;
                        }

                    }

                    //提取now
                    if(results_object.contains("now"))
                    {
                        QJsonValue now_value = results_object.take("now");
                        //qDebug() << "now_value : " << now_value;
                        QJsonObject now_object = now_value.toObject();
                        //提取code
                        if(now_object.contains("code"))
                        {
                            weather_data.weather_now.code = now_object.take("code").toString();
                            //qDebug() << "code : " << code;
                        }
                        //提取temperature
                        if(now_object.contains("temperature"))
                        {
                            weather_data.weather_now.temperature = now_object.take("temperature").toString();
                            //qDebug() << "temperature : " << temperature;
                        }
                        //提取text
                        if(now_object.contains("text"))
                        {
                            weather_data.weather_now.text = now_object.take("text").toString();
                            //qDebug() << "text : " << text;
                        }

                    }
                }
            }
        }
    }

    flag++;
    if(flag == 3)
    {
       flag = 0;
       emit weather_info_sig(weather_data);
    }
}

void MainWidget::json_analysis_daliy(QByteArray data)
{
    QByteArray block;
    block = data;
    //qDebug() << "接收 : " << block;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(block, &json_error);

    //qDebug() << "parse_doucment : " << parse_doucment;

    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            //开始解析json对象
            QJsonObject jsonObject = parse_doucment.object();
            if(jsonObject.contains("results"))
            {
                QJsonValue results_value = jsonObject.take("results");
                if(results_value.isArray()) //判断他是不是json数组
                {
                    QJsonArray results_array = results_value.toArray();
                    QJsonObject results_object = results_array.at(0).toObject();
                    //qDebug() << "results_object : " << results_object;

                    //提取daily
                    if(results_object.contains("daily"))
                    {
                        QJsonValue daily_value = results_object.take("daily");
                        if(daily_value.isArray()) //判断是不是json数组
                        {
                            //分别提取今天，明天，后天的天气预报
                            QJsonArray daily_array = daily_value.toArray();
                            QJsonObject today_object = daily_array.at(0).toObject();
                            QJsonObject tomorrow_object = daily_array.at(1).toObject();
                            QJsonObject the_day_after_tomorrow_object = daily_array.at(2).toObject();

                            //提取今天的date
                            if(today_object.contains("date"))
                            {
                                weather_data.weather_daily_1.date = today_object.take("date").toString();
                                //qDebug() << "today date : " << weather_daily_1.date;
                            }
                            //提取今天的code_day
                            if(today_object.contains("code_day"))
                            {
                                weather_data.weather_daily_1.code_day = today_object.take("code_day").toString();
                                //qDebug() << "today code_day : " << weather_daily_1.code_day;
                            }
                            //提取今天的code_night
                            if(today_object.contains("code_night"))
                            {
                                weather_data.weather_daily_1.code_night = today_object.take("code_night").toString();
                                //qDebug() << "today code_night : " << weather_daily_1.code_night;
                            }
                            //提取今天的high
                            if(today_object.contains("high"))
                            {
                                weather_data.weather_daily_1.high = today_object.take("high").toString();
                                //qDebug() << "today high : " << weather_daily_1.high;
                            }
                            //提取今天的low
                            if(today_object.contains("low"))
                            {
                                weather_data.weather_daily_1.low = today_object.take("low").toString();
                                //qDebug() << "today low : " << weather_daily_1.low;
                            }
                            //提取今天的precip
                            if(today_object.contains("precip"))
                            {
                                weather_data.weather_daily_1.precip = today_object.take("precip").toString();
                                //qDebug() << "today precip : " << weather_daily_1.precip;
                            }
                            //提取今天的text_day
                            if(today_object.contains("text_day"))
                            {
                                weather_data.weather_daily_1.text_day = today_object.take("text_day").toString();
                                //qDebug() << "today text_day : " << weather_daily_1.text_day;
                            }
                            //提取今天的text_night
                            if(today_object.contains("text_night"))
                            {
                                weather_data.weather_daily_1.text_night = today_object.take("text_night").toString();
                                //qDebug() << "today text_night : " << weather_daily_1.text_night;
                            }
                            //提取今天的wind_direction
                            if(today_object.contains("wind_direction"))
                            {
                                weather_data.weather_daily_1.wind_direction = today_object.take("wind_direction").toString();
                                //qDebug() << "today wind_direction : " << weather_daily_1.wind_direction;
                            }
                            //提取今天的wind_direction_degree
                            if(today_object.contains("wind_direction_degree"))
                            {
                                weather_data.weather_daily_1.wind_direction_degree = today_object.take("wind_direction_degree").toString();
                                //qDebug() << "today wind_direction_degree : " << weather_daily_1.wind_direction_degree;
                            }
                            //提取今天的wind_scale
                            if(today_object.contains("wind_scale"))
                            {
                                weather_data.weather_daily_1.wind_scale = today_object.take("wind_scale").toString();
                                //qDebug() << "today wind_scale : " << weather_daily_1.wind_scale;
                            }
                            //提取今天的wind_speed
                            if(today_object.contains("wind_speed"))
                            {
                                weather_data.weather_daily_1.wind_speed = today_object.take("wind_speed").toString();
                                //qDebug() << "today wind_speed : " << weather_daily_1.wind_speed;
                            }

                            //提取明天的date
                            if(tomorrow_object.contains("date"))
                            {
                                weather_data.weather_daily_2.date = tomorrow_object.take("date").toString();
                                //qDebug() << "tomorrow date : " << weather_data.weather_daily_2.date;
                            }
                            //提取明天的code_day
                            if(tomorrow_object.contains("code_day"))
                            {
                                weather_data.weather_daily_2.code_day = tomorrow_object.take("code_day").toString();
                                //qDebug() << "tomorrow code_day : " << weather_data.weather_daily_2.code_day;
                            }
                            //提取明天的code_night
                            if(tomorrow_object.contains("code_night"))
                            {
                                weather_data.weather_daily_2.code_night = tomorrow_object.take("code_night").toString();
                                //qDebug() << "tomorrow code_night : " << weather_data.weather_daily_2.code_night;
                            }
                            //提取明天的high
                            if(tomorrow_object.contains("high"))
                            {
                                weather_data.weather_daily_2.high = tomorrow_object.take("high").toString();
                                //qDebug() << "tomorrow high : " << weather_data.weather_daily_2.high;
                            }
                            //提取明天的low
                            if(tomorrow_object.contains("low"))
                            {
                                weather_data.weather_daily_2.low = tomorrow_object.take("low").toString();
                                //qDebug() << "tomorrow low : " << weather_data.weather_daily_2.low;
                            }
                            //提取明天的precip
                            if(tomorrow_object.contains("precip"))
                            {
                                weather_data.weather_daily_2.precip = tomorrow_object.take("precip").toString();
                                //qDebug() << "tomorrow precip : " << weather_data.weather_daily_2.precip;
                            }
                            //提取明天的text_day
                            if(tomorrow_object.contains("text_day"))
                            {
                                weather_data.weather_daily_2.text_day = tomorrow_object.take("text_day").toString();
                                //qDebug() << "tomorrow text_day : " << weather_data.weather_daily_2.text_day;
                            }
                            //提取明天的text_night
                            if(tomorrow_object.contains("text_night"))
                            {
                                weather_data.weather_daily_2.text_night = tomorrow_object.take("text_night").toString();
                                //qDebug() << "tomorrow text_night : " << weather_data.weather_daily_2.text_night;
                            }
                            //提取明天的wind_direction
                            if(tomorrow_object.contains("wind_direction"))
                            {
                                weather_data.weather_daily_2.wind_direction = tomorrow_object.take("wind_direction").toString();
                                //qDebug() << "tomorrow wind_direction : " << weather_data.weather_daily_2.wind_direction;
                            }
                            //提取明天的wind_direction_degree
                            if(tomorrow_object.contains("wind_direction_degree"))
                            {
                                weather_data.weather_daily_2.wind_direction_degree = tomorrow_object.take("wind_direction_degree").toString();
                                //qDebug() << "tomorrow wind_direction_degree : " << weather_data.weather_daily_2.wind_direction_degree;
                            }
                            //提取明天的wind_scale
                            if(tomorrow_object.contains("wind_scale"))
                            {
                                weather_data.weather_daily_2.wind_scale = tomorrow_object.take("wind_scale").toString();
                                //qDebug() << "tomorrow wind_scale : " << weather_data.weather_daily_2.wind_scale;
                            }
                            //提取明天的wind_speed
                            if(tomorrow_object.contains("wind_speed"))
                            {
                                weather_data.weather_daily_2.wind_speed = tomorrow_object.take("wind_speed").toString();
                                //qDebug() << "tomorrow wind_speed : " << weather_data.weather_daily_2.wind_speed;
                            }

                            //提取后天的date
                            if(the_day_after_tomorrow_object.contains("date"))
                            {
                                weather_data.weather_daily_3.date = the_day_after_tomorrow_object.take("date").toString();
                                //qDebug() << "the_day_after_tomorrow date : " << weather_data.weather_daily_3.date;
                            }
                            //提取后天的code_day
                            if(the_day_after_tomorrow_object.contains("code_day"))
                            {
                                weather_data.weather_daily_3.code_day = the_day_after_tomorrow_object.take("code_day").toString();
                                //qDebug() << "the_day_after_tomorrow code_day : " << weather_data.weather_daily_3.code_day;
                            }
                            //提取后天的code_night
                            if(the_day_after_tomorrow_object.contains("code_night"))
                            {
                                weather_data.weather_daily_3.code_night = the_day_after_tomorrow_object.take("code_night").toString();
                                //qDebug() << "the_day_after_tomorrow code_night : " << weather_data.weather_daily_3.code_night;
                            }
                            //提取后天的high
                            if(the_day_after_tomorrow_object.contains("high"))
                            {
                                weather_data.weather_daily_3.high = the_day_after_tomorrow_object.take("high").toString();
                                //qDebug() << "the_day_after_tomorrow high : " << weather_data.weather_daily_3.high;
                            }
                            //提取后天的low
                            if(the_day_after_tomorrow_object.contains("low"))
                            {
                                weather_data.weather_daily_3.low = the_day_after_tomorrow_object.take("low").toString();
                                //qDebug() << "the_day_after_tomorrow low : " << weather_data.weather_daily_3.low;
                            }
                            //提取后天的precip
                            if(the_day_after_tomorrow_object.contains("precip"))
                            {
                                weather_data.weather_daily_3.precip = the_day_after_tomorrow_object.take("precip").toString();
                                //qDebug() << "the_day_after_tomorrow precip : " << weather_data.weather_daily_3.precip;
                            }
                            //提取后天的text_day
                            if(the_day_after_tomorrow_object.contains("text_day"))
                            {
                                weather_data.weather_daily_3.text_day = the_day_after_tomorrow_object.take("text_day").toString();
                                //qDebug() << "the_day_after_tomorrow text_day : " << weather_data.weather_daily_3.text_day;
                            }
                            //提取后天的text_night
                            if(the_day_after_tomorrow_object.contains("text_night"))
                            {
                                weather_data.weather_daily_3.text_night = the_day_after_tomorrow_object.take("text_night").toString();
                                //qDebug() << "the_day_after_tomorrow text_night : " << weather_data.weather_daily_3.text_night;
                            }
                            //提取后天的wind_direction
                            if(the_day_after_tomorrow_object.contains("wind_direction"))
                            {
                                weather_data.weather_daily_3.wind_direction = the_day_after_tomorrow_object.take("wind_direction").toString();
                                //qDebug() << "the_day_after_tomorrow wind_direction : " << weather_data.weather_daily_3.wind_direction;
                            }
                            //提取后天的wind_direction_degree
                            if(the_day_after_tomorrow_object.contains("wind_direction_degree"))
                            {
                                weather_data.weather_daily_3.wind_direction_degree = the_day_after_tomorrow_object.take("wind_direction_degree").toString();
                                //qDebug() << "the_day_after_tomorrow wind_direction_degree : " << weather_data.weather_daily_3.wind_direction_degree;
                            }
                            //提取后天的wind_scale
                            if(the_day_after_tomorrow_object.contains("wind_scale"))
                            {
                                weather_data.weather_daily_3.wind_scale = the_day_after_tomorrow_object.take("wind_scale").toString();
                                //qDebug() << "the_day_after_tomorrow wind_scale : " << weather_data.weather_daily_3.wind_scale;
                            }
                            //提取后天的wind_speed
                            if(the_day_after_tomorrow_object.contains("wind_speed"))
                            {
                                weather_data.weather_daily_3.wind_speed = the_day_after_tomorrow_object.take("wind_speed").toString();
                                //qDebug() << "the_day_after_tomorrow wind_speed : " << weather_data.weather_daily_3.wind_speed;
                            }

                        }
                    }
                    //提取last_update
                    if(results_object.contains("last_update"))
                    {
                        //last_update = results_object.take("last_update").toString();
                        //qDebug() << "last_update : " << last_update;
                    }
                    //提取location
                    if(results_object.contains("location"))
                    {
                        QJsonValue location_value = results_object.take("location");
                        //qDebug() << "location_value : " << location_value;
                        QJsonObject location_object = location_value.toObject();
                        //提取country
                        if(location_object.contains("country"))
                        {
                            weather_data.weather_now.country = location_object.take("country").toString();
                            //qDebug() << "country : " << country;
                        }
                        //提取id
                        if(location_object.contains("id"))
                        {
                            weather_data.weather_now.id = location_object.take("id").toString();
                            //qDebug() << "id : " << id;
                        }
                        //提取name
                        if(location_object.contains("name"))
                        {
                            weather_data.weather_now.name = location_object.take("name").toString();
                            //qDebug() << "name : " << name;
                        }
                        //提取path
                        if(location_object.contains("path"))
                        {
                            weather_data.weather_now.path = location_object.take("path").toString();
                            //qDebug() << "path : " << path;
                        }
                        //提取timezone
                        if(location_object.contains("timezone"))
                        {
                            weather_data.weather_now.timezone = location_object.take("timezone").toString();
                            //qDebug() << "timezone : " << timezone;
                        }
                        //提取timezone_offset
                        if(location_object.contains("timezone_offset"))
                        {
                            weather_data.weather_now.timezone_offset = location_object.take("timezone_offset").toString();
                            //qDebug() << "timezone_offset : " << timezone_offset;
                        }
                    }
                }
            }
        }
    }

    flag++;
    if(flag == 3)
    {
       flag = 0;
       emit weather_info_sig(weather_data);
    }
}

void MainWidget::json_analysis_life(QByteArray data)
{
    QByteArray block;
    block = data;
    //qDebug() << "接收 : " << block;
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(block, &json_error);

    //qDebug() << "parse_doucment : " << parse_doucment;
    if(json_error.error == QJsonParseError::NoError)
    {
        if(parse_doucment.isObject())
        {
            //开始解析json对象
            QJsonObject jsonObject = parse_doucment.object();
            if(jsonObject.contains("results"))
            {
                QJsonValue results_value = jsonObject.take("results");
                if(results_value.isArray()) //判断他是不是json数组
                {
                    QJsonArray results_array = results_value.toArray();
                    QJsonObject results_object = results_array.at(0).toObject();
                    //qDebug() << "results_object : " << results_object;

                    //提取suggestion
                    if(results_object.contains("suggestion"))
                    {
                        QJsonValue suggestion_value = results_object.take("suggestion");
                        //qDebug() << "suggestion_value : " << suggestion_value;
                        QJsonObject suggestion_object = suggestion_value.toObject();
                        //提取car_washing
                        if(suggestion_object.contains("car_washing"))
                        {
                            QJsonValue car_washing_value = suggestion_object.take("car_washing");
                            QJsonObject car_washing_object = car_washing_value.toObject();
                            if(car_washing_object.contains("brief"))
                            {
                                weather_data.weather_life.car_washing = car_washing_object.take("brief").toString();
                                //qDebug() << "car_washing : " << car_washing;
                            }
                        }
                        //提取dressing
                        if(suggestion_object.contains("dressing"))
                        {
                            QJsonValue dressing_value = suggestion_object.take("dressing");
                            QJsonObject dressing_object = dressing_value.toObject();
                            if(dressing_object.contains("brief"))
                            {
                                weather_data.weather_life.dressing = dressing_object.take("brief").toString();
                                //qDebug() << "dressing : " << dressing;
                            }
                        }
                        //提取flu
                        if(suggestion_object.contains("flu"))
                        {
                            QJsonValue flu_value = suggestion_object.take("flu");
                            QJsonObject flu_object = flu_value.toObject();
                            if(flu_object.contains("brief"))
                            {
                                weather_data.weather_life.flu = flu_object.take("brief").toString();
                                //qDebug() << "flu : " << flu;
                            }
                        }
                        //提取sport
                        if(suggestion_object.contains("sport"))
                        {
                            QJsonValue sport_value = suggestion_object.take("sport");
                            QJsonObject sport_object = sport_value.toObject();
                            if(sport_object.contains("brief"))
                            {
                                weather_data.weather_life.sport = sport_object.take("brief").toString();
                                //qDebug() << "sport : " << sport;
                            }
                        }
                        //提取travel
                        if(suggestion_object.contains("travel"))
                        {
                            QJsonValue travel_value = suggestion_object.take("travel");
                            QJsonObject travel_object = travel_value.toObject();
                            if(travel_object.contains("brief"))
                            {
                                weather_data.weather_life.travel = travel_object.take("brief").toString();
                                //qDebug() << "travel : " << travel;
                            }
                        }
                        //uv
                        if(suggestion_object.contains("uv"))
                        {
                            QJsonValue uv_value = suggestion_object.take("uv");
                            QJsonObject uv_object = uv_value.toObject();
                            if(uv_object.contains("brief"))
                            {
                                weather_data.weather_life.uv = uv_object.take("brief").toString();
                                //qDebug() << "uv : " << uv;
                            }
                        }
                    }
                }
            }
        }
    }

    flag++;
    if(flag == 3)
    {
       flag = 0;
       emit weather_info_sig(weather_data);
    }
}
