#ifndef EKRAN_H
#define EKRAN_H

#include "movablepoint.h"
#include <QCheckBox>
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

  void onBilinearInterpolationToggled(bool checked)
  {
    // Handle the bilinear interpolation checkbox toggle
    if (checked) {
      // Enable bilinear interpolation
    } else {
      // Disable bilinear interpolation
    }
  }

private:
  bool mousePresssed = false;

  QPoint pressStart;
  QImage canvasClone;
  int tool = 6;

  // Bezier
  QPoint* selectedPoint = nullptr;
  // std::vector<MovablePoint> points;

  // scan line
  std::vector<QPoint> leftTrianglePoints;
  std::vector<QPoint> rightTrianglePoints;

  QImage img;
  QCheckBox* bilinearCheckBox;
};

#endif // EKRAN_H
