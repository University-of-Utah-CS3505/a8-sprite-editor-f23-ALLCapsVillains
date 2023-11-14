#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <drawingCanvas.h>
#include<QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu * menu = menuBar()->addMenu("File");


    QWidget *contentWidget = ui->scrollArea->widget();

    // set the scrollarea's widget to be QVbox layout for vertiaclly setting the frames
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    contentWidget->setLayout(layout);

    // then add the first frame to that layout correctly
    layout->addWidget(ui->frame);
    ui->frame->setFixedSize(ui->frame->size());
    ui->frame->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    //add the first frame to the frame list and current index for it
    framesViewsList.append(ui->frame);
    currentFrameIndex = 0;


    menu->addAction("Save");
    menu->addAction("Save As");
    menu->addAction("Load");

    menu=menuBar()->addMenu("Presets");
    menu->addAction("Insert Preset 1");
    menu->addAction("Insert Preset 2");
    menu->addAction("Insert Preset 3");
    menu->addAction("Insert Preset 4");
    menu->addAction("Insert Preset 5");

    menu=menuBar()->addMenu("Theme");
    menu->addAction("Dark Mode");
    menu->addAction("Light Mode");
    menu->addAction("Light Blue");
    menu->addAction("Mocha");
    menu->addAction("Rojo");


    connect(ui->gridSizeSldr, &QSlider::valueChanged, this, &MainWindow::changeGridSize);


    //Pixmaps for our button Icons, (Currently using local images until we figure out Resource File.
    QPixmap penMap(":/pen.png");
    QPixmap eraserMap(":/eraser.png");
    QPixmap fillBucketMap(":/fillBucket.png");
    QPixmap handMap(":/hand.png");

    QIcon penIcon(penMap);
    ui->draw->setIcon(penIcon);

    QIcon eraserIcon(eraserMap);
    ui->erase->setIcon(eraserIcon);

    QIcon fillBucketIcon(fillBucketMap);
    ui->fill->setIcon(fillBucketIcon);

    QIcon handIcon(handMap);
    ui->selection->setIcon(handIcon);

    ui->colorBtn->setStyleSheet("background-color: black"); // default color to black

    connect(ui->erase, &QPushButton::clicked, this, &MainWindow::eraseButtonClicked);
    connect(ui->draw, &QPushButton::clicked, this, &MainWindow::drawButtonClicked);
    connect(ui->fill, &QPushButton::clicked, this, &MainWindow::fillButtonClicked);
    connect(ui->colorBtn, &QPushButton::clicked, this, &MainWindow::colorButtonClicked);

    //when the drawing is finished, the preview window will show the scene
    connect(ui->graphicsCanvas, &drawingCanvas::drawingFinish, this, &MainWindow::frameUpdate);
    connect(ui->graphicsCanvas, &drawingCanvas::updatePreviewWindow, this, &MainWindow::previewWindowUpdate);

    // setting the timer for animation
    previewAnimationTimer = new QTimer(this);
    connect(previewAnimationTimer, &QTimer::timeout, this, &MainWindow::previewWindowUpdate);

    connect(ui->slider, &QSlider::valueChanged, this, &MainWindow::fpsChanged);

    connect(ui->spinBox, &QSpinBox::valueChanged, this, &MainWindow::on_spinBox_valueChanged);

}

MainWindow::~MainWindow()
{
    if (previewAnimationTimer->isActive()) {
        previewAnimationTimer->stop();
    }
    delete previewAnimationTimer;
    delete ui;
}

void MainWindow::changeGridSize(){
    ui->graphicsCanvas->gridSizeChanged(ui->gridSizeSldr->value());
}

void MainWindow::eraseButtonClicked() {
    //calling the Eraserchange method for change the earser's status.
    //Active to be not active, or reverse.
    ui->graphicsCanvas->drawingMode(true);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->Eraserchange(true);
}

void MainWindow::drawButtonClicked()
{
    ui->graphicsCanvas->drawingMode(true);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->Eraserchange(false);
}

void MainWindow::fillButtonClicked()
{
    ui->graphicsCanvas->drawingMode(false);
    ui->graphicsCanvas->fillMode(true);
}

void MainWindow::colorButtonClicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
    if (color.isValid())
    {
        ui->graphicsCanvas->colorChange(color);

        // Change the icon to selected color
        QString buttonColor = QString("background-color: %1").arg(color.name());
        ui->colorBtn->setStyleSheet(buttonColor);
    }
}

