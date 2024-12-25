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

// Function to multiply two 3x3 matrices
void
multiply3x3(float A[][3], float B[][3], float C[][3]);

// Function to multiply a 3x3 matrix with a 3x1 matrix
void
multiply3x1(float A[][3], float B[], float C[]);

uchar
bilinear_interpolation_single(float a,
                              float b,
                              uchar p1,
                              uchar p2,
                              uchar p3,
                              uchar p4);
QColor
bilinear_interpolation_color(float a,
                             float b,
                             const QColor& p1,
                             const QColor& p2,
                             const QColor& p3,
                             const QColor& p4);

#endif // TOOLS_H
