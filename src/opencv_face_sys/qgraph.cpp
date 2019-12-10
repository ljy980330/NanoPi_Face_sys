#include "qgraph.h"

QGraph::QGraph(QWidget *parent) : QWidget(parent)
{
    startX = 0;
    startY = 0;
    background_color = Qt::black;
    line_color = Qt::white;

    cur = 0;
    for(int i = 0; i < length; i++)
    {
        RateData[i] = 1.00;
        //qDebug() << i << " " << RateData[i];
    }
}

QGraph::~QGraph()
{

}

void QGraph::setColor(QColor bgcolor, QColor lcolor)
{
    background_color = bgcolor;
    line_color = lcolor;
}

void QGraph::setRate(double rate)
{
    double Rate;
    Rate = static_cast<double>(rate / 100.00);

    cur = (cur+1) % length;
    RateData[cur] = (1.00 - Rate);

    //qDebug() << QString("RateData[%1] : %2").arg(cur).arg(QString::number(RateData[cur],'f',6));
    this->update();
}

void QGraph::dataReset()
{
    for(int i = 0; i < length; i++)
    {
        RateData[i] = 1.00;
        //qDebug() << i << " " << RateData[i];
    }
}

void QGraph::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(background_color);

    painter.setRenderHint(QPainter::Antialiasing, true);  //设置渲染,启动反锯齿

    //画背景
    graph_heigh = static_cast<double>((double)this->height() / ROWS);
    graph_width = static_cast<double>((double)this->width()  / COLS);

    for(int row = 0; row < ROWS+1; row++)
    {
        painter.drawLine(QPointF(startX,startY+row*graph_heigh),QPointF(startX+COLS*graph_width,startY+row*graph_heigh));
    }
    for(int col = 0; col < COLS+1; col++)
    {
        painter.drawLine(QPointF(startX+col*graph_width,startY),QPointF(startX+col*graph_width,startY+ROWS*graph_heigh));
    }

    //画波形图
    int bg_r,bg_g,bg_b,l_r,l_g,l_b,r,g,b;
    line_color.getRgb(&l_r,&l_g,&l_b);
    background_color.getRgb(&bg_r,&bg_g,&bg_b);
    r = (l_r + bg_r) / 2;
    g = (l_g + bg_g) / 2;
    b = (l_b + bg_b) / 2;
    QColor brush_color(r,g,b,100);
    //qDebug() << QString("(%1,%2,%3)").arg(r).arg(g).arg(b);

    QBrush line_brush;
    line_brush.setColor(brush_color);
    line_brush.setStyle(Qt::SolidPattern);

    QPen line_pen;
    line_pen.setColor(line_color);
    line_pen.setWidth(3);
    line_pen.setStyle(Qt::SolidLine);

    painter.setPen(line_pen);
    painter.setBrush(line_brush);

    int line_width = this->width() / (COLS*2);
    int index = cur;
    QPointF point;
    QPainterPath linePath(QPointF(startX-5,this->height()));

    for(int i = 0; i < length; i++)
    {
        index = (index+1) % length;
        point.setX(startX+i*line_width);
        point.setY(static_cast<double>((double)(this->height())*RateData[index]));
        linePath.lineTo(point);
        //qDebug() << point;
        //qDebug() << QString("%1 RateData[%4]:%5").arg(i).arg(index).arg(QString::number(RateData[index],'f',6));
    }
    //linePath.lineTo(QPointF(this->width()+5,this->height()));
    linePath.lineTo(QPointF(startX+COLS*graph_width,startY+ROWS*graph_heigh));

    painter.drawPath(linePath);
}
