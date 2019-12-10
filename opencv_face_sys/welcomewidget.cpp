#include "welcomewidget.h"
#include "ui_welcomewidget.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);

    this->resize(800,480);

    welcome_movie = new QMovie(this);
    welcome_movie->setFileName("./image/welcome.gif");
    ui->welcome_label->setMovie(welcome_movie);
    ui->welcome_label->setScaledContents(true);
    welcome_movie->start();

    welcome_timer = new QTimer(this);
    connect(welcome_timer,&QTimer::timeout,this,&WelcomeWidget::welcome_end);
    welcome_timer->setSingleShot(true);
    welcome_timer->start(5000);

    //close时可以进入析构函数
    this->setAttribute(Qt::WA_DeleteOnClose);
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::welcome_end()
{
    emit welcome_end_sig();
    //qDebug() << "welcome end...";
}
