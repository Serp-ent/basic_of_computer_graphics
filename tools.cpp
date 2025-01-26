
#include <QImage>
#include <QPoint>
#include <QRgb>
#include <cmath>
#include <qdebug.h>
#include <stack>

#include "movablepoint.h"
#include <algorithm>
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

double
bezier_3(double t, double p0, double p1, double p2, double p3)
{
  double term0 = std::pow(1 - t, 3) * p0;
  double term1 = 3 * std::pow(1 - t, 2) * t * p1;
  double term2 = 3 * (1 - t) * std::pow(t, 2) * p2;
  double term3 = std::pow(t, 3) * p3;

  return term0 + term1 + term2 + term3;
}

double
horner(double x, std::vector<double> a)
{
  double v = a[0];
  for (int k = 1; k < a.size(); ++k) {
    v = a[k] + x * v;
  }

  return v;
}

void
drawBezierCurve(QImage& canvas,
                QPoint p1,
                QPoint p2,
                QPoint p3,
                QPoint p4,
                int N)
{
  for (int i = 0; i < N; ++i) {
    const double t = i / (double)N;
    const double t_next = (i + 1) / (double)N;

    int x1 = bezier_3(t, p1.x(), p2.x(), p3.x(), p4.x());
    int y1 = bezier_3(t, p1.y(), p2.y(), p3.y(), p4.y());
    int x2 = bezier_3(t_next, p1.x(), p2.x(), p3.x(), p4.x());
    int y2 = bezier_3(t_next, p1.y(), p2.y(), p3.y(), p4.y());

    drawLine(canvas, x1, y1, x2, y2, 255, 255, 255);
  }
}

void
drawBezier(QImage& canvas, std::vector<MovablePoint>& points)
{
  // redraw
  canvas.fill(Qt::black); // Clear canvas with black
  for (const auto& point : points) {
    point.draw(canvas);
  }

  if (points.size() >= 4) {
    constexpr int firstCurveCount = 4;

    // Draw control point lines for the first curve
    drawLine(canvas,
             points.at(0).x(),
             points.at(0).y(),
             points.at(1).x(),
             points.at(1).y(),
             0,
             128,
             128);
    drawLine(canvas,
             points.at(2).x(),
             points.at(2).y(),
             points.at(3).x(),
             points.at(3).y(),
             0,
             128,
             128);

    drawBezierCurve(
      canvas, points.at(0), points.at(1), points.at(2), points.at(3), 20);

    // 3 for every curve needed because we already have start one
    int additionalCurves = (points.size() - firstCurveCount) / 3;
    for (int i = 0; i < additionalCurves; ++i) {
      int base = i * 3 + 3; // start after first curve

      drawLine(canvas,
               points.at(base).x(),
               points.at(base).y(),
               points.at(base + 1).x(),
               points.at(base + 1).y(),
               0,
               128,
               128); // Example: gray line
      drawLine(canvas,
               points.at(base + 2).x(),
               points.at(base + 2).y(),
               points.at(base + 3).x(),
               points.at(base + 3).y(),
               0,
               128,
               128); // Example: gray line

      drawBezierCurve(canvas,
                      points.at(base),
                      points.at(base + 1),
                      points.at(base + 2),
                      points.at(base + 3),
                      20);
    }
  }
}

void
flood_fill(QImage& canvas, QPoint p, QColor from, QColor to)
{
  std::stack<QPoint> points;
  if (canvas.pixelColor(p) != from)
    return;
  points.push(p);

  while (!points.empty()) {
    QPoint v = points.top();
    points.pop();

    if (canvas.pixelColor(v) == from) {
      int w = v.x();
      int e = v.x();

      while (w > 0 && canvas.pixelColor(w, v.y()) == from)
        --w;
      while (e < canvas.width() && canvas.pixelColor(e, v.y()) == from)
        ++e;

      for (int i = w + 1; i < e; ++i) // draw horizontal line
        drawPixel(canvas, i, v.y(), to.red(), to.green(), to.blue());

      for (int i = w + 1; i < e; ++i) {
        // add to up
        if (v.y() + 1 < canvas.height() &&
            canvas.pixelColor(i, v.y() + 1) == from)
          points.emplace(i, v.y() + 1);
        // add to down
        if (v.y() > 0 && canvas.pixelColor(i, v.y() - 1) == from)
          points.emplace(i, v.y() - 1);
      }
    }
  }

  qDebug() << "Filled using flood_fill\n";
}

