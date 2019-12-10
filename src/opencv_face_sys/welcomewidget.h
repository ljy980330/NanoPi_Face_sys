#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include "includes.h"

namespace Ui {
class WelcomeWidget;
}

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWidget(QWidget *parent = 0);
    ~WelcomeWidget();

    QMovie *welcome_movie;
    QTimer *welcome_timer;

protected slots:
    void welcome_end();

signals:
    void welcome_end_sig();

private:
    Ui::WelcomeWidget *ui;
};

#endif // WELCOMEWIDGET_H
