#ifndef BLURQRUNABLE_H
#define BLURQRUNABLE_H
#include <QThreadPool>
#include <QRunnable>
#include <QImage>

#include "gaussianblur.h"

class BlurQRunable final: public QRunnable {
private:
  bool finished_;
  QImage image_;
  QString filename_;

public:
  BlurQRunable(QImage imagen,QString filename):
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
