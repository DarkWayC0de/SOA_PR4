#include "globalfunctions.h"

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
