#ifndef TOOLS_H
#define TOOLS_H

#include "movablepoint.h"
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
drawBezierCurve(QImage& canvas,
                QPoint p1,
                QPoint p2,
                QPoint p3,
                QPoint p4,
                int N);
void
drawBezier(QImage& canvas, std::vector<MovablePoint>& points);

double
horner(double x, std::vector<double> a);
double
bezier_3(double t, double x);

void
flood_fill(QImage& canvas, QPoint p, QColor from, QColor to);

void
scan_line(QImage& canvas, const std::vector<QPoint>& points);

void
blend(const QImage& background,
      const QImage& foreground,
      float alpha,
      int mode,
      QImage& result);

#endif // TOOLS_H
