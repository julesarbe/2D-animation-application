#include "frame.h"
#include "frame.cpp"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "drawingarea.h"
#include "shape.h"
#include "shape.cpp"

using namespace std;

DrawingArea::DrawingArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    label = new QLabel(this);

    modified = false;
    drawing = false;
    shapeType = "handdraw";
    myPenWidth = 2;
    myPenColor = Qt::blue;

    greyPenColor = QColor(200,200,200);
    ghostsNumber = 1;

    isPlay = false;
    isBackground = false;
    background = new Frame(0);

    startFrame = 1;
    endFrame = 51;
    currentFrame = 1;

    listFrames = {};
    for (int i=startFrame; i<=endFrame;i++){
        Frame f(i);
        listFrames.append(f);
    }

    label->setText(QString::fromStdString("Current frame : ")+QVariant(currentFrame).toString()+QString::fromStdString("    "));
}

void DrawingArea::goToFrame(int i){
    currentFrame = i;
    clearImage();
    drawFrame(currentFrame, 255, false);
    for (int g=1;g<=ghostsNumber;g++){
        if (currentFrame != endFrame){
            drawFrame(currentFrame+g, (100/(ghostsNumber+1))*(ghostsNumber+1-g), true);
        }
        if (currentFrame != startFrame){
            drawFrame(currentFrame-g, (100/(ghostsNumber+1))*(ghostsNumber+1-g), true);
        }
    }
    myPenColor=lastPenColor;
    myPenWidth=lastPenWidth;
}

void DrawingArea::nextFrame(){
    if (currentFrame != endFrame){
        currentFrame+=1;
        clearImage();
        drawBackground(255, false);
        drawFrame(currentFrame, 255, false);
        drawGhosts(currentFrame);
    }
    myPenColor=lastPenColor;
    myPenWidth=lastPenWidth;
}

void DrawingArea::drawGhosts(int i){
    for (int g=1;g<=ghostsNumber;g++){
        if (i != endFrame){
            drawFrame(i+g, (100/(ghostsNumber+1))*(ghostsNumber+1-g), true);
        }
        if (i != startFrame){
            drawFrame(i-g, (100/(ghostsNumber+1))*(ghostsNumber+1-g), true);
        }
    }
}

void DrawingArea::previousFrame(){
    if(currentFrame != startFrame){
        currentFrame-=1;
        clearImage();
        drawBackground(255, false);
        drawFrame(currentFrame, 255, false);
        drawGhosts(currentFrame);
    }
    myPenColor=lastPenColor;
    myPenWidth=lastPenWidth;
}

void DrawingArea::playNextFrame(){
    if (isPlay){
        if (currentFrame<endFrame-1){
            currentFrame+=1;
            clearImage();
            drawBackground(255, false);
            drawFrame(currentFrame, 255, false);
        }
        else {
            currentFrame = 1;
            clearImage();
            drawBackground(255, false);
            drawFrame(currentFrame, 255, false);
        }
    }
    label->setText(QString::fromStdString("Current frame : ")+QVariant(currentFrame).toString());
}

void DrawingArea::drawBackground(int opacity, bool grey){
   QList<Shape*> shapes = background->listShapes;
   for (int s = 0; s < shapes.size(); s++) {
       drawShape(shapes[s], opacity, grey);
   }
}

void DrawingArea::setBackground(){ 
    if (isBackground){
        isBackground = false;
        QTextStream(stdout) << "Not drawing Background"<< endl;
    }
    else{
        isBackground = true;
        QTextStream(stdout) << "Drawing Background"<< endl;
    }

}

void DrawingArea::drawFrame(int i, int opacity, bool grey){
    QList<Shape*> shapes = listFrames[i].listShapes;
    for (int s = 0; s < shapes.size(); s++) {
        drawShape(shapes[s], opacity, grey);
    }
}

void DrawingArea::drawShape(Shape* shape, int opacity, bool grey){
    QPoint start = shape->getStartPoint();
    QPoint end = shape->getEndPoint();
    QString type = shape->getType();
    myPenColor = shape->getColor();
    myPenColor.setAlpha(opacity);
    myPenWidth = shape->getWidth();
    listOfPoints = shape->getlistOfPoints();
    if (type == "handdraw"){
        if (grey){
            greyPenColor.setAlpha(opacity);
            drawDrawing(start, end, greyPenColor, myPenWidth, listOfPoints);
        }
        drawDrawing(start, end, myPenColor, myPenWidth, listOfPoints);
    }
    if (grey){
        drawShape(start, end, type, greyPenColor, myPenWidth);
    }
    drawShape(start, end, type, myPenColor, myPenWidth);
}

