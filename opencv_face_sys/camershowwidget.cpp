#include "camershowwidget.h"

CamerShowWidget::CamerShowWidget(QWidget *parent) : QWidget(parent)
{
    startX = 0;
    startY = 0;

    loadFlag = false;
}

CamerShowWidget::~CamerShowWidget()
{
    if(loadLabel != NULL)
    {
        delete loadLabel;
        loadLabel = NULL;
    }

    if(loadMovie != NULL)
    {
        delete loadLabel;
        loadMovie = NULL;
    }
}

void CamerShowWidget::setCamerPic(cv::Mat pic)
{
    img = pic;
    image = Mat2QImage(pic);

    this->update();
}

void CamerShowWidget::setFindPic(bool flag)
{
    loadFlag = flag;
    if(loadFlag == true)
    {
        //等待认证，图片变暗
        image = Mat2QImage(setBrightness(img, -75));

        loadLabel = new QLabel(this);
        loadMovie = new QMovie(LOAD_FILE_PATH);

        this->update();
        loadLabel->setMovie(loadMovie);
        loadLabel->setScaledContents(true);
        loadMovie->start();
        loadLabel->show();
    }
    else
    {
        loadMovie->stop();
        loadMovie->deleteLater();
        loadLabel->deleteLater();
    }
}

void CamerShowWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if(!image.isNull())
    {
        painter.drawPixmap(startX,startY,this->width(),this->height(),QPixmap::fromImage(image));
    }

    if(loadFlag == true)
    {
        if(this->width() > this->height())
        {
            sideWid = this->width() / 6.0;
        }
        else
        {
            sideWid = this->height() / 6.0;
        }
        loadLabel->setGeometry(startX+sideWid*2,startY+sideWid*2,sideWid*2,sideWid*2);
    }
}

QImage CamerShowWidget::Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.type() == CV_8UC3)                             //3 channels color image
    {
        qImg = QImage(cvImg.data, cvImg.cols, cvImg.rows, cvImg.step, QImage::Format_RGB888);
        return qImg.rgbSwapped();
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols,cvImg.rows,
                    cvImg.cols*cvImg.channels(),
                    QImage::Format_RGB888);
    }

    return qImg;
}

cv::Mat CamerShowWidget::setBrightness(cv::Mat mat, int BrightValue)
{
    using namespace cv;

    double alpha = 1.0;
    double beta = BrightValue;
    Mat new_image = Mat::zeros( mat.size(), mat.type() );

    // 执行运算 new_image(i,j) = alpha*image(i,j) + beta
    for( int y = 0; y < mat.rows; y++ )
    {
        for( int x = 0; x < mat.cols; x++ )
        {
            for( int c = 0; c < 3; c++ )
            {
                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( alpha*( mat.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }

    blur(new_image,new_image,Size(5,5));

    return new_image;
}
