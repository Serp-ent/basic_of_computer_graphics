#include "Ekran.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

#include "layer.h"
#include "movablepoint.h"
#include <QBoxLayout>
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

  // if (event->button() == Qt::LeftButton) {
  //   // Check if any of the predefined points is near the mouse click
  //   bool pointNearby = false;
  //   for (auto& point : leftTrianglePoints) {
  //     if (euclideanSquare(pressStart, point) <=
  //         MovablePoint::POINT_RADIUS * MovablePoint::POINT_RADIUS) {
  //       pointNearby = true;
  //       selectedPoint = &point;
  //       return;
  //     }
  //   }
  //   for (auto& point : rightTrianglePoints) {
  //     if (euclideanSquare(pressStart, point) <=
  //         MovablePoint::POINT_RADIUS * MovablePoint::POINT_RADIUS) {
  //       pointNearby = true;
  //       selectedPoint = &point;
  //       return;
  //     }
  //   }
  // }

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
  if (event->button() == Qt::LeftButton) {
    selectedPoint = nullptr;
  }
}

void
Ekran::mxmMultiply(float a[4][4], float b[4][4])
{
  float c[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      c[i][j] = b[i][j];
    }
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      float suma = 0.0;
      for (int k = 0; k < 4; k++) {
        suma += a[i][k] * c[k][j];
      }
      mP[i][j] = suma;
    }
  }
}

void
Ekran::mxvMultiply(float a[4][4], float b[4])
{
  for (int i = 0; i < 4; i++) {
    vP[i] = 0;
  }

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      vP[i] += (a[i][j] * b[j]);
    }
  }
}

void
Ekran::transform()
{
  float a[4][4];
  float c[4]; //[3];

  float t1[4][4]; // z + 100

  float t[4][4];  // move
  float rx[4][4]; // rotation
  float ry[4][4];
  float rz[4][4];
  float s[4][4]; // scaling

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      a[i][j] = 0;
      t1[i][j] = 0;
      t[i][j] = 0;
      rx[i][j] = 0;
      ry[i][j] = 0;
      rz[i][j] = 0;
      s[i][j] = 0;
    }
    c[i] = 0;
  }

  t[0][0] = 1;
  t[1][1] = 1;
  t[2][2] = 1;
  t[3][3] = 1;

  t[0][3] = sliderTXValue;
  t[1][3] = sliderTYValue;
  t[2][3] = sliderTZValue;

  t1[0][0] = 1; // T(z+500)
  t1[1][1] = 1;
  t1[2][2] = 1;
  t1[3][3] = 1;

  t1[0][3] = 1;
  t1[1][3] = 1;
  t1[2][3] = 500;

  rz[0][0] = cos(sliderRZValue * M_PI / 180); // R
  rz[0][1] = -sin(sliderRZValue * M_PI / 180);
  rz[1][0] = sin(sliderRZValue * M_PI / 180);
  rz[1][1] = cos(sliderRZValue * M_PI / 180);
  rz[2][2] = 1;
  rz[3][3] = 1;

  rx[0][0] = 1;
  rx[1][1] = cos(sliderRXValue * M_PI / 180);
  rx[1][2] = -sin(sliderRXValue * M_PI / 180);
  rx[2][1] = sin(sliderRXValue * M_PI / 180);
  rx[2][2] = cos(sliderRXValue * M_PI / 180);
  rx[3][3] = 1;

  ry[0][0] = cos(sliderRYValue * M_PI / 180);
  ry[0][2] = sin(sliderRYValue * M_PI / 180);
  ry[1][1] = 1;
  ry[2][0] = -sin(sliderRYValue * M_PI / 180);
  ry[2][2] = cos(sliderRYValue * M_PI / 180);
  ry[3][3] = 1;

  s[0][0] = sliderSXValue / 100.0;
  s[1][1] = sliderSYValue / 100.0;
  s[2][2] = sliderSZValue / 100.0;
  s[3][3] = 1;

  a[0][0] = 1;
  a[1][1] = 1;
  a[2][2] = 1;
  a[3][3] = 1;

  mxmMultiply(s, a);
  mxmMultiply(rx, mP);
  mxmMultiply(ry, mP);
  mxmMultiply(rz, mP);
  mxmMultiply(t, mP);
  mxmMultiply(t1, mP);

  canvas.fill(Qt::black);

  float cx, cy;
  cx = szer / 2;
  cy = wys / 2;
  f = 1000;
  c[3] = 1;

  for (int i = 0; i < points.size(); i++) {
    c[0] = points[i].x;
    c[1] = points[i].y;
    c[2] = points[i].z;

    mxvMultiply(mP, c);

    if (vP[2] < 0) {
      vP[2] = 1;
    }

    point p = { (int)vP[0], (int)vP[1], (int)vP[2] };
    pointsFaces[i] = p;

    float tempx, tempy;
    tempx = (vP[0] * f) / vP[2] + cx;
    tempy = (vP[1] * f) / vP[2] + cy;

    pointsOut[i].first = tempx;
    pointsOut[i].second = tempy;
  }

  if (showBackLines->isChecked()) {
    drawCubeLines();
  } else {
    setVisibility();
    drawCubeFaces();
  }

  update();
}

