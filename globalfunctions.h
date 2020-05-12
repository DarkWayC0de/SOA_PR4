#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

#include <iostream>

#include "gaussianblur.h"

#define RUTAORIG "../soa-1920-pipeline-DarkWayC0de/Images/"
#define RUTARESULT "result/gb_"


QImage toGrayScale(QImage imagen);

void procesade(QStringList files);

QVector<QStringList> divide_list(QVector<QStringList> file_list, int nList, int dep = 0);



#endif // GLOBALFUNCTIONS_H
