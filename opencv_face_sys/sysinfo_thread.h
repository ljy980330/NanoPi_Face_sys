#ifndef SYSINFO_THREAD_H
#define SYSINFO_THREAD_H

#include <QObject>
#include "includes.h"

class SysInfo_thread : public QThread
{
    Q_OBJECT
public:
    explicit SysInfo_thread(QObject *parent = 0);
    ~SysInfo_thread();

    void stop();

    SysInfo_Data sys_info_data;

    CpuInfo_Data pre_cpu_info_data;

    CpuInfo_Data get_cpu_info();
    MemInfo_Data get_mem_info();
    DiskInfo_Data get_disk_info();

protected:
    void run();

signals:
    void sys_info_sig(struct SysInfo_Data);

private:
    volatile bool stopped;
};

#endif // SYSINFO_THREAD_H
