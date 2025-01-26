#ifndef EKRAN_H
#define EKRAN_H

#include "movablepoint.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QGroupBox>
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

  QImage canvas;
  int szer;
  int wys;
  int poczX;
  int poczY;
  int startX;
  int startY;

  float f;

  int sliderTXValue;
  int sliderTYValue;
  int sliderTZValue;

  float sliderRXValue;
  float sliderRYValue;
  float sliderRZValue;

  int sliderSXValue;
  int sliderSYValue;
  int sliderSZValue;

  float mP[4][4];
  float vP[4];

  float vOut[3];

  struct point
  {
    int x, y, z;
  };

  struct face
  {
    int a, b, c, d;
    bool visible;
  };

  std::vector<point> points;
  std::vector<std::pair<int, int>> pointsOut;
  std::vector<point> pointsFaces;
  std::vector<face> faces;

  QGroupBox* grupa;

  QLabel* labelTX;
  QLabel* labelTY;
  QLabel* labelTZ;
  QLabel* labelSX;
  QLabel* labelSY;
  QLabel* labelSZ;
  QLabel* labelRX;
  QLabel* labelRY;
  QLabel* labelRZ;

  QSlider* sliderTX = new QSlider(Qt::Horizontal);
  QSlider* sliderTY = new QSlider(Qt::Horizontal);
  QSlider* sliderTZ = new QSlider(Qt::Horizontal);
  QSlider* sliderSX = new QSlider(Qt::Horizontal);
  QSlider* sliderSY = new QSlider(Qt::Horizontal);
  QSlider* sliderSZ = new QSlider(Qt::Horizontal);
  QSlider* sliderRX = new QSlider(Qt::Horizontal);
  QSlider* sliderRY = new QSlider(Qt::Horizontal);
  QSlider* sliderRZ = new QSlider(Qt::Horizontal);

  void czysc();
  void mxmMultiply(float a[4][4], float b[4][4]);
  void mxvMultiply(float a[4][4], float b[4]);
  void transform();
  void loadCube();
  void drawCubeLines();
  void drawPixel(int x, int y, int r, int g, int b);
  void ekran_drawLine(int x1, int y1, int x2, int y2);
  void setVisibility();
  void drawCubeFaces();
};

#endif // EKRAN_H
