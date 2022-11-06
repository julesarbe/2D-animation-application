#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include "frame.h"
#include "qlabel.h"
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class Shape;
class Frame;

class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    DrawingArea(QWidget *parent = 0);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setFrame (int i);
    void goToFrame(int i);

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }

    int startFrame;
    int endFrame;
    int currentFrame;
    int ghostsNumber;
    QLabel *label;
    QList<Frame> listFrames;
    Frame* background;

    Shape* newShape;

public slots:
    void clearImage();
    void print();
    void lineMode();
    void squareMode();
    void circleMode();
    void renderAnimation();
    void drawMode();
    void setBackground();
    void nextFrame();
    void previousFrame();
    void getInfo();
    void renderImage();
    void playNextFrame();
    void playMode();
    void eraseImage();
    void eraseLastShape();
    void eraseBackground();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    void drawFrame(int i, int opacity, bool grey);
    void drawGhosts(int i);
    void addShape(QPoint startPoint, QPoint endPoint, QString shape);
    void drawDrawing(const QPoint &startPoint, const QPoint &endPoint, QColor color, int width, QList<QPoint> list);
    void drawLineTo(const QPoint &endPoint);
    void drawShape(const QPoint &startPoint, const QPoint &endPoint, QString shape, QColor color, int width);
    void drawShape(Shape* shape, int opacity, bool grey);
    void resizeImage(QImage *image, const QSize &newSize);
    void drawBackground(int opacity, bool grey);

    bool modified;
    bool isPlay;

    QString shapeType;
    bool drawing;
    bool isBackground;

    int myPenWidth;
    QColor myPenColor;

    int lastPenWidth;
    QColor lastPenColor;

    QColor greyPenColor;

    QImage image;

    QPoint lastPoint;
    QPoint startPoint;
    QPoint endPoint;
    QList<QPoint> listOfPoints;
};

#endif
