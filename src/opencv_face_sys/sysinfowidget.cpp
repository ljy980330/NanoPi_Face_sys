#include "sysinfowidget.h"
#include "ui_sysinfowidget.h"

SysInfoWidget::SysInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysInfoWidget)
{
    ui->setupUi(this);

    ui->cpu_info_widget->setColor(QColor(QRgb(0xd9f4fc)), QColor(QRgb(0x12bff3)));
    ui->cpu_temp_widget->setColor(QColor(QRgb(0xfeb9bb)), QColor(QRgb(0xe53439)));
    ui->mem_info_widget->setColor(QColor(QRgb(0xeed8f9)), QColor(QRgb(0x9e2ed6)));
    ui->disk_info_widget->setColor(QColor(QRgb(0xeaf9e2)), QColor(QRgb(0x65c133)));

    this->resize(800,480);

    update_timer = new QTimer(this);
    connect(update_timer,&QTimer::timeout,this,&SysInfoWidget::data_update);
    update_timer->start(500);

    //close时可以进入析构函数
    this->setAttribute(Qt::WA_DeleteOnClose);
}

SysInfoWidget::~SysInfoWidget()
{
    update_timer->stop();

    delete ui;
}

void SysInfoWidget::data_update()
{
    //qDebug() << "data_update";
    ui->cpu_info_widget->setRate(sys_info_data.cpu_info_data.rate);
    ui->cpu_temp_widget->setRate(sys_info_data.cpu_info_data.temp);
    ui->mem_info_widget->setRate(sys_info_data.mem_info_data.rate);
    ui->disk_info_widget->setRate(sys_info_data.disk_info_data.rate);
}

void SysInfoWidget::sysinfo_widget_reset()
{
    ui->cpu_info_widget->dataReset();
    ui->cpu_temp_widget->dataReset();
    ui->mem_info_widget->dataReset();
    ui->disk_info_widget->dataReset();
}

/***********************************************
 *      get system info slots functions
 **********************************************/
void SysInfoWidget::get_sys_info(struct SysInfo_Data data)
{
    //qDebug() << "get_sys_info";
    sys_info_data = data;
    get_cpu_info(data.cpu_info_data);
    get_mem_info(data.mem_info_data);
    get_disk_info(data.disk_info_data);
}

void SysInfoWidget::get_cpu_info(CpuInfo_Data cpu_info_data)
{
    QString cpu_info_str,cpu_temp_str;
    cpu_info_str = QString("CPU信息\n%1%  %2MHz").arg(QString::number(cpu_info_data.rate, 'f', 2)).arg(cpu_info_data.freq);
    cpu_temp_str = QString("CPU温度\n%1°C").arg(QString::number(cpu_info_data.temp,'f',2));

    ui->cpu_info_label->setText(cpu_info_str);
    ui->cpu_temp_label->setText(cpu_temp_str);
}

void SysInfoWidget::get_mem_info(MemInfo_Data mem_info_data)
{
    QString mem_info_str;
    mem_info_str = QString("内存信息\n%1/%2 MB  %3%").arg(mem_info_data.used).arg(mem_info_data.total).arg(QString::number(mem_info_data.rate,'f',2));

    ui->mem_info_label->setText(mem_info_str);
}

void SysInfoWidget::get_disk_info(DiskInfo_Data disk_info_data)
{
    QString disk_info_str;
    double total = static_cast<double>((double)disk_info_data.total / 1024);
    double used = static_cast<double>((double)disk_info_data.used / 1024);

    disk_info_str = QString("硬盘信息\n%1/%2 GB  %3%").arg(QString::number(used,'f',2)).arg(QString::number(total,'f',2)).arg(QString::number(disk_info_data.rate,'f',2));

    ui->disk_info_label->setText(disk_info_str);
}

void SysInfoWidget::mousePressEvent(QMouseEvent *)
{
    emit sysinfo_widget_close_sig();
}
