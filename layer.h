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
    setAlpha(alpha);
  }

  QImage getImage() const { return image; }
  QString getName() const { return name; }

  float getAlpha() const;
  void setAlpha(float newAlpha);

private:
  QImage image;
  float alpha;
  int mode;
  QString name;
};

#endif // LAYER_H
