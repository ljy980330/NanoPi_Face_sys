#include "camerinfo_thread.h"

CamerInfo_thread::CamerInfo_thread(QObject *parent) :
    QThread(parent)
{
    stopped = true;

    QFile file_set(VIDEO_SETFILE);
    if(file_set.open(QFile::ReadOnly))
    {
        QString line_str;
        line_str = file_set.readLine();
        line_str.chop(1);
        video_name = line_str;
        line_str = file_set.readLine();
        line_str.chop(1);
        video_width = line_str.toInt();
        line_str = file_set.readLine();
        line_str.chop(1);
        video_height = line_str.toInt();
        line_str = file_set.readLine();
        line_str.chop(1);
        video_fps = line_str.toInt();

//        qDebug() << "video_name: " << video_name;
//        qDebug() << "video_width: " << video_width;
//        qDebug() << "video_height: " << video_height;

        file_set.close();
    }
    else
    {
        qDebug() << "can not open " << VIDEO_SETFILE;
        video_name = VIDEO_NAME_INIT;
        video_height = VIDEO_HEIGH_INIT;
        video_width = VIDEO_WIDTH_INIT;
    }

    QFile came_file(video_name);
    if(came_file.open(QFile::ReadOnly))
    {
        came_file.close();
        opencam();
    }
    else
    {
        qDebug() << "No device named " << video_name;
    }
}

CamerInfo_thread::~CamerInfo_thread()
{
    stopped = true;

    if (capture.isOpened())
    {
        capture.release();
    }
}

void CamerInfo_thread::run()
{
    stopped = false;

    camer_info_data.fps = capture.get(CV_CAP_PROP_FPS); //get camer fps
    qDebug() << camer_info_data.fps;

    while(stopped == false)
    {
        if (capture.isOpened())
        {
            capture >> frame;
            if (!frame.empty())
            {
                camer_info_data.frame = frame;
                emit camer_info_sig(camer_info_data);
            }
        }
        else
        {
            qDebug() << video_name << " is not open...\nexit thread...";
            break;
        }
    }
}

void CamerInfo_thread::stop()
{
    stopped = true;
}

void CamerInfo_thread::opencam()
{
    if (capture.isOpened())
            capture.release();     //decide if capture is already opened; if so,close it
    capture.open(video_name.toStdString(), cv::CAP_V4L);           //open the default camera
    capture.set(CV_CAP_PROP_FRAME_WIDTH, video_width);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, video_height);
    capture.set(CV_CAP_PROP_FPS, video_fps);
}
