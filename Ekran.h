#ifndef EKRAN_H
#define EKRAN_H

#include "movablepoint.h"
#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QListWidget>
#include <QMouseEvent>
#include <QObject>
#include <QSlider>
#include <QWidget>
#include <cmath>
#include <layer.h>

class Ekran : public QWidget
{
  Q_OBJECT
public:
  explicit Ekran(QWidget* parent = nullptr);
  QImage canvas;

protected:
  void paintEvent(QPaintEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;

signals:

  // QWidget interface
protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

private:
  bool mousePresssed = false;

  QPoint pressStart;
  QImage canvasClone;
  int tool = 6;

  // Bezier
  QPoint* selectedPoint = nullptr;
  // std::vector<MovablePoint> points;

  // scan line
  std::vector<QPoint> points;

  // alpha blending // exercise 6 *****************************************
  /*
  std::vector<Layer> layers;
  QSlider* alphaSlider; // TODO: read slider values form layers
  QLabel* alphaLabel;
  QListWidget* layerList;
  QComboBox* blendModeDropdown;
  void updateAlpha(int value)
  {
    float alpha = value / 100.0f;

    QListWidgetItem* selectedItem = layerList->currentItem();
    if (selectedItem) {
      QString layerName = selectedItem->text();
      for (auto& l : layers) {
        if (l.getName() == layerName) {
          l.setAlpha(alpha);
          break;
        }
      }
    }

    alphaLabel->setText(
      QString("Alpha: %1").arg(alpha, 0, 'f', 2)); // Update label
  }

  void onLayerSelectionChanged()
  {
    // Get the selected item
    QListWidgetItem* selectedItem = layerList->currentItem();
    if (selectedItem) {
      QString layerName = selectedItem->text();

      // Find the layer with the same name and set the alpha slider value based
      // on that layer's alpha
      for (auto& l : layers) {
        if (l.getName() == layerName) {
          float alpha = l.getAlpha();
          alphaSlider->setValue(
            static_cast<int>(alpha * 100)); // Set slider value (0 to 100)

          alphaLabel->setText(
            QString("Alpha: %1").arg(alpha, 0, 'f', 2)); // Update label

          int blendModeIndex =
            l.getBlendMode(); // Get the blend mode index (assuming it's stored
                              // as an int)
          blendModeDropdown->setCurrentIndex(
            blendModeIndex); // Set combo box to the appropriate blend mode

          break;
        }
      }
    }
  }

  void onBlendModeChanged(int modeIndex)
  {
    // Get the selected item
    QListWidgetItem* selectedItem = layerList->currentItem();
    if (selectedItem) {
      QString layerName = selectedItem->text();

      // Find the layer with the same name and set the blend mode
      for (auto& layer : layers) {
        if (layer.getName() == layerName) {
          // Set the blend mode based on the selected index
          layer.setBlendMode(modeIndex);
          break;
        }
      }
    }
  }
  // alpha blending // exercise 6 *****************************************/

  QImage img;
  float img_pos[3];
  float img_out_sizes[3];
  // transform
  float translation[3][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
  // no rotation in the begining
  float rotation[3][3] = {
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
  };
  // on scaling in the begining
  float scale_matrix[3][3] = {
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
  };

  float shearing_matrix[3][3] = {
    { 1, 0, 0 },
    { 0, 1, 0 },
    { 0, 0, 1 },
  };

  // TODO: use slider group
  QSlider* translationX;
  QSlider* translationY;
  QSlider* rotationSlider;
  QSlider* scalingXSlider;
  QSlider* scalingYSlider;
  QSlider* shearingXSlider;
  QSlider* shearingYSlider;
  void translate_x(int value) { translation[0][2] = value; };
  void translate_y(int value) { translation[1][2] = value; };
  void rotate_image(int degree)
  {
    double alpha = (degree / 180.0) * M_PI;
    rotation[0][0] = std::cos(alpha);
    rotation[0][1] = -std::sin(alpha);
    rotation[0][2] = 0;

    rotation[1][0] = std::sin(alpha);
    rotation[1][1] = std::cos(alpha);
    rotation[1][2] = 0;

    rotation[2][0] = 0;
    rotation[2][1] = 0;
    rotation[2][2] = 1;
  };

  float scale_value(int value)
  {
    if (value < 0 || value > 200) {
      return 1.0f; // Default scale
    }

    // Calculate scaling factor relative to 100
    return value / 100.0f;
  }

  float scale_shearing(int value)
  {
    if (value < -200 || value > 200) {
      return 1.0f; // Default scale
    }

    // Calculate scaling factor relative to 100
    return value / 100.0f;
  }

  void scale_x(int value) { scale_matrix[0][0] = scale_value(value); }
  void scale_y(int value) { scale_matrix[1][1] = scale_value(value); }

  void shear_x(int value) { shearing_matrix[0][1] = scale_shearing(value); }
  void shear_y(int value) { shearing_matrix[1][0] = scale_shearing(value); }
};

#endif // EKRAN_H
