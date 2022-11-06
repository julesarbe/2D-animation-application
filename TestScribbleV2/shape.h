#ifndef SHAPE_H
#define SHAPE_H

#include "qcolor.h"
#include "qpainter.h"
#include "qpoint.h"
#include <QList>

class Shape {
 public:
    QPoint startPoint, endPoint;
    QString type;
    QColor color;
    int width;
    QList<QPoint> listOfPoints;


    Shape(QPoint start, QPoint end);
    Shape(QPoint start, QPoint end, QString type, QColor color, int width);
    Shape (QPoint a, QPoint b, QString typeShape, QColor c, int w, QList<QPoint> list);

    QPoint getStartPoint();
    QPoint getEndPoint();
    QString getType();
    QColor getColor();
    int getWidth();
    QList<QPoint> getlistOfPoints();
    void setColor(QColor c);

    static QString* createShape(QPoint start, QPoint end, QString type, QColor color, int Width);
    static QString* createDrawing(QPoint start, QPoint end, QString type, QColor color, int Width, QList<QPoint> list);

};

class Line :public Shape {

  public:
    Line(QPoint start, QPoint end, QColor color, int width);
};

class Square :public Shape {

  public:
    Square(QPoint start, QPoint end, QColor color, int width);
};

class Circle :public Shape {

  public:
    Circle(QPoint Start, QPoint End, QColor color, int width);

    QPoint get_center();
    QPoint get_radius();
};

class Drawing:public Shape {
  public :
    Drawing(QPoint Start, QPoint End, QColor color, int width, QList<QPoint> listOfPoints);
};

#endif // SHAPE_H
