#include "sysinfo_thread.h"

SysInfo_thread::SysInfo_thread(QObject *parent)
    : QThread(parent)
{
    stopped = true;

    pre_cpu_info_data.total = 0;
    pre_cpu_info_data.idle = 0;
    pre_cpu_info_data.iowait = 0;
}

SysInfo_thread::~SysInfo_thread()
{
    stopped = true;
}

void SysInfo_thread::run()
{
    stopped = false;

    while(stopped == false)
    {
        sys_info_data.cpu_info_data = get_cpu_info();
        sys_info_data.mem_info_data = get_mem_info();
        sys_info_data.disk_info_data = get_disk_info();

        emit sys_info_sig(sys_info_data);

        QThread::msleep(500);
    }
}

void SysInfo_thread::stop()
{
    stopped = true;
}

//获得CPU信息
CpuInfo_Data SysInfo_thread::get_cpu_info()
{
    // 初始化为0
    struct CpuInfo_Data cpu_info_data;
    cpu_info_data.usr = 0;
    cpu_info_data.nic = 0;
    cpu_info_data.sys = 0;
    cpu_info_data.idle = 0;
    cpu_info_data.iowait = 0;
    cpu_info_data.irq = 0;
    cpu_info_data.softirq = 0;
    cpu_info_data.steal = 0;
    cpu_info_data.temp = 0;
    cpu_info_data.total = 0;
    cpu_info_data.rate = 0;
    cpu_info_data.freq = 0;

    //获得CPU占用信息
    QString info_file_name("/proc/stat");
    QFileInfo file_info(info_file_name);
    //判断文件是否存在
    if(file_info.isFile())
    {
        //提取文件中的数据
        std::FILE *cpu_fd = std::fopen(info_file_name.toLatin1().data(),"r");
        if(std::fscanf(cpu_fd,"cpu  %lld %lld %lld %lld %lld %lld %lld %lld",\
                    &cpu_info_data.usr,&cpu_info_data.nic,&cpu_info_data.sys, \
                    &cpu_info_data.idle,&cpu_info_data.iowait, \
                    &cpu_info_data.irq,&cpu_info_data.softirq, \
                    &cpu_info_data.steal) < 4)
        {
            qDebug() << "failed to read " << info_file_name;
        }
        std::fclose(cpu_fd);

        //计算CPU总资源数
        cpu_info_data.total = cpu_info_data.usr + cpu_info_data.nic + cpu_info_data.sys + cpu_info_data.idle + cpu_info_data.iowait + cpu_info_data.irq + cpu_info_data.softirq + cpu_info_data.steal;

        long long total = cpu_info_data.total;
        long long idle = cpu_info_data.idle + cpu_info_data.iowait;
        long long pre_total = pre_cpu_info_data.total;
        long long pre_idle = pre_cpu_info_data.idle + pre_cpu_info_data.iowait;

        cpu_info_data.rate = static_cast<double>((double)((total-pre_total) - (idle-pre_idle)) / (double)(total-pre_total)) * 100;
        pre_cpu_info_data = cpu_info_data;

        //qDebug() << QString("cpu rate : %1%").arg(cpu_info_data.rate);
    }
    else
    {
        qDebug() << "No file named : " << info_file_name;
    }

    //获取CPU温度
    QString temp_file_name("/sys/class/thermal/thermal_zone0/temp");
    file_info.setFile(temp_file_name);
    if(file_info.isFile())
    {
        QFile temp_file(temp_file_name);
        if(temp_file.open(QFile::ReadOnly))
        {
            QString cpu_temp_str = temp_file.readAll();
            cpu_info_data.temp = static_cast<double>((double)(cpu_temp_str.toDouble()) / 1000.0);
            temp_file.close();
        }
        else
        {
            qDebug() << QString("read %1 failed...").arg(temp_file_name);
        }
    }
    else
    {
        //qDebug() << "No file named : " << temp_file_name;
    }

    //获取CPU实时频率
    QString freq_file_name("/sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq");
    QFileInfo freq_file_info(freq_file_name);
    if(freq_file_info.isFile())
    {
        QFile freq_file(freq_file_name);
        if(freq_file.open(QFile::ReadOnly))
        {
            QString cpu_freq_str = freq_file.readAll();
            cpu_info_data.freq = static_cast<double>((double)cpu_freq_str.toDouble() / 1000.0);
            freq_file.close();
        }
        else
        {
            qDebug() << QString("read %1 failed...").arg(freq_file_name);
        }
    }
    else
    {
        //qDebug() << "No file named : " << freq_file_name;
    }

    return cpu_info_data;
}

//获取内存信息
MemInfo_Data SysInfo_thread::get_mem_info()
{
    struct MemInfo_Data mem_info_data;
    mem_info_data.total = 0;
    mem_info_data.free  = 0;
    mem_info_data.used = 0;
    mem_info_data.rate = 0;

    QString file_info_name("/proc/meminfo");
    QFileInfo file_info(file_info_name);
    if(file_info.isFile())
    {
        std::FILE *mem_fd = std::fopen(file_info_name.toLatin1().data(),"r");
        std::fscanf(mem_fd,"MemTotal: %lu kB\n",&mem_info_data.total);
        std::fscanf(mem_fd,"MemFree: %lu kB\n",&mem_info_data.free);
        std::fclose(mem_fd);

        mem_info_data.used = mem_info_data.total - mem_info_data.free;
        mem_info_data.rate = static_cast<double>((double)mem_info_data.used / (double)mem_info_data.total) * 100.0;

        mem_info_data.total = mem_info_data.total / 1024;
        mem_info_data.used  = mem_info_data.used  / 1024;
        mem_info_data.free  = mem_info_data.free  / 1024;
    }
    else
    {
        qDebug() << "No file named : " << file_info_name;
    }

    return mem_info_data;
}

//获取硬盘中根目录使用情况
DiskInfo_Data SysInfo_thread::get_disk_info()
{
    struct DiskInfo_Data disk_info_data;
    disk_info_data.total = 0;
    disk_info_data.free  = 0;
    disk_info_data.used = 0;
    disk_info_data.rate = 0;

    struct statfs disk_info;
    QString path = "/";

    if (statfs(path.toLatin1().data(), &disk_info) == -1)
    {
        qDebug() << "Failed to get file disk infomation";
        return disk_info_data;
    }

    disk_info_data.total = disk_info.f_blocks * disk_info.f_bsize;
    disk_info_data.free = disk_info.f_bfree * disk_info.f_bsize;
    disk_info_data.used = disk_info_data.total - disk_info_data.free;
    disk_info_data.rate = static_cast<double>((double)disk_info_data.used / (double)disk_info_data.total) * 100.0;

    disk_info_data.total = disk_info_data.total / 1024 / 1024;
    disk_info_data.free  = disk_info_data.free  / 1024 / 1024;
    disk_info_data.used  = disk_info_data.used  / 1024 / 1024;

    return disk_info_data;
}