void
Ekran::loadCube()
{
  int r = 100;

  point p1 = { -r, r, r };
  points.push_back(p1);
  pointsFaces.push_back(p1);
  pointsOut.push_back(std::make_pair(-r, r));
  point p2 = { r, r, r };
  points.push_back(p2);
  pointsOut.push_back(std::make_pair(r, r));
  pointsFaces.push_back(p2);
  point p3 = { r, -r, r };
  points.push_back(p3);
  pointsFaces.push_back(p3);
  pointsOut.push_back(std::make_pair(r, -r));
  point p4 = { -r, -r, r };
  points.push_back(p4);
  pointsFaces.push_back(p4);
  pointsOut.push_back(std::make_pair(-r, -r));

  point p5 = { -r, r, -r };
  points.push_back(p5);
  pointsFaces.push_back(p5);
  pointsOut.push_back(std::make_pair(-r, r));
  point p6 = { r, r, -r };
  points.push_back(p6);
  pointsFaces.push_back(p6);
  pointsOut.push_back(std::make_pair(r, r));
  point p7 = { r, -r, -r };
  points.push_back(p7);
  pointsFaces.push_back(p7);
  pointsOut.push_back(std::make_pair(r, -r));
  point p8 = { -r, -r, -r };
  points.push_back(p8);
  pointsFaces.push_back(p8);
  pointsOut.push_back(std::make_pair(-r, -r));

  face f1 = { 1, 0, 3, 2, false };
  faces.push_back(f1);
  face f2 = { 4, 5, 6, 7, false };
  faces.push_back(f2);
  face f3 = { 5, 1, 2, 6, false };
  faces.push_back(f3);
  face f4 = { 0, 4, 7, 3, false };
  faces.push_back(f4);
  face f5 = { 0, 1, 5, 4, false };
  faces.push_back(f5);
  face f6 = { 7, 6, 2, 3, false };
  faces.push_back(f6);
}

void
Ekran::drawCubeLines()
{
  drawLine(canvas,
           pointsOut[0].first,
           pointsOut[0].second,
           pointsOut[1].first,
           pointsOut[1].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[2].first,
           pointsOut[2].second,
           pointsOut[1].first,
           pointsOut[1].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[2].first,
           pointsOut[2].second,
           pointsOut[3].first,
           pointsOut[3].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[0].first,
           pointsOut[0].second,
           pointsOut[3].first,
           pointsOut[3].second,
           255,
           255,
           255);

  drawLine(canvas,
           pointsOut[5].first,
           pointsOut[5].second,
           pointsOut[4].first,
           pointsOut[4].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[5].first,
           pointsOut[5].second,
           pointsOut[6].first,
           pointsOut[6].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[7].first,
           pointsOut[7].second,
           pointsOut[6].first,
           pointsOut[6].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[7].first,
           pointsOut[7].second,
           pointsOut[4].first,
           pointsOut[4].second,
           255,
           255,
           255);

  drawLine(canvas,
           pointsOut[0].first,
           pointsOut[0].second,
           pointsOut[4].first,
           pointsOut[4].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[1].first,
           pointsOut[1].second,
           pointsOut[5].first,
           pointsOut[5].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[2].first,
           pointsOut[2].second,
           pointsOut[6].first,
           pointsOut[6].second,
           255,
           255,
           255);
  drawLine(canvas,
           pointsOut[3].first,
           pointsOut[3].second,
           pointsOut[7].first,
           pointsOut[7].second,
           255,
           255,
           255);
}

