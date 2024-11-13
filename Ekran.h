#ifndef EKRAN_H
#define EKRAN_H

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
  void
  drawLine(int x1, int y1, int x2, int y2, uint red, uint green, uint blue);
  void drawCircle(int x0,
                  int y0,
                  double radius,
                  uint red,
                  uint green,
                  uint blue);

signals:

  // QWidget interface
protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

private:
  bool mousePresssed = false;
  QPoint pressStart;
  QImage canvasClone;
  int tool = 2;
};

#endif // EKRAN_H
