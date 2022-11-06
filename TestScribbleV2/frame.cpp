#include "frame.h"

Frame::Frame(int i)
{
    frameNumber = i;
    listShapes = {};
}

Frame::Frame(int i, QList<Shape*> list)
{
    frameNumber = i;
    listShapes = list;
}
