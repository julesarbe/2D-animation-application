#include <QtWidgets>

#include "mainwindow.h"
#include "drawingarea.h"

MainWindow::MainWindow()
{

//We set the current frame to the 1st one of the animation
    drawingArea = new DrawingArea;
    setCentralWidget(drawingArea);

    createActions();
    createMenus();
    createToolbar();
    createPlaybar();

    setWindowTitle(tr("Animation App"));

    resize(500, 500);
}
void MainWindow::createToolbar(){
   QToolBar* toolbar = addToolBar("main toolbar");
   toolbar->setFixedHeight(50);

   toolbar->addAction(penColorAction);
   toolbar->addAction(penWidthAction);

   toolbar->addSeparator();
   toolbar->addSeparator();

   toolbar->addAction(drawAction);
   toolbar->addAction(lineAction);
   toolbar->addAction(squareAction);
   toolbar->addAction(circleAction);

   toolbar->addSeparator();
   toolbar->addAction(eraseFrameAction);
   toolbar->addAction(eraseLastShapeAction);

   toolbar->addSeparator();
   toolbar->addSeparator();

   toolbar->addAction(penBackgroundAction);
   toolbar->addAction(eraseBackgroundAction);
}

void MainWindow::createPlaybar(){
    QToolBar* toolbar = addToolBar("main toolbar");
    addToolBar(Qt::BottomToolBarArea, toolbar);
    toolbar->setFixedHeight(50);

    toolbar->addSeparator();
    toolbar->addSeparator();

    toolbar->addAction(previousFrameAction);
    toolbar->addAction(playAction);
    toolbar->addAction(nextFrameAction);
}

void MainWindow::goToFrame(){
    QString actualframe=frame->text();
    drawingArea->goToFrame(actualframe.toInt());
}

void MainWindow::createActions()
{
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(tr("CTRL+O"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    renderAnimationAction = new QAction(tr("&Render Animation"), this);
    connect(renderAnimationAction, SIGNAL(triggered()), drawingArea, SLOT(renderAnimation()));
    renderImage = new QAction(tr("&Render Image"), this);
    connect(renderImage, SIGNAL(triggered()), drawingArea, SLOT(renderImage()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("CTRL+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    eraseFrameAction = new QAction(tr("&Erase The frame"), this);
    connect(eraseFrameAction, SIGNAL(triggered()), drawingArea, SLOT(eraseImage()));
    eraseFrameAction->setIcon(QIcon(":/image/Icons/Eraser.png"));

    eraseLastShapeAction = new QAction(tr("&Erase Last Shape"), this);
    connect(eraseLastShapeAction, SIGNAL(triggered()), drawingArea, SLOT(eraseLastShape()));
    eraseLastShapeAction->setIcon(QIcon(":/image/Icons/EraserBack.png"));

    eraseBackgroundAction = new QAction(tr("&Erase Background"), this);
    connect(eraseBackgroundAction, SIGNAL(triggered()), drawingArea, SLOT(eraseBackground()));
    eraseBackgroundAction->setIcon(QIcon(":/image/Icons/EraseBackground.png"));

    penColorAction = new QAction(tr("&Pen Color..."), this);
    connect(penColorAction, SIGNAL(triggered()), this, SLOT(penColor()));
    penColorAction->setIcon(QIcon(":/image/Icons/Color.png"));

    penBackgroundAction = new QAction(tr("&Paint Background"), this);
    connect(penBackgroundAction, SIGNAL(triggered()), drawingArea, SLOT(setBackground()));
    penBackgroundAction->setIcon(QIcon(":/image/Icons/Background.png"));

    penWidthAction = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAction, SIGNAL(triggered()), this, SLOT(penWidth()));
    penWidthAction->setIcon(QIcon(":/image/Icons/Width.png"));

    clearScreenAction = new QAction(tr("&Clear Screen"), this);
    clearScreenAction->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAction, SIGNAL(triggered()), drawingArea, SLOT(clearImage()));

    lineAction = new QAction(tr("&Line"), this);
    connect(lineAction, SIGNAL(triggered()), drawingArea, SLOT(lineMode()));
    lineAction->setIcon(QIcon(":/image/Icons/Line.png"));
    squareAction = new QAction(tr("&Square"), this);
    connect(squareAction, SIGNAL(triggered()), drawingArea, SLOT(squareMode()));
    squareAction->setIcon(QIcon(":/image/Icons/Square.png"));
    circleAction = new QAction(tr("Circle"), this);
    connect(circleAction, SIGNAL(triggered()), drawingArea, SLOT(circleMode()));
    circleAction->setIcon(QIcon(":/image/Icons/Circle.png"));
    drawAction = new QAction(tr("&Hand Drawing Mode"), this);
    connect(drawAction, SIGNAL(triggered()), drawingArea, SLOT(drawMode()));
    drawAction->setIcon(QIcon(":/image/Icons/Drawing.png"));

    nextFrameAction = new QAction(tr("Next Frame"), this);
    connect(nextFrameAction, SIGNAL(triggered()), drawingArea, SLOT(nextFrame()));
    nextFrameAction->setShortcut(tr("Right"));
    nextFrameAction->setIcon(QIcon(":/image/Icons/Next.png"));
    playAction = new QAction(tr("Play"), this);
    connect(playAction, SIGNAL(triggered()), drawingArea, SLOT(playMode()));
    playAction->setShortcut(tr("Down"));
    playAction->setIcon(QIcon(":/image/Icons/Play.png"));
    previousFrameAction = new QAction(tr("Previous Frame"), this);
    connect(previousFrameAction, SIGNAL(triggered()), drawingArea, SLOT(previousFrame()));
    previousFrameAction->setShortcut(tr("Left"));
    previousFrameAction->setIcon(QIcon(":/image/Icons/Previous.png"));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        // Create an action for each file format
        QAction *action = new QAction(text, this);

        // Set an action for each file format
        action->setData(format);

        // When clicked call MainWindow::save()
        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        saveAsActions.append(action);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), drawingArea, SLOT(playNextFrame()));
    timer->start(1000/7);
}

void MainWindow::createMenus()
{
    QMenuBar *menubar = this->menuBar();

    renderMenu = menubar->addMenu(tr("&Rendering"));
    renderMenu->addAction(renderAnimationAction);
    renderMenu->addAction(renderImage);

    saveAsMenu = menubar->addMenu(tr("&Save As"));
    foreach (QAction *action, saveAsActions)
        saveAsMenu->addAction(action);

    openMenu = menubar->addMenu(tr("&Open"));
    openMenu ->addAction(openAction);

    exitMenu = menubar->addMenu(tr("&Exit"));
    exitMenu ->addAction(exitAction);


}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            drawingArea->openImage(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());

    QByteArray fileFormat = action->data().toByteArray();

    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(drawingArea->penColor());

    if (newColor.isValid())
        drawingArea->setPenColor(newColor);
}

void MainWindow::penWidth()
{

    int newWidth = QInputDialog::getInt(this, tr("drawing"),
                                        tr("Select pen width:"),
                                        drawingArea->penWidth(),
                                        1, 50, 1);
    drawingArea->setPenWidth(newWidth);
}

bool MainWindow::maybeSave()
{
    if (drawingArea->isModified()) {
       QMessageBox::StandardButton ret;

       ret = QMessageBox::warning(this, tr("drawing"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveFile("png");

        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty()) {
        return false;
    } else {

        return drawingArea->saveImage(fileName, fileFormat.constData());
    }
}
