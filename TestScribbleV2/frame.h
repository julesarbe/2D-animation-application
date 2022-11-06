#ifndef FRAME_H
#define FRAME_H

#include "shape.h"

class Frame
{
    int frameNumber;
public:
    QList<Shape*> listShapes;
    Frame(int i);
    Frame(int i, QList<Shape*> list);
};

#endif // FRAME_H
