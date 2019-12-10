#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "includes.h"

// include all Widget headers
#include "welcomewidget.h"
#include "camerwidget.h"
#include "weatherwidget.h"
#include "sysinfowidget.h"

// include all thread headers
#include "sysinfo_thread.h"
#include "camerinfo_thread.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    QTimer *rtc_timer;
    QDateTime date_time;
    QDate date;
    struct RTC_Data rtc_data;

    QTimer *weather_timer;
    int flag;
    struct Weather_Data weather_data;
    QString url_weather_now;
    QString url_weather_daily;
    QString url_weather_life;
    void json_analysis_now(QByteArray data);   //解析json格式现在天气
    void json_analysis_daliy(QByteArray data); //解析json格式逐日时间
    void json_analysis_life(QByteArray data);  //解析json格式生活指数

protected:
    void paintEvent(QPaintEvent *); //界面重绘事件
    void resizeEvent(QResizeEvent *); //窗体大小改变事件

    QPixmap BackGroudImage; //背景图片
    QFont font; //字体

signals:
    void rtc_time_sig(struct RTC_Data);

    void weather_info_sig(struct Weather_Data);

protected slots:
    void camer_widget_setup();

    void camer_widget_weather_press(int flag);
    void weather_widget_close();
    void sysinfo_widget_close();

    void rtc_update();
    void weather_update();

private:
    Ui::MainWidget *ui;

    // ui pointer
    WelcomeWidget *welcome_widget_ui;
    CamerWidget *camer_widget_ui;
    WeatherWidget *weather_widget_ui;
    SysInfoWidget *sysinfo_widget_ui;

    // thread pointer
    SysInfo_thread *sys_info_thread;
    CamerInfo_thread *camer_info_thread;

    // weather pointer
    QNetworkAccessManager *m_NetManger_now;
    QNetworkAccessManager *m_NetManger_daily;
    QNetworkAccessManager *m_NetManger_life;
    QNetworkReply *m_Reply_now;
    QNetworkReply *m_Reply_daily;
    QNetworkReply *m_Reply_life;
    unsigned int NetWorkFlag;

private slots:
    void finishedSlot_now(QNetworkReply*);
    void finishedSlot_daily(QNetworkReply*);
    void finishedSlot_life(QNetworkReply*);
};

#endif // MAINWIDGET_H
