#include "blurqrunable.h"

QImage BlurQRunable::image() const
{
  return image_;
}

QString BlurQRunable::filename() const
{
  return filename_;
}

bool BlurQRunable::finished() const
{
  return finished_;
}

void BlurQRunable::run(){
 GaussianBlur blur(3,5);
 image_ =  blur.ApplyGaussianFilterToImage(image_);
 finished_ = true;
}
