#include "movablepoint.h"
#include <tools.h>

MovablePoint::MovablePoint() {}

MovablePoint::MovablePoint(QPoint p)
  : QPoint{ p }
{
}

MovablePoint::MovablePoint(int x, int y)
  : QPoint(x, y)
{
}

void
MovablePoint::draw(QImage& canvas, bool moving) const
{

  drawPixel(canvas, this->x(), this->y(), 255, 255, 255);
  if (moving)
    drawCircle(canvas, this->x(), this->y(), POINT_RADIUS, 0, 255, 0);
  else
    drawCircle(canvas, this->x(), this->y(), POINT_RADIUS, 255, 0, 0);
}
