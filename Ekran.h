#ifndef EKRAN_H
#define EKRAN_H

#include "movablepoint.h"
#include <QMouseEvent>
#include <QObject>
#include <QWidget>

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

  // Bezier
  QPoint* selectedPoint = nullptr;
  int tool = 4;
  std::vector<MovablePoint> points;
};

#endif // EKRAN_H
