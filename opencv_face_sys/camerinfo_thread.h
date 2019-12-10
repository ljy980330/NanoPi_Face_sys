#ifndef CAMERINFO_THREAD_H
#define CAMERINFO_THREAD_H

#include <QObject>
#include "includes.h"

class CamerInfo_thread : public QThread
{
    Q_OBJECT
public:
    explicit CamerInfo_thread(QObject *parent = 0);
    ~CamerInfo_thread();

    void stop();

    QString video_name;
    int video_width, video_height, video_fps;

    void opencam();
    cv::Mat frame;
    cv::VideoCapture capture;
    struct CamerInfo_Data camer_info_data;

protected:
    void run();

signals:
    void camer_info_sig(struct CamerInfo_Data);

private:
    volatile bool stopped;
};

#endif // CAMERINFO_THREAD_H
