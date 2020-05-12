#include "grayscaleqrunable.h"

QImage GrayScaleQRunable::image() const
{
  return image_;
}

QString GrayScaleQRunable::filename() const
{
  return filename_;
}

bool GrayScaleQRunable::finished() const
{
  return finished_;
}

void GrayScaleQRunable::run(){
 image_ =  toGrayScale(image_);
 finished_ = true;
}