void MainWindow::previewWindowUpdate() {
    //check if frames list is empty
    if (ui->previewWindow && !framesViewsList.isEmpty()) {
        // gettting the scene from current frame(graphicsview)
        QGraphicsScene* currentScene = framesViewsList[currentFrameIndex]->scene();

        // make the scene to be a Qpixmap as an image to use
        QPixmap pixmap(currentScene->sceneRect().size().toSize());
        QPainter painter(&pixmap);
        currentScene->render(&painter);

        // Scale the Qpixmap to fit the previw window's size
        QPixmap scaledPixmap = pixmap.scaled(ui->previewWindow->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QGraphicsScene *previewScene = ui->previewWindow->scene();
        if (!previewScene) {
            previewScene = new QGraphicsScene(ui->previewWindow);
            ui->previewWindow->setScene(previewScene);
        }
        //clear it for repopulating it with a new pixmap
        previewScene->clear();

        // Adding the scaled pixmap to the preview window scene properly
        previewScene->addPixmap(scaledPixmap);
        ui->previewWindow->fitInView(previewScene->itemsBoundingRect(), Qt::KeepAspectRatio);

        //Moving to the next frame
        currentFrameIndex = (currentFrameIndex + 1) % framesViewsList.size();
    }
}

//updating the frames part by adding or deleting frames
void MainWindow::frameUpdate(int index) {
    //check if drawing and the current frame are valid
    if (ui->graphicsCanvas && framesViewsList[index]) {
        // Capture contents and the rectangle boundaries of the drawing canvas
        QGraphicsScene* drawingScene = ui->graphicsCanvas->getScene();
        frames[currentFrameIndex] = drawingScene;
        QRectF sceneRectBound = drawingScene->sceneRect();

        //drawing the drawing scene onto the pixmap for using
        QPixmap pixmap(sceneRectBound.size().toSize());
        QPainter painter(&pixmap);
        //make all white to clean all grid first
        painter.fillRect(pixmap.rect(), Qt::white);
        drawingScene->render(&painter, QRectF(), sceneRectBound);

        QPixmap framePixmap = pixmap.scaled(framesViewsList[index]->viewport()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsScene *frameScene = framesViewsList[index]->scene();

        if (!frameScene) {
            frameScene = new QGraphicsScene(framesViewsList[index]);
            framesViewsList[index]->setScene(frameScene);
        }
        //clear it for repopulating it with a new pixmap
        frameScene->clear();

        // Adding the scaled pixmap to the frame scene
        frameScene->addPixmap(framePixmap);

        // Fiting to the current frame
        framesViewsList[index]->fitInView(frameScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_addFrame_clicked()
{
    currentFrameIndex++;
    //call the addNewFrame method in graphicsCanvs
    ui->graphicsCanvas->addNewFrame();

    //create a new graphicsview as the new frame
    QGraphicsView* newFrameView = new QGraphicsView();

    // Set the new frame has same size and scene stuffs as the first frame.
    newFrameView->setFixedSize(ui->frame->size());
    QGraphicsScene* newFrameScene = new QGraphicsScene(newFrameView);
    newFrameView->setScene(newFrameScene);
    newFrameScene->setSceneRect(ui->frame->sceneRect());

    // add this new frame to the UI(scroll view under the previous frames) properlly
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    layout->addWidget(newFrameView);
    newFrameView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    //add this new frame to the frames list and update current frame index
    framesViewsList.append(newFrameView);

    //Only has 2 or more frames to start the animation timer
    if (framesViewsList.size() == 2) {
        //1 second for each
        previewAnimationTimer->start(1000);
        ui->fpsLabel->setText("FPS : 1");
        ui->slider->setValue(1);
    }

}

void MainWindow::fpsChanged(int fps){
    if(fps == 0){
        previewAnimationTimer->stop();
        ui->fpsLabel->setText("FPS : 0");
    }else{
        previewAnimationTimer->start(1000/fps);
        ui->fpsLabel->setText("FPS : " + QString::number(fps));

    }

}

void MainWindow::on_deleteFrame_clicked()
{
    if (framesViewsList.size() <= 1) {
        // Optionally, clear the last frame instead of deleting it
        return;
    }

    // Assuming currentFrameIndex is the index of the frame to delete
    QGraphicsView* frameToDelete = framesViewsList.at(currentFrameIndex);

    // Remove the frame from the UI and delete it
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    layout->removeWidget(frameToDelete);
    framesViewsList.removeAt(currentFrameIndex);
    delete frameToDelete;

    // Adjust the currentFrameIndex to the previous frame
    currentFrameIndex = qMax(0, currentFrameIndex - 1);

}

void MainWindow::on_spinBox_valueChanged(int value)
{
    //ui->graphicsCanvas->setScene(frames[value]);
    QGraphicsScene *scene = frames[value];
    ui->graphicsCanvas->setScene(scene);
}