void
Ekran::setVisibility()
{
  for (int i = 0; i < faces.size(); i++) {
    float v1[3], v2[3];

    v2[0] = pointsFaces[faces[i].c].x - pointsFaces[faces[i].d].x;
    v2[1] = pointsFaces[faces[i].c].y - pointsFaces[faces[i].d].y;
    v2[2] = pointsFaces[faces[i].c].z - pointsFaces[faces[i].d].z;

    v1[0] = pointsFaces[faces[i].a].x - pointsFaces[faces[i].d].x;
    v1[1] = pointsFaces[faces[i].a].y - pointsFaces[faces[i].d].y;
    v1[2] = pointsFaces[faces[i].a].z - pointsFaces[faces[i].d].z;

    vOut[0] = v1[1] * v2[2] - v1[2] * v2[1];
    vOut[1] = v1[2] * v2[0] - v1[0] * v2[2];
    vOut[2] = v1[0] * v2[1] - v1[1] * v2[0];

    float vCam[3];
    vCam[0] = -pointsFaces[faces[i].d].x;
    vCam[1] = -pointsFaces[faces[i].d].y;
    vCam[2] = -pointsFaces[faces[i].d].z;

    float cos =
      (vOut[0] * vCam[0] + vOut[1] * vCam[1] + vOut[2] * vCam[2]) /
      (std::sqrt(vOut[0] * vOut[0] + vOut[1] * vOut[1] + vOut[2] * vOut[2]) *
       std::sqrt(vCam[0] * vCam[0] + vCam[1] * vCam[1] + vCam[2] * vCam[2]));

    if (cos > 0) {
      faces[i].visible = true;
    } else {
      faces[i].visible = false;
    }
  }
}

void
Ekran::drawCubeFaces()
{
  for (int i = 0; i < faces.size(); i++) {
    if (faces[i].visible == true) {
      drawLine(canvas,
               pointsOut[faces[i].a].first,
               pointsOut[faces[i].a].second,
               pointsOut[faces[i].b].first,
               pointsOut[faces[i].b].second,
               255,
               255,
               255);
      drawLine(canvas,
               pointsOut[faces[i].b].first,
               pointsOut[faces[i].b].second,
               pointsOut[faces[i].c].first,
               pointsOut[faces[i].c].second,
               255,
               255,
               255);
      drawLine(canvas,
               pointsOut[faces[i].c].first,
               pointsOut[faces[i].c].second,
               pointsOut[faces[i].d].first,
               pointsOut[faces[i].d].second,
               255,
               255,
               255);
      drawLine(canvas,
               pointsOut[faces[i].a].first,
               pointsOut[faces[i].a].second,
               pointsOut[faces[i].d].first,
               pointsOut[faces[i].d].second,
               255,
               255,
               255);
    }
  }
}

