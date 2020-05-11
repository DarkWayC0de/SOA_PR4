#include <QCoreApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QElapsedTimer>
#include <iostream>
#include <QDir>
#include <QThread>

#include "gaussianblur.h"

#define RUTAORIG "Images/"
#define RUTARESULT "result/gb_"

QImage toGrayScale(QImage imagen);
void procesade(QStringList files);
QVector<QStringList> divide_list(QVector<QStringList> file_list, int nList, int dep = 0);

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Image Processing Pipeline");

    //Command line argument capture
    QCommandLineParser comand;
    comand.setApplicationDescription("Image Processing Pipeline.\n\n"
                                     "By default it runs sequentially.");
    comand.addHelpOption();
    comand.addOptions({
       {
         {"d", "divideAndConquer"},
         QCoreApplication::translate("main","Strategy of \"divide and conquer\" If <nthreads> is 0, a single thread is used (equivalent to the sequential approximation). If it is 1, the list is divided once. For 2, twice. And successively."),
         QCoreApplication::translate("main","nthreads"),
       },
       {
         {"p", "threadpoll"},
         QCoreApplication::translate("main","Strategy of \"threadpoll\"  With two Runnables, one for each task (grayscale and blur). The first injects the second. <nthreads> will be the number of threads in the Qt threadpool."),
         QCoreApplication::translate("main","nthreads"),
       },
    });
    comand.process(a);
    int nthreads = 0;
    QElapsedTimer timer;
    QDir dir(RUTAORIG);
    QStringList filtro;
    filtro << "*.png" << "*.PNG" << "*.jpg" << "*.JPG";
    filtro << "*.jpeg" << "*.JPEG";
    dir.setNameFilters(filtro);
    QStringList files = dir.entryList();

    if(!comand.value("divideAndConquer").isEmpty() && !comand.value("threadpoll").isEmpty()){
        QTextStream(stdout)<<"ERROR: Cannot process threadpool and divide and conquer at the same time\n";
        return 1;
    }
    QTextStream(stdout)<<"Strategy has been chosen: ";
    if(!comand.value("threadpoll").isEmpty() && (nthreads = comand.value("threadpoll").toInt())){
        QTextStream(stdout)<<"threadpoll.\n";
        timer.start();

    }else if(!comand.value("divideAndConquer").isEmpty() && (nthreads = comand.value("divideAndConquer").toInt())){
        QTextStream(stdout)<<"divide and conquer.\n";
        timer.start();
        QVector<QStringList> list = divide_list(QVector<QStringList> {files},nthreads);
        QVector<QThread*> threads;

        for (auto &itemlist:list ){
            QThread *athread = QThread::create(&procesade,itemlist);
            athread -> start();
            threads << athread;
        }
        for (auto& athread:threads){
            athread -> wait();
          }
    }else{
        QTextStream(stdout)<<"secuential.\n";
        timer.start();
        procesade(files);
    }

    std::cout << "Elapsed " << timer.elapsed() << " ms\n";
    return 0;
}

QImage toGrayScale(QImage imagen){
    for(int i=0; i < imagen.width(); i++)
    {
        for(int j=0; j < imagen.height(); j++)
        {
            int graypixel = qGray(imagen.pixel(i,j));
            imagen.setPixel(i,j, QColor(graypixel,graypixel,graypixel).rgb());
        }
    }
    return imagen;
}

void procesade(QStringList files){
  GaussianBlur blur(3,5);

  for ( const auto& i : files) {
      std::cout << i.toStdString() << "\n";
      QImage imagen(RUTAORIG+i);
      QImage grayimage, result;
      grayimage = toGrayScale(imagen);
      result = blur.ApplyGaussianFilterToImage(grayimage);
      result.save(RUTARESULT+i);
      QString savedFileName;
      savedFileName = RUTARESULT + i;
      savedFileName.replace(".","_gsb.");
      result.save(savedFileName);
  }
}

QVector<QStringList> divide_list(QVector<QStringList> file_list, int nList, int dep){
  if(dep < nList){
    QStringList newlist = file_list.first();
    int half = newlist.length() / 2 - 1;
    QStringList l1 ;
    QStringList l2 ;
    for(auto item:newlist){
        if(half-->0){
            l1.push_back(item);
          }else{
            l2.push_back(item);
          }
    }
    QVector<QStringList> q1;
    q1.push_back(l1);
    QVector<QStringList> q2;
    q2.push_back(l2);
    return divide_list(q1,nList, dep + 1) + divide_list(q2,nList, dep + 1);
  } else {
    return file_list;
  }
}
