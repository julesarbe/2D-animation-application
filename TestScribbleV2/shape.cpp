// base class
#include "qpoint.h"
#include "shape.h"
#include <QList>

Shape::Shape(QPoint a, QPoint b){
    startPoint = a;
    endPoint = b;
    color = QColor(0,0,255) ;
    width = 1;
    listOfPoints = {};
}

Shape::Shape (QPoint a, QPoint b, QString typeShape, QColor c, int w){
    startPoint = a;
    endPoint = b;
    type = typeShape;
    color = c;
    width = w;
    listOfPoints = {};
}

Shape::Shape (QPoint a, QPoint b, QString typeShape, QColor c, int w, QList<QPoint> list){
    startPoint = a;
    endPoint = b;
    type = typeShape;
    color = c;
    width = w;
    listOfPoints = list;
}

static Shape* createShape(QPoint a, QPoint b, QString typeShape, QColor c, int w, QList<QPoint> list){
    if (typeShape == "line"){
        Line* newShape = new Line(a, b, c, w);
        return newShape;
    }
    else if (typeShape == "circle"){
        Circle* newShape = new Circle(a, b, c, w);
        return newShape;
    }
    else if (typeShape == "square"){
        Square* newShape = new Square(a, b, c, w);
        return newShape;
    }
    else if (typeShape == "handdraw"){
        Drawing* newShape = new Drawing(a, b, c, w, list);
        return newShape;
    }
}

void Shape::setColor(QColor c){
    color = c;
}
QPoint Shape::getStartPoint(){
    return this->startPoint;
}
QPoint Shape::getEndPoint(){
    return this->endPoint;
}
QString Shape::getType(){
    return this->type;
}
QColor Shape::getColor(){
    return this->color;
}
int Shape::getWidth(){
    return this->width;
}
QList<QPoint> Shape::getlistOfPoints(){
    return this->listOfPoints;
}

Line::Line(QPoint a, QPoint b, QColor c, int w):Shape(a,b,QString::fromStdString("line"), c, w){
}

Square::Square(QPoint a, QPoint b, QColor c, int w):Shape(a,b, QString::fromStdString("square"), c, w){
}

Circle::Circle(QPoint a, QPoint b, QColor c, int w):Shape(a,b,QString::fromStdString("circle"), c, w){
}

Drawing::Drawing(QPoint a, QPoint b, QColor c, int w, QList<QPoint> list):Shape(a,b, QString::fromStdString("handdraw"), c, w, list){
}