Ekran::Ekran(QWidget* parent)
  : QWidget{ parent }
{
  setWindowTitle("Kostka 3d");
  setMouseTracking(true);
  this->resize(800, 600);

  grupa = new QGroupBox(this);
  grupa->setGeometry(QRect(650, 50, 137, 500));
  QVBoxLayout* boxLayout = new QVBoxLayout(this);

  labelTX = new QLabel("Przesunięcie x");
  labelTY = new QLabel("Przesunięcie y");
  labelTZ = new QLabel("Przesunięcie z");
  labelRX = new QLabel("Obrót x");
  labelRY = new QLabel("Obrót y");
  labelRZ = new QLabel("Obrót z");
  labelSX = new QLabel("Skalowanie x");
  labelSY = new QLabel("Skalowanie y");
  labelSZ = new QLabel("Skalowanie z");
  labelBackLines = new QLabel("Pokaż Linie");

  boxLayout->addWidget(labelTX);
  sliderTX->setRange(-350, 350);
  sliderTX->setValue(0);
  sliderTXValue = 0;
  QObject::connect(sliderTX, &QSlider::valueChanged, [&](int i) {
    sliderTXValue = i;
    transform();
  });
  boxLayout->addWidget(sliderTX);

  boxLayout->addWidget(labelTY);
  sliderTY->setRange(-350, 350);
  sliderTY->setValue(0);
  sliderTYValue = 0;
  QObject::connect(sliderTY, &QSlider::valueChanged, [&](int i) {
    sliderTYValue = i;
    transform();
  });
  boxLayout->addWidget(sliderTY);

  boxLayout->addWidget(labelTZ);
  sliderTZ->setRange(-350, 350);
  sliderTZ->setValue(0);
  sliderTZValue = 0;
  QObject::connect(sliderTZ, &QSlider::valueChanged, [&](int i) {
    sliderTZValue = i;
    transform();
  });
  boxLayout->addWidget(sliderTZ);

  boxLayout->addWidget(labelRX);
  sliderRX->setRange(0, 360);
  sliderRX->setValue(0);
  sliderRXValue = 0;
  QObject::connect(sliderRX, &QSlider::valueChanged, [&](int i) {
    sliderRXValue = i;
    transform();
  });

  boxLayout->addWidget(sliderRX);

  boxLayout->addWidget(labelRY);
  sliderRY->setRange(0, 360);
  sliderRY->setValue(0);
  sliderRYValue = 0;
  QObject::connect(sliderRY, &QSlider::valueChanged, [&](int i) {
    sliderRYValue = i;
    transform();
  });

  boxLayout->addWidget(sliderRY);

  boxLayout->addWidget(labelRZ);
  sliderRZ->setRange(0, 360);
  sliderRZ->setValue(0);
  sliderRZValue = 0;
  QObject::connect(sliderRZ, &QSlider::valueChanged, [&](int i) {
    sliderRZValue = i;
    transform();
  });

  boxLayout->addWidget(sliderRZ);

  boxLayout->addWidget(labelSX);
  sliderSX->setRange(0, 100);
  sliderSX->setValue(50);
  sliderSXValue = 50;
  QObject::connect(sliderSX, &QSlider::valueChanged, [&](int i) {
    sliderSXValue = i;
    transform();
  });

  boxLayout->addWidget(sliderSX);

  boxLayout->addWidget(labelSY);
  sliderSY->setRange(0, 100);
  sliderSY->setValue(50);
  sliderSYValue = 50;
  QObject::connect(sliderSY, &QSlider::valueChanged, [&](int i) {
    sliderSYValue = i;
    transform();
  });

  boxLayout->addWidget(sliderSY);

  boxLayout->addWidget(labelSZ);
  sliderSZ->setRange(0, 100);
  sliderSZ->setValue(50);
  sliderSZValue = 50;
  QObject::connect(sliderSZ, &QSlider::valueChanged, [&](int i) {
    sliderSZValue = i;
    transform();
  });

  boxLayout->addWidget(sliderSZ);

  QHBoxLayout* backLinesRow = new QHBoxLayout();
  showBackLines = new QCheckBox(this);
  showBackLines->setChecked(true);
  QObject::connect(
    showBackLines, &QCheckBox::stateChanged, [&]() { transform(); });
  backLinesRow->addWidget(labelBackLines);
  backLinesRow->addWidget(showBackLines);

  boxLayout->addLayout(backLinesRow);

  grupa->setLayout(boxLayout);

  szer = 600;
  wys = 600;
  poczX = 25;
  poczY = 25;

  // Initialize the canvas
  canvas = QImage(width(), height(), QImage::Format_RGB32);

  update();

  loadCube();
  canvas.fill(Qt::black);
  transform();
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

  // Draw the left image (original)
  // QRect leftImageRect(10, 10, 400, 400);
  p.drawImage(0, 0, canvas);
}

void
Ekran::mouseMoveEvent(QMouseEvent* event)
{
  if (!mousePresssed || !selectedPoint) {
    return;
  }
  // canvas = canvasClone;

  QPoint p = event->pos();
  *selectedPoint = event->pos();

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
