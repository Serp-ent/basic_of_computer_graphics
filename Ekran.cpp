#include "Ekran.h"
#include <QDebug>
#include <QPainter>
#include <cmath>
#include <iostream>

void
drawPixel(QImage* canvas, int x, int y, uint red, uint green, uint blue)
{
  if (x < 0)
    return;
  if (x >= canvas->width())
    return;
  if (y < 0)
    return;
  if (y >= canvas->height())
    return;

  uchar* p = canvas->scanLine(y);
  p[4 * x + 0] = blue;  // blue
  p[4 * x + 1] = green; // reen
  p[4 * x + 2] = red;   // red
}

void
Ekran::drawLine(int x0, int y0, int x1, int y1, uint red, uint green, uint blue)
{

  double m = (double)(y1 - y0) / (x1 - x0);
  if (std::abs(m) <= 1) {
    if (x0 > x1) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; ++x) {
      int y = m * (x - x0) + y0;
      drawPixel(&this->canvas, x, (int)std::floor(y + 0.5), red, green, blue);
    }
  } else {
    if (y0 > y1) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }

    // oblicz ponownie wsp kierunkowy dla odwroconych osi
    m = (double)(x1 - x0) / (y1 - y0);
    for (int y = y0; y <= y1; ++y) {
      int x = m * (y - y0) + x0;
      drawPixel(&this->canvas, (int)std::floor(x + 0.5), y, red, green, blue);
    }
  }

  update();
}

void
Ekran::drawCircle(int x0,
                  int y0,
                  double radius,
                  uint red,
                  uint green,
                  uint blue)
{
  int x_end = radius / std::sqrt(2);
  for (int x = 0; x <= x_end; ++x) {
    int y = std::sqrt(radius * radius - x * x);

    drawPixel(&this->canvas, x0 + x, y0 + y, red, green, blue);
    drawPixel(&this->canvas, x0 + y, y0 + x, red, green, blue);

    drawPixel(&this->canvas, x0 + y, y0 - x, red, green, blue);
    drawPixel(&this->canvas, x0 + x, y0 - y, red, green, blue);

    drawPixel(&this->canvas, x0 - x, y0 - y, red, green, blue);
    drawPixel(&this->canvas, x0 - y, y0 - x, red, green, blue);

    drawPixel(&this->canvas, x0 - y, y0 + x, red, green, blue);
    drawPixel(&this->canvas, x0 - x, y0 + y, red, green, blue);
  }
}

void
Ekran::mousePressEvent(QMouseEvent* event)
{
  mousePresssed = true;
  pressStart = event->pos();
  canvasClone = canvas.copy();
}

void
Ekran::mouseReleaseEvent(QMouseEvent* event)
{
  mousePresssed = false;
}

Ekran::Ekran(QWidget* parent)
  : QWidget{ parent }
{
  setMouseTracking(true);

  canvas = QImage(this->width(), this->height(), QImage::Format_RGB32);
  canvas.fill(0);
}

void
Ekran::paintEvent(QPaintEvent* event)
{
  QPainter p(this);
  p.fillRect(0, 0, width(), height(), Qt::black);
  p.drawImage(0, 0, canvas);
}

void
Ekran::mouseMoveEvent(QMouseEvent* event)
{
  if (!mousePresssed) {
    return;
  }
  canvas = canvasClone;

  QPoint p = event->pos();

  switch (tool) {
    case 1:
      drawLine(pressStart.x(), pressStart.y(), p.x(), p.y(), 255, 255, 255);
      break;
    case 2: {
      // double radius = (pressStart - p).manhattanLength();
      int a = (pressStart.x() - p.x());
      int b = (pressStart.y() - p.y());
      double radius = sqrt((a * a) + (b * b));
      drawCircle(pressStart.x(), pressStart.y(), radius, 255, 255, 255);
    } break;
    default:
      break;
  }

  update();
}
