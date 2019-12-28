#include "camerwidget.h"
#include "ui_camerwidget.h"

CamerWidget::CamerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CamerWidget)
{
    ui->setupUi(this);

    msgLabel.setParent(this);
    QFont font;
    font.setPointSize(50);
    msgLabel.setFont(font);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255, 255, 255));
    msgLabel.setAutoFillBackground(true);  //一定要这句，否则不行
    msgLabel.setPalette(palette);

    msgLabel.hide();

    this->resize(800,480);

    qRegisterMetaType<FaceSearchData>("FaceSearchData");
    qRegisterMetaType<FaceGetDetailResponse>("FaceGetDetailResponse");

    delay_flag = false;
    findFace_flag = false;
    faceshow_flag = false;

    //支持Qt中文字库
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //init system info label
    ui->name_info_label->setText(nameStr);
    ui->confiden_info_label->setText(confidenStr);
    ui->result_info_label->setText(resultStr);
    ui->time_info_label->setText(TimeStr);

    //初始化人脸检测文件
    if(!faceCascade.load(faceCascadeName.toStdString()))
    {
        qDebug() << QString("Error loading %1. Exiting!").arg(faceCascadeName);
        this->close();
    }

    findFaceDelay_timer = new QTimer(this);
    connect(findFaceDelay_timer,&QTimer::timeout,this,&CamerWidget::findFaceDelay_update);

    delay_timer = new QTimer(this);
    connect(delay_timer,&QTimer::timeout,this,&CamerWidget::delay_update);

    facefind_thread = new FaceFind_thread;
    connect(facefind_thread,&FaceFind_thread::face_search_sig,this,&CamerWidget::get_face_search);

//    QFile file(faceNamePath);
//    if(file.open(QFile::ReadOnly))
//    {
//        while(!file.atEnd())
//        {
//            QString strLine = file.readLine();
//            if(strLine == "\n")
//                break;
//            strLine.chop(1);
//            faceTokenList.append(strLine.split(" ")[0]);
//            faceNameList.append(strLine.split(" ")[1]);
//        }
//    }
//    else
//    {
//        qDebug() << faceNamePath << " read failed!";
//    }

    //close时可以进入析构函数
    this->setAttribute(Qt::WA_DeleteOnClose);
}

CamerWidget::~CamerWidget()
{
    if(facefind_thread->isRunning())
    {
        facefind_thread->wait();
    }
    facefind_thread->quit();
    delete facefind_thread;

    if(delay_timer->isActive())
    {
        delay_timer->stop();
    }

    if(findFaceDelay_timer->isActive())
    {
        findFaceDelay_timer->stop();
    }

    delete ui;
}

void CamerWidget::get_face_search(struct FaceSearchData data)
{
    int ret;
    faceSearch = data;
    double time_s = static_cast<double>((double)faceSearch.time_used / 1000.0);

    ui->camer_widget->setFindPic(false);
    faceshow_flag = false;

    if(!faceSearch.error_message.isNull())
    {
        ret = NETWORK_ERROR;
        goto update_ui;
    }

    if(faceSearch.results.face_token.isNull())
    {
        ret = FAILED_NOPERSON;
        goto update_ui;
    }

    if(faceSearch.results.confidence > faceSearch.thresholds.thresholds_5)
    {
       QString name = faceSearch.results.user_id;

       ui->confiden_info_label->setText(QString("%1%2%").arg(confidenStr).arg(QString::number(faceSearch.results.confidence,'f',2)));
       ui->name_info_label->setText(QString("%1%2").arg(nameStr).arg(name));
       ui->result_info_label->setText(QString("%1识别成功 (%2S)").arg(resultStr).arg(QString::number(time_s,'f',2)));
       //QProcess::execute(QString("%1 %2").arg(soundFilePath).arg("识别成功"));
       QString uartStr = QString("%1,%2").arg(name).arg("欢迎");
       QProcess::execute(QString("%1 %2").arg(soundFilePath).arg(uartStr));

       QString filePath = faceFilePath+faceSearch.results.face_token;
       QFileInfo fileInfo(filePath);
       if(!fileInfo.isFile())
       {
           QFile file(filePath);
           if(file.open(QFile::WriteOnly))
           {
               file.close();
           }
       }
       QFile file(filePath);
       if(file.open(QFile::WriteOnly|QFile::Append))
       {
           QDateTime datetime = QDateTime::currentDateTime();
           QString time_str = datetime.toString("yyyy-MM-dd HH:mm:ss");
           file.write(time_str.toLatin1());
           file.write(QString("\n").toLatin1());
           file.close();
           ui->time_info_label->setText(QString("%1%2").arg(TimeStr).arg(time_str));
           //qDebug() << "write OK";
       }
       else
       {
           qDebug() << "write failed...." << filePath;
       }

       delay_timer->setSingleShot(true);
       delay_timer->start(5000);
       delay_flag = true;

       return ;
    }
    else
    {
        ret = FAILED_MOVE;
        goto update_ui;
    }

update_ui:
    UpdateUiShow(ret, time_s);
}

