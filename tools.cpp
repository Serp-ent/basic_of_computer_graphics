
#include <QImage>
#include <QPoint>
#include <cmath>

#include <tools.h>

double
euclideanSquare(QPoint start, QPoint end)
{
  int x = start.x() - end.x();
  int y = start.y() - end.y();
  return (x * x + y * y);
}

double
euclidean(QPoint start, QPoint end)
{
  return std::sqrt(euclideanSquare(start, end));
}

void
drawPixel(QImage& canvas, int x, int y, uint red, uint green, uint blue)
{
  if (x < 0)
    return;
  if (x >= canvas.width())
    return;
  if (y < 0)
    return;
  if (y >= canvas.height())
    return;

  uchar* p = canvas.scanLine(y);
  p[4 * x + 0] = blue;  // blue
  p[4 * x + 1] = green; // reen
  p[4 * x + 2] = red;   // red
}

void
drawLine(QImage& canvas,
         int x0,
         int y0,
         int x1,
         int y1,
         uint red,
         uint green,
         uint blue)
{

  double m = (double)(y1 - y0) / (x1 - x0);
  if (std::abs(m) <= 1) {
    if (x0 > x1) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }

    for (int x = x0; x <= x1; ++x) {
      int y = m * (x - x0) + y0;
      drawPixel(canvas, x, (int)std::floor(y + 0.5), red, green, blue);
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
      drawPixel(canvas, (int)std::floor(x + 0.5), y, red, green, blue);
    }
  }
}

void
drawCircle(QImage& canvas,
           int x0,
           int y0,
           double radius,
           uint red,
           uint green,
           uint blue)
{
  int x_end = radius / std::sqrt(2);
  for (int x = 0; x <= x_end; ++x) {
    int y = std::sqrt(radius * radius - x * x);

    drawPixel(canvas, x0 + x, y0 + y, red, green, blue);
    drawPixel(canvas, x0 + y, y0 + x, red, green, blue);

    drawPixel(canvas, x0 + y, y0 - x, red, green, blue);
    drawPixel(canvas, x0 + x, y0 - y, red, green, blue);

    drawPixel(canvas, x0 - x, y0 - y, red, green, blue);
    drawPixel(canvas, x0 - y, y0 - x, red, green, blue);

    drawPixel(canvas, x0 - y, y0 + x, red, green, blue);
    drawPixel(canvas, x0 - x, y0 + y, red, green, blue);
  }
}

void
drawEllipse(QImage& canvas,
            int x0,
            int y0,
            double a,
            double b,
            uint red,
            uint green,
            uint blue)
{
  constexpr int n = 10;
  constexpr double step = 2 * M_PI / n;

  std::vector<std::pair<int, int>> points;

  double alpha = 0;
  while (alpha < 2 * M_PI) {
    double x = a * std::cos(alpha);
    double y = b * std::sin(alpha);

    points.push_back({ x0 + int(x), y0 + int(y) });

    alpha += step;
  }

  for (int i = 0; i < points.size() - 1; ++i) {
    drawLine(canvas,
             points[i].first,
             points[i].second,
             points[i + 1].first,
             points[i + 1].second,
             red,
             green,
             blue);
  }

  drawLine(canvas,
           points.front().first,
           points.front().second,
           points.back().first,
           points.back().second,
           red,
           green,
           blue);
}

void
drawBezier(QImage& canvas, QPoint p1, QPoint p2, QPoint p3, QPoint p4, int N)
{
}
