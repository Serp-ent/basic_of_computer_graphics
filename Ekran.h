#ifndef EKRAN_H
#define EKRAN_H

#include "movablepoint.h"
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

  // alpha blending // exercise 6
  std::vector<Layer> layers;
  QSlider* alphaSlider; // TODO: read slider values form layers
  QLabel* alphaLabel;
  QListWidget* layerList;
  void updateAlpha(float value)
  {
    qDebug() << "update alpha to " << value << '\n';
    // TODO: maybe invoke update here
  }
};

#endif // EKRAN_H