void CamerWidget::UpdateUiShow(int message, double time_s)
{
    switch(message)
    {
        case NETWORK_ERROR:
            msgLabel.clear();
            msgLabel.setText("网络错误\n请检查网络连接");
            msgLabel.setGeometry(2*(this->width())/6,2*(this->height())/6,2*(this->width())/6,2*(this->height())/6);
            msgLabel.show();
            ui->camer_widget->setFindPic(false);
            delay_timer->setSingleShot(true);
            delay_timer->start(2000);
            delay_flag = true;
            break;
        case FAILED_NOPERSON:
            ui->result_info_label->setText(QString("%1识别失败 (%2S)").arg(resultStr).arg(QString::number(time_s,'f',2)));
            msgLabel.setText("识别失败\n无此人信息,请联系管理员");
            msgLabel.setGeometry(2*(this->width())/6,2*(this->height())/6,2*(this->width())/6,2*(this->height())/6);
            msgLabel.show();
            QProcess::execute(QString("%1 %2").arg(soundFilePath).arg("识别失败,无此人信息,请联系管理员"));
            delay_timer->setSingleShot(true);
            delay_timer->start(2000);
            delay_flag = true;
            break;
        case FAILED_MOVE:
            ui->result_info_label->setText(QString("%1识别失败 (%2S)").arg(resultStr).arg(QString::number(time_s,'f',2)));
            msgLabel.setText("识别失败\n请不要移动");
            msgLabel.setGeometry(2*(this->width())/6,2*(this->height())/6,2*(this->width())/6,2*(this->height())/6);
            msgLabel.show();
            QProcess::execute(QString("%1 %2").arg(soundFilePath).arg("识别失败,请不要移动"));
            delay_timer->setSingleShot(true);
            delay_timer->start(2000);
            delay_flag = true;
            break;
    }
}

void CamerWidget::findFaceDelay_update()
{
    faceshow_flag = true;
    if(getFaceROI() == true)
    {
        cv::imwrite(faceImagePath.toLatin1().data(), faceROI);
        ui->camer_widget->setFindPic(true);
        facefind_thread->start();
    }
    else
    {
        faceshow_flag = false;
        delay_update();
    }
}

void CamerWidget::delay_update()
{
    msgLabel.hide();
    findFace_flag = false;
    delay_flag = false;
    ui->name_info_label->setText(nameStr);
    ui->confiden_info_label->setText(confidenStr);
    ui->result_info_label->setText(resultStr);
    ui->time_info_label->setText(TimeStr);
}

void CamerWidget::get_camer_info(struct CamerInfo_Data camer_info_data)
{
    if(faceshow_flag == false)
    {
        camer_info = camer_info_data;
        if(delay_flag == false)
            ui->camer_widget->setCamerPic(faceCasePic(camer_info_data.frame));
        else
            ui->camer_widget->setCamerPic(camer_info_data.frame);
    }
}

