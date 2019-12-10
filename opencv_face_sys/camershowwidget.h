#ifndef CAMERSHOWWIDGET_H
#define CAMERSHOWWIDGET_H

#include <QWidget>
#include "includes.h"

class CamerShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CamerShowWidget(QWidget *parent = 0);
    ~CamerShowWidget();

    QImage image;
    cv::Mat img;

    int startX,startY;
    int sideWid;

    bool loadFlag;
    QLabel *loadLabel;
    QMovie *loadMovie;

    void setCamerPic(cv::Mat pic);
    void setFindPic(bool flag);

    QImage Mat2QImage(cv::Mat cvImg);
    cv::Mat setBrightness(cv::Mat mat, int BrightValue);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // CAMERSHOWWIDGET_H
