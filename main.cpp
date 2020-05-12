#include <iostream>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QElapsedTimer>
#include <iostream>
#include <QDir>
#include <QThread>
#include <QThreadPool>

#include "globalfunctions.h"
#include "grayscaleqrunable.h"
#include "blurqrunable.h"


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

        QThreadPool::globalInstance()->setMaxThreadCount(nthreads);
        QVector<GrayScaleQRunable*> pipelineGrayScale;
        QVector<BlurQRunable*> pipelineBlur;

        for (const auto& i: files){
            QString name =QString::fromStdString(i.toStdString());
            QTextStream(stdout) << name << "\n";
            QImage imagen(RUTAORIG+i);

            GrayScaleQRunable* aGrayScale =
                new GrayScaleQRunable(imagen,QString::fromStdString(i.toStdString()));

            QThreadPool::globalInstance()->start(aGrayScale);
            pipelineGrayScale << aGrayScale;
        }

        while (!(pipelineGrayScale.empty() && pipelineBlur.empty())) {
            if(!pipelineGrayScale.empty()){
              if(pipelineGrayScale.first()->finished()){
                  BlurQRunable* aBlur = new BlurQRunable(pipelineGrayScale.first()->image(),pipelineGrayScale.first()->filename());
                  delete  pipelineGrayScale.first();
                  pipelineGrayScale.pop_front();
                  QThreadPool::globalInstance()->start(aBlur);
                  pipelineBlur << aBlur;
               }
            }
            if(!pipelineBlur.empty()){
              if(pipelineBlur.first()->finished()){
                 pipelineBlur.first()->image().save(RUTARESULT+pipelineBlur.first()->filename());
                 delete pipelineBlur.first();
                 pipelineBlur.pop_front();
              }
            }
            std::cout<<"gray:"<<pipelineGrayScale.size()<<" blur:"<<pipelineBlur.size()<<" \n";
          }

      QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());

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




