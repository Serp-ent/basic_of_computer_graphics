#ifndef MOVABLEPOINT_H
#define MOVABLEPOINT_H

#include <QImage>
#include <QPoint>

class MovablePoint : public QPoint
{
public:
  static constexpr double POINT_RADIUS = 5;

  MovablePoint();
  MovablePoint(QPoint p);
  MovablePoint(int x, int y);

  void draw(QImage& canvas, bool moving = false) const;
};

#endif // MOVABLEPOINT_H