void
scan_line(QImage& canvas, const std::vector<QPoint>& points)
{
  if (points.size() < 3)
    return;

  int minY = points.at(0).y();
  int maxY = minY;
  for (const QPoint& p : points) {
    minY = std::min(minY, p.y());
    maxY = std::max(maxY, p.y());
  }

  // wszystkie punkty sa na tej samym y obliczamy tylko x
  for (int y = minY; y <= maxY; ++y) {
    std::vector<int> przeciecia;

    // znajdz punkty przeciecia z prosta pozioma y
    for (int i = 0; i < points.size(); ++i) {
      const QPoint& curr = points.at(i);
      const QPoint& next = points.at((i + 1) % points.size());

      int lower_bound = std::min(curr.y(), next.y());
      int upper_bound = std::max(curr.y(), next.y());
      if (y > lower_bound && y <= upper_bound) {
        // TODO: oblicz x z rownania
        int x =
          (((y - curr.y()) * (next.x() - curr.x())) / (next.y() - curr.y())) +
          curr.x();
        przeciecia.push_back(x);
      }
    }

    // sortuj tablice przeciec
    std::sort(przeciecia.begin(), przeciecia.end());

    // zamaluj wszystkie poziome segmenty przeciecia[2n] do przeciecia[2n + 1]
    for (int i = 0; i < przeciecia.size(); i += 2) {
      if (i + 1 < przeciecia.size()) {
        int start_x = przeciecia.at(i);
        int end_x = przeciecia.at(i + 1);

        for (int x = start_x; x < end_x; ++x) {
          drawPixel(canvas, x, y, 255, 255, 255);
        }
      }
    }
  }
}

QRgb
normal_mode(QRgb f, QRgb b)
{
  return b;
}

QRgb
multiply_mode(QRgb foreground, QRgb background)
{
  uchar foregroundRed = qRed(foreground);
  uchar foregroundGreen = qGreen(foreground);
  uchar foregroundBlue = qBlue(foreground);

  uchar backgroundRed = qRed(background);
  uchar backgroundGreen = qGreen(background);
  uchar backgroundBlue = qBlue(background);

  uchar red = (foregroundRed * backgroundRed) >> 8;
  uchar green = (foregroundGreen * backgroundGreen) >> 8;
  uchar blue = (foregroundBlue * backgroundBlue) >> 8;

  // Combine the results back into a QRgb value
  return qRgb(red, green, blue);
}

uchar
blend_function(uchar a, uchar b, float alpha)
{
  // b is foreground
  // a is background
  return alpha * b + (1 - alpha) * a;
}
QRgb
screen_mode(QRgb foreground, QRgb background)
{
  uchar foregroundRed = qRed(foreground);
  uchar foregroundGreen = qGreen(foreground);
  uchar foregroundBlue = qBlue(foreground);

  uchar backgroundRed = qRed(background);
  uchar backgroundGreen = qGreen(background);
  uchar backgroundBlue = qBlue(background);

  uchar red = 255 - ((255 - foregroundRed) * (255 - backgroundRed)) >> 8;
  uchar green = 255 - ((255 - foregroundGreen) * (255 - backgroundGreen)) >> 8;
  uchar blue = 255 - ((255 - foregroundBlue) * (255 - backgroundBlue)) >> 8;

  return qRgb(red, green, blue);
}

QRgb
overlay_mode(QRgb foreground, QRgb background)
{
  uchar foregroundRed = qRed(foreground);
  uchar foregroundGreen = qGreen(foreground);
  uchar foregroundBlue = qBlue(foreground);

  uchar backgroundRed = qRed(background);
  uchar backgroundGreen = qGreen(background);
  uchar backgroundBlue = qBlue(background);

  uchar red =
    (foregroundRed < 128)
      ? (2 * foregroundRed * backgroundRed) / 255
      : 255 - (2 * (255 - foregroundRed) * (255 - backgroundRed)) / 255;
  uchar green =
    (foregroundGreen < 128)
      ? (2 * foregroundGreen * backgroundGreen) / 255
      : 255 - (2 * (255 - foregroundGreen) * (255 - backgroundGreen)) / 255;
  uchar blue =
    (foregroundBlue < 128)
      ? (2 * foregroundBlue * backgroundBlue) / 255
      : 255 - (2 * (255 - foregroundBlue) * (255 - backgroundBlue)) / 255;

  return qRgb(red, green, blue);
}

