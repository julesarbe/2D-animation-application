#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qgroupbox.h"
#include "qlineedit.h"
#include <QList>
#include <QMap>
#include <QMainWindow>

class DrawingArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    int startFrame;
    int endFrame;
    int currentFrame;
    QLineEdit *frame = new QLineEdit;

public slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void goToFrame();

private:
    void createActions();
    void createMenus();
    void createPlaybar();
    void createToolbar();

    bool maybeSave();

    bool saveFile(const QByteArray &fileFormat);

    DrawingArea *drawingArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *openMenu;
    QMenu *exitMenu;
    QMenu *colorMenu;
    QMenu *widthMenu;
    QMenu *styleMenu;
    QMenu *shapeMenu;
    QMenu *renderMenu;

    QAction *openAction;

    QList<QAction *> saveAsActions;
    QAction *exitAction;
    QAction *penColorAction;
    QAction *penWidthAction;
    QAction *printAction;
    QAction *lineAction;
    QAction *drawAction;
    QAction *penBackgroundAction;
    QAction *squareAction;
    QAction *circleAction;
    QAction *clearScreenAction;
    QAction *eraseFrameAction;
    QAction *eraseLastShapeAction;
    QAction *eraseBackgroundAction;
    QAction *nextFrameAction;
    QAction *playAction;
    QAction *previousFrameAction;
    QAction *getInfoAction;
    QAction *renderImage;
    QAction *renderAnimationAction;

};

#endif
