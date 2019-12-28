#ifndef INCLUDES_H
#define INCLUDES_H

// include c header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// include c++ header
#include <iostream>
#include <string>
#include <cstdio>

// include linux header
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/statfs.h>
#include <sys/vfs.h>

// include qt header
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QThread>
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QTextCodec>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QMovie>
#include <QFontDatabase>
#include <QtNetwork>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QProcess>
#include <QMetaType>
#include <QLabel>
#include <QMutex>
#include <QStringList>
#include <QProcess>

// include opencv header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "videoio.hpp"

// include face++ header
#include "facepp/FaceppApi.hpp"

// rtc struct
struct RTC_Data
{
    QString time_text;
    QString week;
};

// weather struct
struct Weather_Now_Data
{
    QString last_update; //最后一次更新时间
    QString country; //国家
    QString id; //ID号
    QString name; //名字
    QString path; //地点
    QString timezone; //时区
    QString timezone_offset; //时区偏移
    QString code; //天气气象代码
    QString temperature; //温度
    QString text; //天气气象文字

};

struct Weather_Life_Data
{
    QString car_washing; //洗车
    QString dressing; //穿衣
    QString flu; //感冒
    QString sport; //运动
    QString travel; //旅游
    QString uv; //紫外线
};

struct Weather_Daily_Data
{
    QString date; //返回日期
    QString text_day; //白天天气现象文字
    QString code_day; //白天天气现象代码
    QString text_night; //晚间天气现象文字
    QString code_night; //晚间天气现象代码
    QString high; //当天最高温度
    QString low; //当天最低温度
    QString precip; //降水概率 范围0~100，单位百分比
    QString wind_direction; //风向文字
    QString wind_direction_degree; //风向角度 范围0~360
    QString wind_speed; //风速，单位km/h
    QString wind_scale; //风力等级
};

struct Weather_Data
{
    struct Weather_Now_Data weather_now;
    struct Weather_Daily_Data weather_daily_1, weather_daily_2, weather_daily_3;
    struct Weather_Life_Data weather_life;
};

// system info struct
struct CpuInfo_Data
{
    long long total;
    long long usr;
    long long nic;
    long long sys;
    long long idle;
    long long iowait;
    long long irq;
    long long softirq;
    long long steal;
    double freq;
    double rate;
    double temp;
};

struct MemInfo_Data
{
    long unsigned int total;
    long unsigned int free;
    long unsigned int used;
    double rate;
};

struct DiskInfo_Data
{
    long long total;
    long long free;
    long long used;
    double rate;
};

struct SysInfo_Data
{
    struct CpuInfo_Data cpu_info_data;
    struct MemInfo_Data mem_info_data;
    struct DiskInfo_Data disk_info_data;
};

// opencv camer info struct
struct CamerInfo_Data
{
    cv::Mat frame;
    double fps;
};

//facepp struct
struct ThresholdsDate
{
    double thresholds_3;
    double thresholds_4;
    double thresholds_5;
};

struct ResultsDate
{
    QString face_token;
    double confidence;
    QString user_id;
};

struct FaceSearchData
{
    QString request_id;
    struct ResultsDate results;
    QString image_id;
    struct ThresholdsDate thresholds;
    int time_used;
    QString error_message;
};

struct FaceGetDetailResponse
{
    QString request_id;
    QString image_id;
    QString face_token;
    QString user_id;
    int time_used;
    QString error_message;
};

#define VIDEO_NAME_INIT "/dev/video0"
#define VIDEO_WIDTH_INIT 640
#define VIDEO_HEIGH_INIT 480
#define VIDEO_SETFILE "./data/opencv_set"

#define APIkey "jnKErmEcRWpMI1UHB2rNZF10AUxss_8-"
#define APIsecre "1wvLR6qXsPrhQHAwmv3ekjM-JbN8b9hH"
#define FaceSetToken "ce5a1743232bd1763f0b15772e9ce0e0"

#define LOAD_FILE_PATH "./image/load2.gif"

#endif // INCLUDES_H