QRgb
darken_mode(QRgb foreground, QRgb background)
{
  uchar foregroundRed = qRed(foreground);
  uchar foregroundGreen = qGreen(foreground);
  uchar foregroundBlue = qBlue(foreground);

  uchar backgroundRed = qRed(background);
  uchar backgroundGreen = qGreen(background);
  uchar backgroundBlue = qBlue(background);

  uchar red = qMin(foregroundRed, backgroundRed);
  uchar green = qMin(foregroundGreen, backgroundGreen);
  uchar blue = qMin(foregroundBlue, backgroundBlue);

  return qRgb(red, green, blue);
}

QRgb
lighten_mode(QRgb foreground, QRgb background)
{
  uchar foregroundRed = qRed(foreground);
  uchar foregroundGreen = qGreen(foreground);
  uchar foregroundBlue = qBlue(foreground);

  uchar backgroundRed = qRed(background);
  uchar backgroundGreen = qGreen(background);
  uchar backgroundBlue = qBlue(background);

  uchar red = qMax(foregroundRed, backgroundRed);
  uchar green = qMax(foregroundGreen, backgroundGreen);
  uchar blue = qMax(foregroundBlue, backgroundBlue);

  return qRgb(red, green, blue);
}

typedef QRgb (*BlendModeFunc)(QRgb foreground, QRgb background);
BlendModeFunc blendModes[] = {
  normal_mode,   // 0
  multiply_mode, // 1
  screen_mode,   // 2
  overlay_mode,  // 3
  darken_mode,   // 4
  lighten_mode,  // 5
};

void
blend(const QImage& background,
      const QImage& foreground,
      float alpha,
      int mode,
      QImage& out)
{
  if (mode < 0 || mode >= sizeof(blendModes) / sizeof(BlendModeFunc)) {
    throw new std::runtime_error("Blend mode out of range");
  }

  BlendModeFunc modeFunc = blendModes[mode];
  // iterate over all pixels
  for (int y = 0; y < background.height(); ++y) {
    for (int x = 0; x < background.width(); ++x) {
      QRgb c = modeFunc(background.pixel(x, y), foreground.pixel(x, y));
      // TODO: handle multiple blend modes if (mode == 0) etc...
      //   uchar c = f[mode](background[i], foreground[i]);

      // do it for red
      QRgb bpixel = background.pixel(x, y);
      uchar r = blend_function(qRed(c), qRed(bpixel), alpha);
      uchar g = blend_function(qGreen(c), qGreen(bpixel), alpha);
      uchar b = blend_function(qBlue(c), qBlue(bpixel), alpha);
      // do it for green

      //   out[i] = alpha * c + (1 - a) background[i];
      QRgb result = qRgb(r, g, b);
      out.setPixel(x, y, result);
    }
  }
}

void
multiply3x1(float A[][3], float B[], float C[])
{
  // Initialize result matrix C to 0
  for (int i = 0; i < 3; i++) {
    C[i] = 0;
  }

  // Perform multiplication
  for (int i = 0; i < 3; i++) {
    for (int k = 0; k < 3; k++) {
      C[i] += A[i][k] * B[k];
    }
  }
}

void
multiply3x3(float A[][3], float B[][3], float C[][3])
{
  // Initialize result matrix C to 0
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      C[i][j] = 0;
    }
  }

  // Perform multiplication
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 3; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

QColor
bilinear_interpolation_color(float a,
                             float b,
                             const QColor& p1,
                             const QColor& p2,
                             const QColor& p3,
                             const QColor& p4)
{
  // Interpolate the red channel
  float red_top =
    (1 - a) * p1.red() + a * p2.red(); // top row red channel interpolation
  float red_bottom =
    (1 - a) * p3.red() + a * p4.red(); // bottom row red channel interpolation
  float red_final = (1 - b) * red_top + b * red_bottom;

  // Interpolate the green channel
  float green_top = (1 - a) * p1.green() +
                    a * p2.green(); // top row green channel interpolation
  float green_bottom = (1 - a) * p3.green() +
                       a * p4.green(); // bottom row green channel interpolation
  float green_final = (1 - b) * green_top + b * green_bottom;

  // Interpolate the blue channel
  float blue_top =
    (1 - a) * p1.blue() + a * p2.blue(); // top row blue channel interpolation
  float blue_bottom = (1 - a) * p3.blue() +
                      a * p4.blue(); // bottom row blue channel interpolation
  float blue_final = (1 - b) * blue_top + b * blue_bottom;

  // Clamp the values between 0 and 255, and return the resulting color
  return QColor(static_cast<int>(std::min(std::max(red_final, 0.0f), 255.0f)),
                static_cast<int>(std::min(std::max(green_final, 0.0f), 255.0f)),
                static_cast<int>(std::min(std::max(blue_final, 0.0f), 255.0f)));
}

