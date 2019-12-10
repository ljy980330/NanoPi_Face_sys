#ifndef SYSINFOWIDGET_H
#define SYSINFOWIDGET_H

#include <QWidget>
#include "includes.h"

namespace Ui {
class SysInfoWidget;
}

class SysInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SysInfoWidget(QWidget *parent = 0);
    ~SysInfoWidget();

    SysInfo_Data sys_info_data;
    QTimer *update_timer;

    void get_cpu_info(CpuInfo_Data cpu_info_data);
    void get_mem_info(MemInfo_Data mem_info_data);
    void get_disk_info(DiskInfo_Data disk_info_data);

    void sysinfo_widget_reset();

public slots:
    void get_sys_info(struct SysInfo_Data data);

    void data_update();

signals:
    void sysinfo_widget_close_sig();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    Ui::SysInfoWidget *ui;
};

#endif // SYSINFOWIDGET_H
