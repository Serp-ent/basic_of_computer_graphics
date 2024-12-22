#ifndef LAYER_H
#define LAYER_H

#include <QImage>

class Layer
{
public:
  Layer(QImage image, float alpha, int mode, QString name)
    : image(std::move(image))
    , alpha(alpha)
    , mode(mode)
    , name(std::move(name))
  {
  }

  QImage getImage() const { return image; }
  QString getName() const { return name; }

private:
  QImage image;
  float alpha;
  int mode;
  QString name;
};

#endif // LAYER_H
