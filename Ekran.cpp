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
  scalingXSlider->setGeometry(width() - 200 - 10, 180, 200, 30);
  connect(scalingXSlider, &QSlider::valueChanged, this, &Ekran::scale_x);
  // scaleYSlider
  scalingYSlider = new QSlider(Qt::Horizontal, this);
  scalingYSlider->setRange(0, scale_bound);
  scalingYSlider->setValue(scale_bound / 2);
  scalingYSlider->setGeometry(width() - 200 - 10, 210, 200, 30);
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
  shearingXSlider->setGeometry(width() - 200 - 10, 290, 200, 30);
  connect(shearingXSlider, &QSlider::valueChanged, this, &Ekran::shear_x);
  // shearing Y;
  shearingXSlider = new QSlider(Qt::Horizontal, this);
  shearingXSlider->setRange(-shearing_bound, shearing_bound);
  shearingXSlider->setValue(0);
  shearingXSlider->setGeometry(width() - 200 - 10, 320, 200, 30);
  connect(shearingXSlider, &QSlider::valueChanged, this, &Ekran::shear_y);
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

  p.drawImage(10, 10, canvas);

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

  float translationToOrigin[3][3] = {
    { 1, 0, -(img_pos[0] + img.width() / 2.0f) },
    { 0, 1, -(img_pos[1] + img.height() / 2.0f) },
    { 0, 0, 1 }
  };

  float translationBack[3][3] = { { 1, 0, (img_pos[0] + img.width() / 2.0f) },
                                  { 0, 1, (img_pos[1] + img.height() / 2.0f) },
                                  { 0, 0, 1 } };

  // Start with an identity matrix
  float combinedMatrix[3][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };

  // Combine the matrices in the correct order
  float tempMatrix[3][3];

  // 1. Translate to origin
  multiply3x3(translationToOrigin, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 2. Apply scaling
  multiply3x3(scale_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 3. Apply rotation
  multiply3x3(rotation_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 4. Apply shearing
  multiply3x3(shearing_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 5. Translate back to the original position
  multiply3x3(translationBack, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 6. Apply the final translation
  multiply3x3(translation_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  QImage transformedCanvas = canvas;
  for (int y = 0; y < img.height(); ++y) {
    for (int x = 0; x < img.width(); ++x) {
      float pixelPos[3] = { img_pos[0] + (float)x,
                            img_pos[1] + (float)y,
                            1 }; // Pixel coordinates
      float transformedPos[3] = { 0, 0, 1 };

      // Apply the combined transformation matrix
      multiply3x1(combinedMatrix, pixelPos, transformedPos);

      // Retrieve pixel color and draw the transformed pixel
      QColor pixel = img.pixelColor(x, y);
      uint red = pixel.red();
      uint green = pixel.green();
      uint blue = pixel.blue();

      // drawPixel(transformedCanvas,
      //           transformedPos[0],
      //           transformedPos[1],
      //           red,
      //           green,
      //           blue);
    }
  }

  // ************************************************************

  float inv_translationToOrigin[3][3] = {
    { 1, 0, -(img_pos[0] + img.width() / 2.0f) },
    { 0, 1, -(img_pos[1] + img.height() / 2.0f) },
    { 0, 0, 1 }
  };
  // Translate back from the center of the image (from origin)
  float inv_translationBack[3][3] = {
    { 1, 0, img_pos[0] + img.width() / 2.0f },
    { 0, 1, img_pos[1] + img.height() / 2.0f },
    { 0, 0, 1 }
  };
  float inv_translation_matrix[3][3];
  copyMatrix(translation_matrix, inv_translation_matrix);
  inv_translation_matrix[0][2] = -1 * inv_translation_matrix[0][2];
  inv_translation_matrix[1][2] = -1 * inv_translation_matrix[1][2];

  float inv_scale_matrix[3][3];
  copyMatrix(scale_matrix, inv_scale_matrix);
  inv_scale_matrix[0][0] = 1 / scale_matrix[0][0];
  inv_scale_matrix[1][1] = 1 / scale_matrix[1][1];
  inv_scale_matrix[0][2] = 0; // Reset translation part
  inv_scale_matrix[1][2] = 0;

  float inv_rotation_matrix[3][3];
  copyMatrix(rotation_matrix, inv_rotation_matrix);
  inv_rotation_matrix[0][0] = std::cos(alpha);
  inv_rotation_matrix[0][1] = std::sin(alpha);
  inv_rotation_matrix[1][0] = -std::sin(alpha);
  inv_rotation_matrix[1][1] = std::cos(alpha);

  float inv_shearing_matrix[3][3];
  copyMatrix(shearing_matrix, inv_shearing_matrix);
  inv_shearing_matrix[0][1] = -shearing_matrix[0][1];
  inv_shearing_matrix[1][0] = -shearing_matrix[1][0];

  /*
  // 1. Translate to origin
  multiply3x3(translationToOrigin, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 2. Apply scaling
  multiply3x3(scale_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 3. Apply rotation
  multiply3x3(rotation_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 4. Apply shearing
  multiply3x3(shearing_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 5. Translate back to the original position
  multiply3x3(translationBack, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);

  // 6. Apply the final translation
  multiply3x3(translation_matrix, combinedMatrix, tempMatrix);
  copyMatrix(tempMatrix, combinedMatrix);
  */

  // Start with the identity matrix for invMatrix
  float invMatrix[3][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
  // multiply the inverse matrices in reverse order
  // Step 1: Undo general translation
  multiply3x3(inv_translation_matrix, invMatrix, tempMatrix);
  copyMatrix(tempMatrix, invMatrix);
  // to origin
  multiply3x3(translationToOrigin, invMatrix, tempMatrix);
  copyMatrix(tempMatrix, invMatrix);
  // Step 4: Undo shearing
  multiply3x3(inv_shearing_matrix, invMatrix, tempMatrix);
  copyMatrix(tempMatrix, invMatrix);
  // Step 2: Undo rotation around the center
  multiply3x3(inv_rotation_matrix, invMatrix, tempMatrix);
  copyMatrix(tempMatrix, invMatrix);
  // Step 3: Undo scaling (also around the center)
  multiply3x3(inv_scale_matrix, invMatrix, tempMatrix);
  copyMatrix(tempMatrix, invMatrix);
  // Step 6: Undo translation back to the original position
  multiply3x3(translationBack, invMatrix, tempMatrix);
  copyMatrix(tempMatrix, invMatrix);

  // go through every pixel of transformedCanvas
  // use inverse matrix to seek in old image
  // fill color and use bilinear interpolation

  for (int y = 0; y < transformedCanvas.height(); ++y) {
    for (int x = 0; x < transformedCanvas.width(); ++x) {
      float pixel[3];
      pixel[0] = x;
      pixel[1] = y;
      pixel[2] = 1;

      // For given [x, y, 1] matrix, do inverse
      float invPos[3] = { 0, 0, 0 };
      multiply3x1(invMatrix, pixel, invPos);

      // If the calculated position is out of the image bounds, skip
      if ((invPos[0] < img_pos[0] || invPos[0] >= img_pos[0] + img.width()) ||
          (invPos[1] < img_pos[1] || invPos[1] >= img_pos[1] + img.height())) {
        continue;
      }

      // Check for clear hit (no fractional part)
      if (invPos[0] == std::floor(invPos[0]) &&
          invPos[1] == std::floor(invPos[1])) {
        // Clear hit: get the color from the image
        int x0 = static_cast<int>(invPos[0]) - static_cast<int>(img_pos[0]);
        int y0 = static_cast<int>(invPos[1]) - static_cast<int>(img_pos[1]);

        // Ensure coordinates are within the image bounds
        if (x0 < 0 || x0 >= img.width() || y0 < 0 || y0 >= img.height()) {
          continue;
        }

        const QColor& color = img.pixelColor(x0, y0);

        // Draw the transformed pixel with the clear hit color
        drawPixel(
          transformedCanvas, x, y, color.red(), color.green(), color.blue());
        continue; // Skip to the next pixel
      }

      // Perform bilinear interpolation if not a clear hit
      int x0 =
        static_cast<int>(std::floor(invPos[0])) - static_cast<int>(img_pos[0]);
      int y0 =
        static_cast<int>(std::floor(invPos[1])) - static_cast<int>(img_pos[1]);
      int x1 = x0 + 1;
      int y1 = y0 + 1;

      // Ensure surrounding coordinates are within bounds
      if (x0 < 0 || x0 >= img.width() || y0 < 0 || y0 >= img.height() ||
          x1 < 0 || x1 >= img.width() || y1 < 0 || y1 >= img.height()) {
        continue;
      }

      // Calculate interpolation weights (a and b)
      float a = invPos[0] - std::floor(invPos[0]);
      float b = invPos[1] - std::floor(invPos[1]);

      // Get the colors of the four surrounding pixels
      QColor p1 = img.pixelColor(x0, y0);
      QColor p2 = img.pixelColor(x1, y0);
      QColor p3 = img.pixelColor(x0, y1);
      QColor p4 = img.pixelColor(x1, y1);

      // Perform bilinear interpolation for each color channel
      QColor interpolatedColor =
        bilinear_interpolation_color(a, b, p1, p2, p3, p4);

      // Draw the interpolated pixel
      drawPixel(transformedCanvas,
                x,
                y,
                interpolatedColor.red(),
                interpolatedColor.green(),
                interpolatedColor.blue());
    }
  }
  p.drawImage(10, 10, transformedCanvas);

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
