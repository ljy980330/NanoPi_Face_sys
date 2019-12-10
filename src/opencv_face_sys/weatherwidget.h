#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QWidget>
#include "includes.h"

namespace Ui {
class WeatherWidget;
}

class WeatherWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherWidget(QWidget *parent = 0);
    ~WeatherWidget();

    void weather_now_updata(struct Weather_Now_Data weather_now);
    void weather_daily_updata(struct Weather_Daily_Data weather_daily_1,struct Weather_Daily_Data weather_daily_2,struct Weather_Daily_Data weather_daily_3);
    void weather_life_updata(struct Weather_Life_Data weather_life);

public slots:
    void get_weather_info(struct Weather_Data weather_data);
    void rtc_updata(struct RTC_Data rtc_data);

signals:
    void weather_widget_close_sig();

protected:
    void mousePressEvent(QMouseEvent *);

private:
    Ui::WeatherWidget *ui;
};

#endif // WEATHERWIDGET_H
