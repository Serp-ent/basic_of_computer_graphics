#ifndef TOOLS_H
#define TOOLS_H

#include <QImage>
#include <QPoint>

double
euclideanSquare(QPoint start, QPoint end);

double
euclidean(QPoint start, QPoint end);

void
drawPixel(QImage& canvas, int x, int y, uint red, uint green, uint blue);

void
drawLine(QImage& canvas,
         int x0,
         int y0,
         int x1,
         int y1,
         uint red,
         uint green,
         uint blue);

void
drawCircle(QImage& canvas,
           int x0,
           int y0,
           double radius,
           uint red,
           uint green,
           uint blue);

void
drawEllipse(QImage& canvas,
            int x0,
            int y0,
            double a,
            double b,
            uint red,
            uint green,
            uint blue);

void
drawBezier(QImage& canvas, QPoint p1, QPoint p2, QPoint p3, QPoint p4, int N);

#endif // TOOLS_H
