#ifndef GRAYSCALEQRUNABLE_H
#define GRAYSCALEQRUNABLE_H
#include <QThreadPool>
#include <QRunnable>
#include <QImage>

#include "globalfunctions.h"

class GrayScaleQRunable: public QRunnable {
private:
  bool finished_;
  QImage image_;
  QString filename_;

public:
  GrayScaleQRunable(QImage imagen,QString filename):
    finished_(false),
    image_(imagen),
    filename_(filename){
    setAutoDelete(false);
  }

  virtual ~GrayScaleQRunable() = default;

  void run() override;

  bool finished() const;
  QImage image() const;
  QString filename() const;
};

#endif // GRAYSCALEQRUNABLE_H
