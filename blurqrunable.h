#ifndef BLURQRUNABLE_H
#define BLURQRUNABLE_H
#include <QThreadPool>
#include <QRunnable>
#include <QImage>

#include "gaussianblur.h"

class BlurQRunable final: public QRunnable {
private:
  std::shared_ptr<GaussianBlur> blur_;
  bool finished_;
  QImage image_;
  QString filename_;

public:
  BlurQRunable(QImage imagen,QString filename, GaussianBlur blur):
    blur_(std::make_shared<GaussianBlur>(blur)),
    finished_(false),
    image_(imagen),
    filename_(filename){
    setAutoDelete(false);
  }

  virtual ~BlurQRunable() = default;

  void run() override;

  bool finished() const;
  QImage image() const;
  QString filename() const;
};

#endif // BLURQRUNABLE_H
