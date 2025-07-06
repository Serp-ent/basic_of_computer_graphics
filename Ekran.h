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

  struct Vec3
  {
    int x, y, z;
  };
  struct Vec2
  {
    int x, y;
  };

  struct Face
  {
    int a, b, c, d;
    bool visible;
  };

  std::vector<Vec3> points;
  std::vector<Vec3> pointsFaces;
  std::vector<Vec2> pointsOut;
  std::vector<Face> faces;

  QGroupBox* grupa;

  QLabel* labelTX = new QLabel("Przesunięcie x");
  QLabel* labelTY = new QLabel("Przesunięcie y");
  QLabel* labelTZ = new QLabel("Przesunięcie z");
  QLabel* labelRX = new QLabel("Obrót x");
  QLabel* labelRY = new QLabel("Obrót y");
  QLabel* labelRZ = new QLabel("Obrót z");
  QLabel* labelSX = new QLabel("Skalowanie x");
  QLabel* labelSY = new QLabel("Skalowanie y");
  QLabel* labelSZ = new QLabel("Skalowanie z");
  QLabel* labelBackLines = new QLabel("Pokaż Linie");

  QSlider* sliderTX = new QSlider(Qt::Horizontal);
  QSlider* sliderTY = new QSlider(Qt::Horizontal);
  QSlider* sliderTZ = new QSlider(Qt::Horizontal);
  QSlider* sliderSX = new QSlider(Qt::Horizontal);
  QSlider* sliderSY = new QSlider(Qt::Horizontal);
  QSlider* sliderSZ = new QSlider(Qt::Horizontal);
  QSlider* sliderRX = new QSlider(Qt::Horizontal);
  QSlider* sliderRY = new QSlider(Qt::Horizontal);
  QSlider* sliderRZ = new QSlider(Qt::Horizontal);

  QCheckBox* showBackLines;
  QImage texture;
  QCheckBox *showTexture;
  bool useBilinear = true;

  void loadCube();
  void mxmMultiply(float a[4][4], float b[4][4]);
  void mxvMultiply(float a[4][4], float b[4]);
  void transform();
  void drawCubeLines();
  void setVisibility();
  void drawCubeFaces();
  void drawTexturedTriangle(const QPoint& p1, const QPoint& p2, const QPoint& p3,
                            const QPoint& t1, const QPoint& t2, const QPoint& t3);
  void drawCubeTextured();
};

#endif // EKRAN_H
