#include "weatherwidget.h"
#include "ui_weatherwidget.h"

WeatherWidget::WeatherWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherWidget)
{
    ui->setupUi(this);

    this->resize(800,480);

    //close时可以进入析构函数
    this->setAttribute(Qt::WA_DeleteOnClose);
}

WeatherWidget::~WeatherWidget()
{
    delete ui;
}

void WeatherWidget::get_weather_info(Weather_Data weather_data)
{
    weather_now_updata(weather_data.weather_now);
    weather_daily_updata(weather_data.weather_daily_1,weather_data.weather_daily_2,weather_data.weather_daily_3);
    weather_life_updata(weather_data.weather_life);
}

void WeatherWidget::weather_now_updata(Weather_Now_Data weather_now)
{
    //显示天气实况
    ui->temp_lcdNumber_2->display(weather_now.temperature);
    ui->path_label_2->setText(weather_now.name);
    ui->weather_text_label_2->setText(weather_now.text);

    weather_now.last_update.replace(10,1,' ');
    weather_now.last_update.replace(19,6,' ');
    ui->last_updata_label->setText(tr("最后更新时间：%1").arg(weather_now.last_update));
    //天气图片
    QString weather_ico_path;
    weather_ico_path = "./image/black/" + weather_now.code + "@2x.png";
    QPixmap weatherprint(weather_ico_path);
    ui->weather_code_label_2->setPixmap(weatherprint);
    ui->weather_code_label_2->setScaledContents(true);
    ui->weather_code_label_2->show();
}

void WeatherWidget::weather_daily_updata(struct Weather_Daily_Data weather_daily_1,struct Weather_Daily_Data weather_daily_2,struct Weather_Daily_Data weather_daily_3)
{
    //今天
    ui->daily_today_text_label->setText(weather_daily_1.text_day);
    ui->daily_today_label->setText(weather_daily_1.date);
    ui->daily_today_temp_label->setText(tr("温度：%1/%2").arg(weather_daily_1.high).arg(weather_daily_1.low));
    ui->daily_today_wind_direction_label->setText(tr("风向：%1").arg(weather_daily_1.wind_direction));
    ui->daily_today_wind_speed_label->setText(tr("风力：%1级").arg(weather_daily_1.wind_scale));
    //天气图片
    QString today_weather_ico_path;
    today_weather_ico_path = "./image/black/" + weather_daily_1.code_day + "@2x.png";
    QPixmap today_weatherprint(today_weather_ico_path);
    ui->daily_today_code_label->setPixmap(today_weatherprint);
    ui->daily_today_code_label->setScaledContents(true);
    ui->daily_today_code_label->show();

    //明天
    ui->daily_tomorrow_text_label->setText(weather_daily_2.text_day);
    ui->daily_tomorrow_label->setText(weather_daily_2.date);
    ui->daily_tomorrow_temp_label->setText(tr("温度：%1/%2").arg(weather_daily_2.high).arg(weather_daily_2.low));
    ui->daily_tomorrow_wind_direction_label->setAlignment(Qt::AlignHCenter);
    ui->daily_tomorrow_wind_direction_label->setText(tr("风向：%1").arg(weather_daily_2.wind_direction));
    ui->daily_tomorrow_wind_speed_label->setText(tr("风力：%1级").arg(weather_daily_2.wind_scale));
    //天气图片
    QString tomorrow_weather_ico_path;
    tomorrow_weather_ico_path = "./image/black/" + weather_daily_2.code_day + "@2x.png";
    QPixmap tomorrow_weatherprint(tomorrow_weather_ico_path);
    ui->daily_tomorrow_code_label->setPixmap(tomorrow_weatherprint);
    ui->daily_tomorrow_code_label->setScaledContents(true);
    ui->daily_tomorrow_code_label->show();

    //后天
    ui->daily_the_day_after_text_label_2->setText(weather_daily_3.text_day);
    ui->daily_the_day_after_label->setText(weather_daily_3.date);
    ui->daily_the_day_after_temp_label_2->setText(tr("温度：%1/%2").arg(weather_daily_3.high).arg(weather_daily_3.low));
    ui->daily_the_day_after_wind_direction_label_2->setText(tr("风向：%1").arg(weather_daily_3.wind_direction));
    ui->daily_the_day_after_wind_speed_label_2->setText(tr("风力：%1级").arg(weather_daily_3.wind_scale));
    //天气图片
    QString the_day_after_weather_ico_path;
    the_day_after_weather_ico_path = "./image/black/" + weather_daily_3.code_day + "@2x.png";
    QPixmap the_day_after_weatherprint(the_day_after_weather_ico_path);
    ui->daily_the_day_after_code_label_2->setPixmap(the_day_after_weatherprint);
    ui->daily_the_day_after_code_label_2->setScaledContents(true);
    ui->daily_the_day_after_code_label_2->show();
}

void WeatherWidget::weather_life_updata(Weather_Life_Data weather_life)
{

    //生活指数
    ui->car_washing_label->setText(tr("洗车：%1").arg(weather_life.car_washing));
    ui->dressing_label->setText(tr("穿衣：%1").arg(weather_life.dressing));
    ui->flu_label->setText(tr("感冒：%1").arg(weather_life.flu));
    ui->sport_label->setText(tr("运动：%1").arg(weather_life.sport));
    ui->travel_label->setText(tr("旅游：%1").arg(weather_life.travel));
    ui->uv_label->setText(tr("紫外线：%1").arg(weather_life.uv));
}

void WeatherWidget::mousePressEvent(QMouseEvent *)
{
    //qDebug() << "weather_widget_close_sig";
    emit weather_widget_close_sig();
}

void WeatherWidget::rtc_updata(struct RTC_Data rtc_data)
{
    ui->localtime_label_2->setText(rtc_data.time_text);
    ui->week_label_2->setText(rtc_data.week);
}
