#include "Ekran.h"
#include <QPainter>
#include <QDebug>
#include <iostream>

void drawPixel(QImage* canvas, int x, int y, uint red, uint reen, uint blue) {
    if (x < 0) return;
    if (x >= canvas->width()) return;
    if (y < 0) return;
    if (y >= canvas->height()) return;

    uchar* p = canvas->scanLine(y);
    p[4*x + 0] = blue;  // blue
    p[4*x + 1] = reen;  // reen
    p[4*x + 2] = red;  // red
}

void Ekran::drawLine(int x1, int y1, int x2, int y2, uint red, uint green, uint blue) {
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    double m = (double)(y2 - y1)/(x2 - x1);
    for (int x = x1; x <= x2; ++x) {
        int y = m * (x - x1) + y1;
        drawPixel(&this->canvas, x,(int)std::floor(y + 0.5), red, green, blue);
    }

    update();
}

void Ekran::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Start line";
}

void Ekran::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << " release mouse";
}

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    canvas = QImage(500, 300, QImage::Format_RGB32);
    canvas.fill(0);

    int y = 100;
    int x = 200;

    drawPixel(&canvas, x, y, 255, 0, 0);
}

void Ekran::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.fillRect(0, 0, width(), height(), Qt::yellow);
    p.drawImage(0, 0, canvas);
}

// void Ekran::mouseMoveEvent(QMouseEvent *event)
// {
//     QPoint p = event->pos();
//     p.x();
//     drawPixel(&canvas, p.x(), p.y(), 255, 255, 255);
//     update();
// }
