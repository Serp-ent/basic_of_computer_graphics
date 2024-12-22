#include "Ekran.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

#include "layer.h"
#include "movablepoint.h"
#include <QImage>
#include <QLabel>
#include <algorithm>
#include <tools.h>

void
Ekran::mousePressEvent(QMouseEvent* event)
{
  mousePresssed = true;
  pressStart = event->pos();

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
  if (tool == 4 && (event->button() == Qt::LeftButton)) {
    selectedPoint = nullptr;
  }
}

Ekran::Ekran(QWidget* parent)
  : QWidget{ parent }
{
  setMouseTracking(true);
  this->resize(1000, 600);

  canvas = QImage(640, 480, QImage::Format_RGB32);
  canvas.fill(0);

  layers.push_back(
    Layer(QImage(":/images/wilkizajac.jpg"), 0.5, 1, "Wilk_i_zajac"));
  layers.push_back(Layer(QImage(":/images/room.jpg"), 0.5, 1, "room"));
  layers.push_back(
    Layer(QImage(":/images/treasureIsland.jpg"), 0.5, 1, "treasure_island"));

  for (const auto& l : layers) {
    // make sure that all the images are open
    if (l.getImage().isNull()) {
      throw new std::runtime_error("Cannot load image");
    }
  }

  // alpha slider setup
  alphaSlider = new QSlider(Qt::Horizontal, this);
  alphaSlider->setRange(0, 100);
  alphaSlider->setValue(100);
  alphaSlider->setGeometry(width() - 200 - 10, 10, 200, 30);
  // TODO: conect to invoke action
  connect(alphaSlider, &QSlider::valueChanged, this, &Ekran::updateAlpha);
  alphaLabel = new QLabel(this);
  // TODO: change label on fly
  alphaLabel->setText("Alpha: 100%");
  alphaLabel->setGeometry(alphaSlider->x() - 100, alphaSlider->y(), 90, 30);
  alphaLabel->setAlignment(Qt::AlignRight | Qt::AlignCenter);

  // layer list setup
  layerList = new QListWidget(this);
  layerList->setGeometry(alphaLabel->x(),
                         alphaLabel->y() + 40,
                         alphaSlider->width() + alphaLabel->width(),
                         height() - alphaSlider->height() - 40);
  layerList->setSelectionMode(QAbstractItemView::SingleSelection);
  for (const auto& l : layers) {
    layerList->addItem(l.getName());
  }
  connect(layerList,
          &QListWidget::itemSelectionChanged,
          this,
          &Ekran::onLayerSelectionChanged);

  if (layerList->count() > 0) {
    layerList->setCurrentRow(0); // Select the first item (index 0)
  };
}

void
Ekran::paintEvent(QPaintEvent* event)
{
  QPainter p(this);
  p.fillRect(0, 0, width(), height(), Qt::black);

  p.drawImage(0, 0, canvas);

  QImage blendedCanvas = canvas; // create copy of canvas

  // TODO: maybe use reverse iterators
  // TOOL 6 -> Alpha blending
  for (int i = layers.size() - 1; i >= 0; --i) {
    // TODO: invoke blend function
    // TODO: blend(img, layers[i], alpha, mode[i], save into img)
    // doblendowywac do img
    // img jest jako background, a aktualna warstwa jako foreground
    const Layer& layer = layers.at(i);
    QImage layerImage = layer.getImage();
    float alpha = layer.getAlpha();
    // Blend current layer image with the blended canvas
    blend(blendedCanvas,
          layerImage,
          alpha,
          -1,
          blendedCanvas); // Update blendedCanvas with the result
  }

  p.drawImage(0, 0, blendedCanvas);

  update();

  // if (this->tool == 4) {
  //   // there should be draw points;
  //   for (const auto& p : points) {
  //     p.draw(this->canvas);
  //   }
  //}
}

void
Ekran::mouseMoveEvent(QMouseEvent* event)
{
  if (!mousePresssed) {
    return;
  }
  // canvas = canvasClone;

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
