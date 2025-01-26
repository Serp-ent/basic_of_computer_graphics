#include "Ekran.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

#include "layer.h"
#include "movablepoint.h"
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QImage>
#include <QLabel>
#include <algorithm>
#include <tools.h>

void
Ekran::mousePressEvent(QMouseEvent* event)
{
  mousePresssed = true;
  pressStart = event->pos();

  if (event->button() == Qt::LeftButton) {
    // Check if any of the predefined points is near the mouse click
    bool pointNearby = false;
    for (auto& point : leftTrianglePoints) {
      if (euclideanSquare(pressStart, point) <=
          MovablePoint::POINT_RADIUS * MovablePoint::POINT_RADIUS) {
        pointNearby = true;
        selectedPoint = &point;
        return;
      }
    }
    for (auto& point : rightTrianglePoints) {
      if (euclideanSquare(pressStart, point) <=
          MovablePoint::POINT_RADIUS * MovablePoint::POINT_RADIUS) {
        pointNearby = true;
        selectedPoint = &point;
        return;
      }
    }
  }

  // if (event->button() == Qt::MiddleButton) {
  //   flood_fill(this->canvas, event->pos(), Qt::black, Qt::white);
  //   update();
  //   return;
  // }

  // if (tool < 4) {
  //   canvasClone = canvas.copy();
  //   return;
  // }

  /* if (tool == 4) {
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

         drawBezier(this->canvas, points);

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

       drawBezier(this->canvas, points);
       update();
     }

     canvasClone = canvas.copy();
   } else*/

  // if (tool == 5) {
  //   if (event->button() == Qt::LeftButton) {

  //     qDebug() << "point added\n";
  //     points.push_back(event->pos());
  //     drawPixel(
  //       this->canvas, event->pos().x(), event->pos().y(), 255, 255, 255);
  //   } else if (event->button() == Qt::RightButton) {
  //     qDebug() << "scan_line\n";
  //     scan_line(this->canvas, points);
  //     points.clear();
  //   }

  //   update();
  //   canvasClone = canvas.copy();
  // }
}

void
Ekran::mouseReleaseEvent(QMouseEvent* event)
{
  mousePresssed = false;
  if (event->button() == Qt::LeftButton) {
    selectedPoint = nullptr;
  }
}

Ekran::Ekran(QWidget* parent)
  : QWidget{ parent }
{
  setMouseTracking(true);
  this->resize(1000, 600);

  // Initialize the canvas
  canvas = QImage(width() - 200, height() - 50, QImage::Format_RGB32);
  canvas.fill(Qt::lightGray);

  // Load the image for texturing
  img.load(":/images/wilkizajac.jpg");
  if (img.isNull()) {
    throw new std::runtime_error{ "Cannot open file 'room.jpg' in exercise 6" };
  }

  // Set up the checkbox for bilinear interpolation
  bilinearCheckBox = new QCheckBox("Enable Bilinear Interpolation", this);
  bilinearCheckBox->setGeometry(10, height() - 40, 200, 30);
  connect(bilinearCheckBox,
          &QCheckBox::toggled,
          this,
          &Ekran::onBilinearInterpolationToggled);

  // Set up the layout
  QVBoxLayout* layout = new QVBoxLayout;

  // Create a horizontal layout to hold the images side by side
  QHBoxLayout* imageLayout = new QHBoxLayout;
  imageLayout->setSpacing(0);

  // Create a container for the left image (original image)
  QWidget* leftImageWidget = new QWidget(this);
  leftImageWidget->setGeometry(10, 10, 400, 400);
  imageLayout->addWidget(leftImageWidget);

  // Create a container for the right image (textured output)
  QWidget* rightImageWidget = new QWidget(this);
  rightImageWidget->setGeometry(420, 10, 400, 400);
  imageLayout->addWidget(rightImageWidget);

  layout->addLayout(imageLayout);
  layout->addWidget(bilinearCheckBox);

  // Set the layout for the main widget
  setLayout(layout);

  // Initialize triangle points
  leftTrianglePoints.push_back(QPoint(100, 100));
  leftTrianglePoints.push_back(QPoint(200, 300));
  leftTrianglePoints.push_back(QPoint(300, 100));

  rightTrianglePoints.push_back(QPoint(500, 100));
  rightTrianglePoints.push_back(QPoint(600, 300));
  rightTrianglePoints.push_back(QPoint(700, 100));
}

void
copyMatrix(const float source[3][3], float destination[3][3])
{
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      destination[i][j] = source[i][j];
    }
  }
}

void
Ekran::paintEvent(QPaintEvent* event)
{
  QPainter p(this);
  p.fillRect(0, 0, width(), height(), Qt::black);

  // Draw the left image (original)
  QRect leftImageRect(10, 10, 400, 400);
  p.drawImage(leftImageRect, img);

  // Draw the points as red ellipses, centered on the points
  // Draw the triangle using the points in 'points'
  if (leftTrianglePoints.size() == 3) {
    for (const auto& point : leftTrianglePoints) {
      p.setBrush(Qt::red);
      constexpr int radius = MovablePoint::POINT_RADIUS;
      p.drawEllipse(point - QPoint(radius / 2, radius / 2), radius, radius);
    }

    // Convert std::vector<QPoint> to QPolygon
    QPolygon triangle;
    for (const auto& point : leftTrianglePoints) {
      triangle << point; // Add points to QPolygon
    }

    p.setBrush(Qt::NoBrush);    // No fill for the triangle
    p.setPen(QPen(Qt::red, 2)); // Red outline for the triangle
    p.drawPolygon(triangle);    // Draw the triangle
  }

  canvas.fill(Qt::black); // Clear the canvas to avoid leftover pixels
  applyTexturing(canvas,
                 img,
                 leftTrianglePoints,
                 rightTrianglePoints,
                 bilinearCheckBox->isChecked());

  // Draw the right image (textured output using barycentric coordinates)
  QRect rightImageRect(420, 10, 400, 400);
  p.drawImage(rightImageRect, canvas);
  if (rightTrianglePoints.size() == 3) {
    for (const auto& point : rightTrianglePoints) {
      p.setBrush(Qt::red);
      constexpr int radius = MovablePoint::POINT_RADIUS;
      p.drawEllipse(point - QPoint(radius / 2, radius / 2), radius, radius);
    }
    // Convert std::vector<QPoint> to QPolygon
    QPolygon triangle;
    for (const auto& point : rightTrianglePoints) {
      triangle << point; // Add points to QPolygon
    }

    p.setBrush(Qt::NoBrush);    // No fill for the triangle
    p.setPen(QPen(Qt::red, 2)); // Red outline for the triangle
    p.drawPolygon(triangle);    // Draw the triangle
  }

  // Optionally, you can implement barycentric coordinates texturing here
  // and update the canvas accordingly if the checkbox is checked
}

void
Ekran::mouseMoveEvent(QMouseEvent* event)
{
  if (!mousePresssed || !selectedPoint) {
    return;
  }
  // canvas = canvasClone;

  QPoint p = event->pos();
  *selectedPoint = event->pos();

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
    // case 4: {
    //   if (selectedPoint) {
    //     *selectedPoint = p;

    //     // TODO: this should redraw canvas without points
    //     canvas.fill(Qt::black);
    //     for (const auto& point : points) {
    //       bool currentlyMoved = (selectedPoint == &point);
    //       point.draw(this->canvas, currentlyMoved);
    //     }
    //   }

    //   drawBezier(this->canvas, points);
    // } break;
    case 5:
      break;
    default:
      break;
  }

  update();
}
