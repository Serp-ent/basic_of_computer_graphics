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
  // alpha blending // exercise 6 *****************************************
};

#endif // EKRAN_H
