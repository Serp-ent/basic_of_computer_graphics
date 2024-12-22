#include "layer.h"

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
