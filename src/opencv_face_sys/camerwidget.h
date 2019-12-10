#ifndef CAMERWIDGET_H
#define CAMERWIDGET_H

#include <QWidget>
#include "includes.h"
#include "facefind_thread.h"

namespace Ui {
class CamerWidget;
}

class CamerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CamerWidget(QWidget *parent = 0);
    ~CamerWidget();

    QString faceNamePath = "./data/name";
    QStringList faceTokenList;
    QStringList faceNameList;

    QString soundFilePath = "../sound/sound";

    QString nameStr = "姓名 : ";
    QString confidenStr = "匹配度 : ";
    QString resultStr = "结果 : ";
    QString TimeStr = "登记时间 : ";

    QLabel msgLabel;

    int weather_widget_startX,weather_widget_startY;
    int sysinfo_widget_startX,sysinfo_widget_startY;

    void weather_now_updata(struct Weather_Now_Data weather_now_data);

    QString faceCascadeName = "./haarcascades/haarcascade_frontalface_alt2.xml";
    cv::CascadeClassifier faceCascade;
    cv::Mat faceCasePic(cv::Mat img);

    QString faceFilePath = "./data/";

    QString faceImagePath = "./face.jpg";
    cv::Mat faceROI;
    bool getFaceROI();

    struct CamerInfo_Data camer_info;
    struct FaceSearchData faceSearch;
    QTimer *findFaceDelay_timer;
    QTimer *delay_timer;

    bool delay_flag;
    bool findFace_flag;
    bool faceshow_flag;

    QImage Mat2QImage(cv::Mat cvImg);

public slots:
    void get_camer_info(struct CamerInfo_Data camer_info_data);

    void rtc_updata(struct RTC_Data rtc_data);

    void get_weather_info(struct Weather_Data weather_data);

    void get_face_search(struct FaceSearchData data);

protected slots:
    void findFaceDelay_update();
    void delay_update();

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void mousePress_sig(int flag);

private:
    Ui::CamerWidget *ui;

    FaceFind_thread *facefind_thread;
};

#endif // CAMERWIDGET_H
