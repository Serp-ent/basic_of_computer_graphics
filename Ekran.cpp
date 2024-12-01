#include "Ekran.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

#include "movablepoint.h"
#include <algorithm>
#include <tools.h>

void
Ekran::mousePressEvent(QMouseEvent* event)
{
  mousePresssed = true;
  pressStart = event->pos();

  if (tool != 4)
    return;

  if (event->button() == Qt::LeftButton) {
    QPoint p = event->pos();
    bool pointNearby = false;
    // check if there is point nearby
    for (auto& point : points) {
      if (euclideanSquare(p, point) <=
          MovablePoint::POINT_RADIUS * MovablePoint::POINT_RADIUS) {
        pointNearby = true;
        selectedPoint = &point;
      }
    }

    if (pointNearby) {
      qDebug() << "Point selected for moving";

      // if so catch him and change its border color to green (active)
      // TODO: currently do nothing
    } else {
      points.push_back({ p.x(), p.y() });

      // TODO: there is redraw fix that
      canvas.fill(Qt::black); // Clear canvas with black
      for (const auto& point : points) {
        point.draw(this->canvas);
      }

      if (points.size() >= 4) {
        constexpr int firstCurveCount = 4;

        // Draw control point lines for the first curve
        drawLine(this->canvas,
                 points.at(0).x(),
                 points.at(0).y(),
                 points.at(1).x(),
                 points.at(1).y(),
                 0,
                 128,
                 128);
        drawLine(this->canvas,
                 points.at(2).x(),
                 points.at(2).y(),
                 points.at(3).x(),
                 points.at(3).y(),
                 0,
                 128,
                 128);

        drawBezier(this->canvas,
                   points.at(0),
                   points.at(1),
                   points.at(2),
                   points.at(3),
                   20);

        // 3 for every curve needed because we already have start one
        int additionalCurves = (points.size() - firstCurveCount) / 3;
        for (int i = 0; i < additionalCurves; ++i) {
          int base = i * 3 + 3; // start after first curve

          drawLine(this->canvas,
                   points.at(base).x(),
                   points.at(base).y(),
                   points.at(base + 1).x(),
                   points.at(base + 1).y(),
                   0,
                   128,
                   128); // Example: gray line
          drawLine(this->canvas,
                   points.at(base + 2).x(),
                   points.at(base + 2).y(),
                   points.at(base + 3).x(),
                   points.at(base + 3).y(),
                   0,
                   128,
                   128); // Example: gray line

          drawBezier(this->canvas,
                     points.at(base),
                     points.at(base + 1),
                     points.at(base + 2),
                     points.at(base + 3),
                     20);
        }
      }

      update();
    }
  } else if (event->button() == Qt::RightButton) {
    QPoint p = event->pos();

    auto it =
      std::find_if(points.begin(), points.end(), [&](const QPoint& current) {
        return euclideanSquare(p, current) <=
               MovablePoint::POINT_RADIUS * MovablePoint::POINT_RADIUS;
      });

    if (it != points.end())
      points.erase(it);

    // redraw
    canvas.fill(Qt::black); // Clear canvas with black
    for (const auto& point : points) {
      point.draw(this->canvas);
    }

    if (points.size() >= 4) {
      constexpr int firstCurveCount = 4;

      // Draw control point lines for the first curve
      drawLine(this->canvas,
               points.at(0).x(),
               points.at(0).y(),
               points.at(1).x(),
               points.at(1).y(),
               0,
               128,
               128);
      drawLine(this->canvas,
               points.at(2).x(),
               points.at(2).y(),
               points.at(3).x(),
               points.at(3).y(),
               0,
               128,
               128);

      drawBezier(this->canvas,
                 points.at(0),
                 points.at(1),
                 points.at(2),
                 points.at(3),
                 20);

      // 3 for every curve needed because we already have start one
      int additionalCurves = (points.size() - firstCurveCount) / 3;
      for (int i = 0; i < additionalCurves; ++i) {
        int base = i * 3 + 3; // start after first curve

        drawLine(this->canvas,
                 points.at(base).x(),
                 points.at(base).y(),
                 points.at(base + 1).x(),
                 points.at(base + 1).y(),
                 0,
                 128,
                 128); // Example: gray line
        drawLine(this->canvas,
                 points.at(base + 2).x(),
                 points.at(base + 2).y(),
                 points.at(base + 3).x(),
                 points.at(base + 3).y(),
                 0,
                 128,
                 128); // Example: gray line

        drawBezier(this->canvas,
                   points.at(base),
                   points.at(base + 1),
                   points.at(base + 2),
                   points.at(base + 3),
                   20);
      }
    }
    update();
  }

  canvasClone = canvas.copy();
}

void
Ekran::mouseReleaseEvent(QMouseEvent* event)
{
  mousePresssed = false;
  if (tool == 4 && (event->button() == Qt::LeftButton)) {
    selectedPoint = nullptr;
  }
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

  // there should be draw points;
  for (const auto& p : points) {
    p.draw(this->canvas);
  }
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
      drawLine(this->canvas,
               pressStart.x(),
               pressStart.y(),
               p.x(),
               p.y(),
               255,
               255,
               255);
      break;
    case 2: {
      int a = (pressStart.x() - p.x());
      int b = (pressStart.y() - p.y());
      double radius = sqrt((a * a) + (b * b));
      drawCircle(
        this->canvas, pressStart.x(), pressStart.y(), radius, 255, 255, 255);
    } break;
    case 3: {
      int a = (pressStart.x() - p.x());
      int b = (pressStart.y() - p.y());
      drawEllipse(
        this->canvas, pressStart.x(), pressStart.y(), a, b, 255, 255, 255);
    } break;
    case 4: {
      if (selectedPoint) {
        *selectedPoint = p;

        // TODO: this should redraw canvas without points
        canvas.fill(Qt::black);
        for (const auto& point : points) {
          bool currentlyMoved = (selectedPoint == &point);
          point.draw(this->canvas, currentlyMoved);
        }
      }

      if (points.size() >= 4) {
        constexpr int firstCurveCount = 4;

        // Draw control point lines for the first curve
        drawLine(this->canvas,
                 points.at(0).x(),
                 points.at(0).y(),
                 points.at(1).x(),
                 points.at(1).y(),
                 0,
                 128,
                 128);
        drawLine(this->canvas,
                 points.at(2).x(),
                 points.at(2).y(),
                 points.at(3).x(),
                 points.at(3).y(),
                 0,
                 128,
                 128);

        drawBezier(this->canvas,
                   points.at(0),
                   points.at(1),
                   points.at(2),
                   points.at(3),
                   20);

        // 3 for every curve needed because we already have start one
        int additionalCurves = (points.size() - firstCurveCount) / 3;
        for (int i = 0; i < additionalCurves; ++i) {
          int base = i * 3 + 3; // start after first curve

          drawLine(this->canvas,
                   points.at(base).x(),
                   points.at(base).y(),
                   points.at(base + 1).x(),
                   points.at(base + 1).y(),
                   0,
                   128,
                   128); // Example: gray line
          drawLine(this->canvas,
                   points.at(base + 2).x(),
                   points.at(base + 2).y(),
                   points.at(base + 3).x(),
                   points.at(base + 3).y(),
                   0,
                   128,
                   128); // Example: gray line

          drawBezier(this->canvas,
                     points.at(base),
                     points.at(base + 1),
                     points.at(base + 2),
                     points.at(base + 3),
                     20);
        }
      }
    }
    default:
      break;
  }

  update();
}