void DrawingArea::drawDrawing(const QPoint &startPoint, const QPoint &endPoint, QColor color, int width, QList<QPoint> list){
    QPainter painter(&image);
    painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    for (int i=0;i<list.length()-1;i++){
        painter.drawLine(list[i], list[i+1]);
    }
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(startPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
}

void DrawingArea::drawShape(const QPoint &startPoint, const QPoint &endPoint, QString shape, QColor color, int width){
    QPainter painter(&image);
    painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    if (shape == "line"){
        painter.drawLine(startPoint, endPoint);
    }

    if (shape == "square"){
        QPoint a(startPoint.x(),endPoint.y()), c(endPoint.x(), startPoint.y());
        painter.drawLine(startPoint, a);
        painter.drawLine(a,endPoint);
        painter.drawLine(endPoint,c);
        painter.drawLine(c,startPoint);
    }
    if (shape == "circle"){
        QPoint center = (startPoint + endPoint)/2;
        double radiusx = abs(startPoint.x()-endPoint.x())/2;
        double radiusy = abs(startPoint.y()-endPoint.y())/2;
        painter.drawEllipse(center, (int) radiusx, (int) radiusy);
    }

    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(startPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

}

void DrawingArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));

    painter.drawLine(lastPoint, endPoint);

    modified = true;

    int rad = (myPenWidth / 2) + 2;

    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if (shapeType == "handdraw") {
        lastPoint = event->pos();
        listOfPoints = {lastPoint};
        drawing = true;
    }
    else {
        newShape = createShape(startPoint, event->pos(), shapeType, myPenColor, myPenWidth, listOfPoints);
        if (isBackground){
            background->listShapes.append(newShape);
        }
        else {
            listFrames[currentFrame].listShapes.append(newShape);
        }
        startPoint = event->pos();
        drawing = true;
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing){
        if (shapeType == "handdraw"){
            listOfPoints.append(event->pos());
            drawLineTo(event->pos());
        }
        else{
            if (isBackground){
                background->listShapes.removeLast();
                newShape = createShape(startPoint, event->pos(), shapeType, myPenColor, myPenWidth, listOfPoints);
                background->listShapes.append(newShape);
            }
            else {
                listFrames[currentFrame].listShapes.removeLast();
                newShape = createShape(startPoint, event->pos(), shapeType, myPenColor, myPenWidth, listOfPoints);
                listFrames[currentFrame].listShapes.append(newShape);
            }
            clearImage();
            drawBackground(255, false);
            drawGhosts(currentFrame);
            drawFrame(currentFrame, 255, false);
        }
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (drawing) {
        if (shapeType == "handdraw") {
            listOfPoints.append(event->pos());
            Shape* newShape = createShape(startPoint, event->pos(), shapeType, myPenColor, myPenWidth, listOfPoints);
            listFrames[currentFrame].listShapes.append(newShape);
            drawLineTo(event->pos());
        }
        else {
            if (isBackground){
                background->listShapes.removeLast();
                newShape = createShape(startPoint, event->pos(), shapeType, myPenColor, myPenWidth, listOfPoints);
                background->listShapes.append(newShape);
            }
            else {
                listFrames[currentFrame].listShapes.removeLast();
                newShape = createShape(startPoint, event->pos(), shapeType, myPenColor, myPenWidth, listOfPoints);
                listFrames[currentFrame].listShapes.append(newShape);
            }
            clearImage();
            drawBackground(255, false);
            drawGhosts(currentFrame);
            drawFrame(currentFrame, 255, false);
        }
        lastPenColor=myPenColor;
        lastPenWidth=myPenWidth;
        drawing = false;
    }
}


void DrawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect dirtyRect = event->rect();

    painter.drawImage(dirtyRect, image, dirtyRect);
}

void DrawingArea::print()
{
#if QT_CONFIG(printdialog)

    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}

void DrawingArea::lineMode(){
    shapeType = "line";
}

void DrawingArea::drawMode(){
    shapeType = "handdraw";
}

void DrawingArea::squareMode(){
    shapeType = "square";
}

void DrawingArea::circleMode(){
    shapeType = "circle";
}

void DrawingArea::playMode(){
    if (isPlay){
        isPlay = false;
    }
    else {
        isPlay = true;
    }
}

bool DrawingArea::openImage(const QString &fileName)
{
    QImage loadedImage;

    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

void DrawingArea::renderAnimation(){
    char fileFormat[3] = {'p', 'n', 'g'};
    QString initialPath = QDir::currentPath() + "/untitled."+fileFormat;
    QString saveName = QFileDialog::getSaveFileName(this, tr("Render As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)"));
    for (int i=startFrame;i<endFrame;i++){
        string fileN = saveName.toStdString();
        std::string::size_type pos = fileN.find('.');
        fileN = fileN.substr(0, pos);
        fileN = fileN+std::to_string(i)+".png";
        QString fileName = QString::fromStdString(fileN);
        clearImage();
        drawBackground(255, false);
        drawFrame(i, 255, false);
        saveImage(fileName, fileFormat);
    }
}

void DrawingArea::renderImage(){
    char fileFormat[3] = {'p', 'n', 'g'};
    QString initialPath = QDir::currentPath() + "/untitled."+fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Render As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)"));
    saveImage(fileName, fileFormat);
}

bool DrawingArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}

void DrawingArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void DrawingArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void DrawingArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void DrawingArea::eraseImage(){
    for (int i=0; i<listFrames[currentFrame].listShapes.length(); i++){
        listFrames[currentFrame].listShapes.clear();
    }
    clearImage();
    drawBackground(255, false);
    drawGhosts(currentFrame);
    drawFrame(currentFrame, 255, false);
}

void DrawingArea::eraseLastShape(){
    if (listFrames[currentFrame].listShapes.empty()==false){
        listFrames[currentFrame].listShapes.removeLast();
        clearImage();
        drawBackground(255, false);
        drawGhosts(currentFrame);
        drawFrame(currentFrame, 255, false);
    }
}

void DrawingArea::eraseBackground(){
    background->listShapes.clear();
    clearImage();
    drawBackground(255, false);
    drawGhosts(currentFrame);
    drawFrame(currentFrame, 255, false);
}

void DrawingArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void DrawingArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void DrawingArea::getInfo(){
    QTextStream(stdout) << "Current Frame"<< currentFrame << endl;
    QTextStream(stdout) << "Current Frame Items" << listFrames[currentFrame].listShapes.length() << endl;

    QTextStream(stdout) << "Frame Before"<< currentFrame-1 << endl;
    QTextStream(stdout) << "Frame Before Items" << listFrames[currentFrame-1].listShapes.length() << endl;

    QTextStream(stdout) << "Frame After"<< currentFrame+1 << endl;
    QTextStream(stdout) << "Frame After Items" << listFrames[currentFrame+1].listShapes.length() << endl;
}
