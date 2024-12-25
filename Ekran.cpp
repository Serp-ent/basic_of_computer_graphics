#include "Ekran.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

#include "layer.h"
#include "movablepoint.h"
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

  canvas = QImage(width() - 50 - 200, height() - 20, QImage::Format_RGB32);
  canvas.fill(Qt::lightGray);

  /***** EXERCISE 6 **********************************************************
  layers.push_back(
    Layer(QImage(":/images/wilkizajac.jpg"), 0, 0, "Wilk_i_zajac"));
  layers.push_back(Layer(QImage(":/images/room.jpg"), 0, 0, "room"));
  layers.push_back(
    Layer(QImage(":/images/treasureIsland.jpg"), 0, 0, "treasure_island"));

  for (const auto& l : layers) {
    // make sure that all the images are open
    if (l.getImage().isNull()) {
      throw new std::runtime_error("Cannot load image");
    }
  }

  // Alpha slider setup
  alphaSlider = new QSlider(Qt::Horizontal, this);
  alphaSlider->setRange(0, 100);
  alphaSlider->setValue(100);
  alphaSlider->setGeometry(width() - 200 - 10, 50, 200, 30);
  connect(alphaSlider, &QSlider::valueChanged, this, &Ekran::updateAlpha);

  alphaLabel = new QLabel(this);
  alphaLabel->setText("Alpha: 100%");
  alphaLabel->setGeometry(alphaSlider->x() - 100, alphaSlider->y(), 90, 30);
  alphaLabel->setAlignment(Qt::AlignRight | Qt::AlignCenter);

  // Blend mode dropdown (QComboBox)
  // TODO: new modes should be read from the array
  blendModeDropdown = new QComboBox(this);
  blendModeDropdown->addItem("Normal");
  blendModeDropdown->addItem("Multiply");
  blendModeDropdown->addItem("Screen");
  blendModeDropdown->addItem("Overlay");
  blendModeDropdown->addItem("Darken");
  blendModeDropdown->addItem("Lighten");
  blendModeDropdown->setGeometry(alphaLabel->x(),
                                 alphaSlider->y() - 40,
                                 200,
                                 30); // Place it above the slider
  connect(blendModeDropdown,
          QOverload<int>::of(&QComboBox::currentIndexChanged),
          this,
          &Ekran::onBlendModeChanged);

  // Layer list setup
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
**********************************************************************************/

  img.load(":/images/wilk.png");
  if (img.isNull()) {
    throw new std::runtime_error{ "Cannot open file 'wilk.png' in exercise 6" };
  }

  const int x_center = 10 + canvas.width() / 2 - img.width() / 2;
  const int y_center = 10 + canvas.height() / 2 - img.height() / 2;
  img_pos[0] = x_center;
  img_pos[1] = y_center;
  img_pos[2] = 1;

  // TODO: add reset button
  // translationX;
  translationX = new QSlider(Qt::Horizontal, this);
  const int horizontal_bound = 250;
  translationX->setRange(-horizontal_bound, horizontal_bound);
  translationX->setValue(0);
  translationX->setGeometry(width() - 200 - 10, 50, 200, 30);
  connect(translationX, &QSlider::valueChanged, this, &Ekran::translate_x);
  // translationY;
  const int vertical_bound = 150;
  translationY = new QSlider(Qt::Horizontal, this);
  translationY->setRange(-vertical_bound, vertical_bound);
  translationY->setValue(0);
  translationY->setGeometry(width() - 200 - 10, 80, 200, 30);
  connect(translationY, &QSlider::valueChanged, this, &Ekran::translate_y);
  // rotationSlider;
  const int rotation_bound = 360;
  rotationSlider = new QSlider(Qt::Horizontal, this);
  rotationSlider->setRange(0, rotation_bound);
  rotationSlider->setValue(0);
  rotationSlider->setGeometry(width() - 200 - 10, 130, 200, 30);
  connect(rotationSlider, &QSlider::valueChanged, this, &Ekran::rotate_image);

  // Add the checkbox for maintaining aspect ratio
  QCheckBox* aspectRatioCheckBox = new QCheckBox("Maintain Aspect Ratio", this);
  aspectRatioCheckBox->setGeometry(width() - 200 - 10, 240, 200, 30);
  aspectRatioCheckBox->setChecked(false); // Default unchecked
  connect(aspectRatioCheckBox, &QCheckBox::stateChanged, [this](int state) {
    if (state == Qt::Checked) {
      // Force scalingYSlider to match scalingXSlider
      scalingYSlider->setValue(scalingXSlider->value());
    }
  });
  // scaleXSlider
  const int scale_bound = 200;
  scalingXSlider = new QSlider(Qt::Horizontal, this);
  scalingXSlider->setRange(0, scale_bound);
  scalingXSlider->setValue(scale_bound / 2);
  scalingXSlider->setGeometry(width() - 200 - 10, 180, 290, 30);
  connect(scalingXSlider, &QSlider::valueChanged, this, &Ekran::scale_x);
  // scaleYSlider
  scalingYSlider = new QSlider(Qt::Horizontal, this);
  scalingYSlider->setRange(0, scale_bound);
  scalingYSlider->setValue(scale_bound / 2);
  scalingYSlider->setGeometry(width() - 200 - 10, 210, 320, 30);
  connect(scalingYSlider, &QSlider::valueChanged, this, &Ekran::scale_y);
  // Connect the sliders for aspect ratio scaling
  connect(scalingXSlider,
          &QSlider::valueChanged,
          [this, aspectRatioCheckBox](int value) {
            if (aspectRatioCheckBox->isChecked()) {
              scalingYSlider->setValue(value);
            }
          });
  connect(scalingYSlider,
          &QSlider::valueChanged,
          [this, aspectRatioCheckBox](int value) {
            if (aspectRatioCheckBox->isChecked()) {
              scalingXSlider->setValue(value);
            }
          });

  // shearing X;
  shearingXSlider = new QSlider(Qt::Horizontal, this);
  const int shearing_bound = 50;
  shearingXSlider->setRange(-shearing_bound, shearing_bound);
  shearingXSlider->setValue(0);
  shearingXSlider->setGeometry(width() - 200 - 10, 260, 200, 30);
  connect(shearingXSlider, &QSlider::valueChanged, this, &Ekran::shear_x);
  // shearing Y;
  shearingXSlider = new QSlider(Qt::Horizontal, this);
  shearingXSlider->setRange(-shearing_bound, shearing_bound);
  shearingXSlider->setValue(0);
  shearingXSlider->setGeometry(width() - 200 - 10, 290, 200, 30);
  connect(shearingXSlider, &QSlider::valueChanged, this, &Ekran::shear_y);
}

