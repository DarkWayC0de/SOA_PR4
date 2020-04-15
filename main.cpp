#include <QCoreApplication>
#include <QImage>
#include <QElapsedTimer>
#include <iostream>
#include "gaussianblur.h"
#include <QDir>

QImage toGrayScale(QImage imagen)
{
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QElapsedTimer timer;
    timer.start();

    QDir dir("../../Images");
    QStringList filtro;
    filtro << "*.png" << "*.PNG" << "*.jpg" << "*.JPG";
    filtro << "*.jpeg" << "*.JPEG";
    dir.setNameFilters(filtro);
    QStringList files = dir.entryList();

    GaussianBlur blur(3,5);

    for ( const auto& i : files)
    {
        std::cout << i.toStdString() << "\n";
        QImage imagen("../../Images/"+i);
        QImage grayimage, result;
        grayimage = toGrayScale(imagen);
        result = blur.ApplyGaussianFilterToImage(grayimage);
        //result.save("gb_"+i);
        QString savedFileName;
        savedFileName = i;
        savedFileName.replace(".","_gb.");
        result.save(savedFileName);
    }

/*    QImage imagen("../../Images/lenna.png");
    QImage grayimage,result;

    grayimage = toGrayScale(imagen);

    GaussianBlur blur(3,5);

    result = blur.ApplyGaussianFilterToImage(grayimage);

    result.save("result.png"); */

    std::cout << "Elapsed " << timer.elapsed() << " ms\n";
    return 0;
    //return a.exec();
}
