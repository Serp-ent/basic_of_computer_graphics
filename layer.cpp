#include "layer.h"
#include <QDebug>

float
Layer::getAlpha() const
{
  return alpha;
}

void
Layer::setAlpha(float newAlpha)
{
  if (newAlpha < 0 || newAlpha > 1) {
    throw new std::runtime_error("Alpha should in range [0,1]");
  }
  alpha = newAlpha;
}

int
Layer::getBlendMode() const
{
  return mode;
}

void
Layer::setBlendMode(int newMode)
{

  qDebug()
    << QString("Layer::setBlendMode of '%1' to %2\n").arg(name).arg(newMode);
  // TODO: check if we are in range
  mode = newMode;
}