void
Ekran::paintEvent(QPaintEvent* event)
{
  QPainter p(this);
  p.fillRect(0, 0, width(), height(), Qt::black);

  p.drawImage(10, 10, canvas);

  // EXERCISE 6 QImage blendedCanvas = canvas; // create copy of canvas
  QImage movingCanvas = canvas; // create copy of canvas

  /**** EXERCISE 6 **********************************************
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
          layer.getBlendMode(),
          blendedCanvas); // Update blendedCanvas with the result
  }

  p.drawImage(0, 0, blendedCanvas);
  *************************************************************/

  // do translation multiplications
  //* start of transform chain *************************************
  // translation

  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      float temp[3] = { img_pos[0] + (float)x,
                        img_pos[1] + (float)y,
                        1 }; // Coordinates of the current pixel
      float out[3] = { 0, 0, 1 };
      /* [1] translation
       * multiply3x1(translation, temp, out); // Apply translation
       */

      /* [2] rotation
      // Step 1: Translate the pixel to the origin (image center)
      float translationToOrigin[3][3] = {
        { 1, 0, -(img_pos[0] + img.width() / 2.0f) },
        { 0, 1, -(img_pos[1] + img.height() / 2.0f) },
        { 0, 0, 1 }
      };
      multiply3x1(translationToOrigin, temp, out);

      // // Step 2: Rotate the pixel around the origin
      multiply3x1(rotation, out, temp);

      // // Step 3: translation back to original position
      translationToOrigin[0][2] = -1 * translationToOrigin[0][2];
      translationToOrigin[1][2] = -1 * translationToOrigin[1][2];
      multiply3x1(translationToOrigin, temp, out);
       */

      /* [3] Scaling [TODO: add antialiasing]
      // Step 1: Translate the pixel to the origin (image center)
      float translationToOrigin[3][3] = {
        { 1, 0, -(img_pos[0] + img.width() / 2.0f) },
        { 0, 1, -(img_pos[1] + img.height() / 2.0f) },
        { 0, 0, 1 }
      };
      multiply3x1(translationToOrigin, temp, out);

      // Step 2: Rotate the pixel around the origin
      multiply3x1(scale_matrix, out, temp);

      // Step 3: translation back to original position
      translationToOrigin[0][2] = -1 * translationToOrigin[0][2];
      translationToOrigin[1][2] = -1 * translationToOrigin[1][2];
      multiply3x1(translationToOrigin, temp, out);
      */

      /* [4] Shearing []
       */
      multiply3x1(shearing_matrix, temp, out); // Apply translation

      QColor pixel = img.pixelColor(x, y);
      uint red = pixel.red();
      uint green = pixel.green();
      uint blue = pixel.blue();

      drawPixel(movingCanvas, out[0], out[1], red, green, blue);
    }
  }

  p.drawImage(10, 10, movingCanvas);

  update();

  //* start of transform chain *************************************

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
