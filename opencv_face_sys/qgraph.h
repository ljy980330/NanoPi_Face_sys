#ifndef QGRAPH_H
#define QGRAPH_H

#include <QWidget>
#include "includes.h"

#define ROWS 5
#define COLS 20

class QGraph : public QWidget
{
    Q_OBJECT
public:
    explicit QGraph(QWidget *parent = 0);
    ~QGraph();

    QColor background_color, line_color;

    double graph_heigh,graph_width;
    int startX,startY;

    void setColor(QColor bgcolor, QColor lcolor);
    void setRate(double rate);

    void dataReset();

protected:
    void paintEvent(QPaintEvent *);

private:
    int cur;
    int length = COLS*2+2;
    double RateData[COLS*2+2];
};

#endif // QGRAPH_H