uchar
bilinear_interpolation_single(float a,
                              float b,
                              uchar p1,
                              uchar p2,
                              uchar p3,
                              uchar p4)
{
  // Interpolate along x-axis (horizontal interpolation)
  float top = (1 - a) * p1 + a * p2;    // top row interpolation
  float bottom = (1 - a) * p3 + a * p4; // bottom row interpolation

  // Interpolate along y-axis (vertical interpolation)
  float final_value = (1 - b) * top + b * bottom;

  // Clamp the value between 0 and 255
  return static_cast<uchar>(std::min(std::max(final_value, 0.0f), 255.0f));
}

void
applyTexturing(QImage& canvas,
               const QImage& img,
               const std::vector<QPoint>& leftTrianglePoints,
               const std::vector<QPoint>& rightTrianglePoints,
               bool useBilinearInterpolation)
{
  if (leftTrianglePoints.size() < 3 || rightTrianglePoints.size() < 3)
    return;

  // Define the triangle vertices
  QPoint A = leftTrianglePoints[0];
  QPoint B = leftTrianglePoints[1];
  QPoint C = leftTrianglePoints[2];

  QPoint At = rightTrianglePoints[0];
  QPoint Bt = rightTrianglePoints[1];
  QPoint Ct = rightTrianglePoints[2];

  // Compute bounding box for the textured triangle
  int minX = std::min({ At.x(), Bt.x(), Ct.x() });
  int maxX = std::max({ At.x(), Bt.x(), Ct.x() });
  int minY = std::min({ At.y(), Bt.y(), Ct.y() });
  int maxY = std::max({ At.y(), Bt.y(), Ct.y() });

  for (int x = minX; x <= maxX; ++x) {
    for (int y = minY; y <= maxY; ++y) {
      QPoint P(x, y);

      // Calculate barycentric coordinates for P
      float u, v, w;
      if (!calculateBarycentric(At, Bt, Ct, P, u, v, w))
        continue; // Skip if P is outside the triangle

      // Skip if P is outside the triangle even after barycentric calculation
      if (u < 0 || v < 0 || w < 0)
        continue;

      // Map barycentric coordinates to the original triangle
      float tx = u * A.x() + v * B.x() + w * C.x();
      float ty = u * A.y() + v * B.y() + w * C.y();

      QColor color;

      if (useBilinearInterpolation) {
        // Perform bilinear interpolation
        int x1 = std::floor(tx);
        int x2 = std::ceil(tx);
        int y1 = std::floor(ty);
        int y2 = std::ceil(ty);

        // Clamp texture coordinates to the image bounds
        x1 = std::clamp(x1, 0, img.width() - 1);
        x2 = std::clamp(x2, 0, img.width() - 1);
        y1 = std::clamp(y1, 0, img.height() - 1);
        y2 = std::clamp(y2, 0, img.height() - 1);

        float a = tx - x1; // Horizontal weight
        float b = ty - y1; // Vertical weight

        QColor p1 = img.pixelColor(x1, y1);
        QColor p2 = img.pixelColor(x2, y1);
        QColor p3 = img.pixelColor(x1, y2);
        QColor p4 = img.pixelColor(x2, y2);

        color = bilinear_interpolation_color(a, b, p1, p2, p3, p4);
      } else {
        // Use nearest neighbor sampling
        int texX = std::round(tx);
        int texY = std::round(ty);
        color = img.pixelColor(std::clamp(texX, 0, img.width() - 1),
                               std::clamp(texY, 0, img.height() - 1));
      }

      // Set the pixel on the canvas
      canvas.setPixelColor(x - img.width(), y, color);
    }
  }
}

bool
calculateBarycentric(const QPoint& A,
                     const QPoint& B,
                     const QPoint& C,
                     const QPoint& P,
                     float& u,
                     float& v,
                     float& w)
{
  float denominator =
    (B.y() - C.y()) * (A.x() - C.x()) + (C.x() - B.x()) * (A.y() - C.y());
  if (denominator == 0.0f)
    return false; // Degenerate triangle

  u = ((B.y() - C.y()) * (P.x() - C.x()) + (C.x() - B.x()) * (P.y() - C.y())) /
      denominator;
  v = ((C.y() - A.y()) * (P.x() - C.x()) + (A.x() - C.x()) * (P.y() - C.y())) /
      denominator;
  w = 1.0f - u - v;

  return (u >= 0 && v >= 0 && w >= 0);
}