void CamerWidget::rtc_updata(struct RTC_Data rtc_data)
{
    ui->localtime_label->setText(rtc_data.time_text);
    ui->week_label->setText(rtc_data.week);
}

void CamerWidget::mousePressEvent(QMouseEvent *event)
{
    //摄像头图像区域 1
    //人员信息区域 2
    //天气信息区域 3

    int mouse_press_x,mouse_press_y; //鼠标点击时的坐标
    mouse_press_x = event->x();
    mouse_press_y = event->y();

    sysinfo_widget_startX = this->width() - ui->info_data_widget->width();
    sysinfo_widget_startY = ui->info_data_widget->height();

    weather_widget_startX = this->width() - ui->info_weather_widget->width();
    weather_widget_startY = this->height() - ui->info_weather_widget->height();

    if((mouse_press_x > sysinfo_widget_startX) && (mouse_press_x < this->width()))
    {
        if((mouse_press_y > 0) && (mouse_press_y < sysinfo_widget_startY))
        {
            emit mousePress_sig(2);
        }
    }

    if((mouse_press_x > weather_widget_startX) && (mouse_press_x < this->width()))
    {
        if((mouse_press_y > weather_widget_startY) && (mouse_press_y < this->height()))
        {
            emit mousePress_sig(3);
        }
    }
}

void CamerWidget::get_weather_info(Weather_Data weather_data)
{
    weather_now_updata(weather_data.weather_now);
    //qDebug() << "weather_now_updata";
}

void CamerWidget::weather_now_updata(struct Weather_Now_Data weather_now_data)
{
    //显示天气实况
    ui->temp_lcdNumber->display(weather_now_data.temperature);
    ui->path_label->setText(weather_now_data.name);
    ui->weather_text_label->setText(weather_now_data.text);

    //天气图片
    QString weather_ico_path;
    weather_ico_path = "./image/black/" + weather_now_data.code + "@2x.png";
    QPixmap weatherprint(weather_ico_path);
    ui->weather_code_label->setPixmap(weatherprint);
    ui->weather_code_label->setScaledContents(true);
    ui->weather_code_label->show();
}

cv::Mat CamerWidget::faceCasePic(cv::Mat img)
{
    using namespace cv;

    Mat frame, frameGray;
    frame = img.clone();

    std::vector<Rect> faces;

    //转换灰度
    cvtColor(frame, frameGray, CV_BGR2GRAY);

    //均衡直方图
    equalizeHist(frameGray, frameGray);

    //检测脸部
    faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(frame.cols/4, frame.rows/4));

    //框出人脸
    if(faces.size() != 1)
        return frame;

    for(size_t i=0; i < faces.size(); i++)
    {
        Rect feceRect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        rectangle(frame, feceRect, Scalar(0,0,255), 3, LINE_AA);
    }

    if(findFace_flag == false)
    {
        findFace_flag = true;
        findFaceDelay_timer->setSingleShot(true);
        findFaceDelay_timer->start(1000);
    }

    return frame;
}

bool CamerWidget::getFaceROI()
{
    using namespace cv;

    Mat frame, frameGray;
    frame = camer_info.frame;

    std::vector<Rect> faces;

    //转换灰度
    cvtColor(frame, frameGray, CV_BGR2GRAY);

    //均衡直方图
    equalizeHist(frameGray, frameGray);

    //检测脸部
    faceCascade.detectMultiScale(frameGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(frame.cols/4, frame.rows/4));

    if(faces.size() != 1)
    {
        return false;
    }

    int x = faces[0].x - int(0.0*faces[0].width);
    int y = faces[0].y - int(0.0*faces[0].height);
    int w = int(1 * faces[0].width);
    int h = int(1 * faces[0].height);
    //qDebug() << QString("(%1,%2,%3,%4)").arg(x).arg(y).arg(w).arg(h);

    faceROI = frame(Rect(x,y,w,h));
    return true;
}
