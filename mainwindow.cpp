/*
 *Main Window View Class
 *Created by Alex Qi, Matthew Goh, Sam Onwukeme, Yujie He, Jake Crane,  ZengZheng Jiang
 *Assignment 8: Sprite Editor (ZFX 1.0)
 *
 *Style Reviewed by Mario.
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>

#include <QTimer>
#include <QFileDialog>
#include <QPixmap>
#include<QMovie>
#include <QRect>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow){
    ui->setupUi(this);

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
    currentFrameIndex = frame;

    QMenu * menu = menuBar()->addMenu("File");
    QAction *loadAction = menu->addAction("Load");
    QAction *saveAction = menu->addAction("Save");

    ui->addFrame->setDisabled(true);
    ui->deleteFrame->setDisabled(true);

    menu=menuBar()->addMenu("Theme");

    QAction* dark = menu->addAction("Dark");
    QAction* rojo = menu->addAction("Rojo");
    QAction* sky = menu->addAction("Sky");
    QAction* mocha = menu->addAction("Mocha");


    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


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
    connect(ui->selection, &QPushButton::clicked, this, &MainWindow::selectionButtonClicked);


    //when the drawing is finished, the preview window will show the scene
    connect(ui->graphicsCanvas, &drawingCanvas::drawingFinish, this, &MainWindow::frameUpdate);
    connect(ui->graphicsCanvas, &drawingCanvas::updatePreviewWindow, this, &MainWindow::previewWindowUpdate);

    // setting the timer for animation
    previewAnimationTimer = new QTimer(this);
    connect(previewAnimationTimer, &QTimer::timeout, this, &MainWindow::previewWindowUpdate);

    connect(ui->slider, &QSlider::valueChanged, this, &MainWindow::fpsChanged);

    connect(saveAction, &QAction::triggered, this, &MainWindow::saveDrawing);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadDrawing);

    connect(rojo, &QAction::triggered,this,&MainWindow::rojoTheme);
    connect(dark, &QAction::triggered, this, &MainWindow::darkTheme);
    connect(mocha, &QAction::triggered, this,&MainWindow::mochaTheme);
    connect(sky, &QAction::triggered, this, &MainWindow::skyTheme);
    firstFPS = true;

    connect(ui->graphicsCanvas, &drawingCanvas::cleanFrame, this, &MainWindow::cleanFrames);
    connect(ui->graphicsCanvas, &drawingCanvas::addFrame, this, &MainWindow::saveFrame);
}

MainWindow::~MainWindow(){
    if (previewAnimationTimer->isActive()) {
        previewAnimationTimer->stop();
    }
    delete previewAnimationTimer;
    delete ui;
}

void MainWindow::changeGridSize(){
    ui->graphicsCanvas->gridSizeChanged(ui->gridSizeSldr->value());
}

void MainWindow::eraseButtonClicked(){
    QPixmap iconMap(":/eraser.png");
    QIcon icon(iconMap);
    ui->currentTool->setIcon(icon);


    //calling the Eraserchange method for change the earser's status.
    //Active to be not active, or reverse.
    ui->graphicsCanvas->drawingMode(true);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->eraserChange(true);
    ui->graphicsCanvas->selectionMode(false);
}

void MainWindow::drawButtonClicked(){
    QPixmap iconMap(":/pen.png");
    QIcon icon(iconMap);
    ui->currentTool->setIcon(icon);
    ui->graphicsCanvas->drawingMode(true);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->eraserChange(false);
    ui->graphicsCanvas->selectionMode(false);
}

void MainWindow::fillButtonClicked()
{
    QPixmap iconMap(":/fillBucket.png");
    QIcon icon(iconMap);
    ui->currentTool->setIcon(icon);
    ui->graphicsCanvas->drawingMode(false);
    ui->graphicsCanvas->fillMode(true);
    ui->graphicsCanvas->selectionMode(false);
}

void MainWindow::colorButtonClicked(){
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
    if (color.isValid())
    {
        ui->graphicsCanvas->colorChange(color);

        // Change the icon to selected color
        QString buttonColor = QString("background-color: %1").arg(color.name());
        ui->colorBtn->setStyleSheet(buttonColor);
    }
}

void MainWindow::selectionButtonClicked(){
    QPixmap iconMap(":/hand.png");
    QIcon icon(iconMap);
    ui->currentTool->setIcon(icon);
    ui->graphicsCanvas->drawingMode(false);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->selectionMode(true);
}

void MainWindow::cursorButtonClicked()
{
    QCursor c;
    c.setShape(Qt::ArrowCursor);
    setCursor(c);
    ui->graphicsCanvas->drawingMode(false);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->selectionMode(false);
}

void MainWindow::saveDrawing() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Drawing"), "", tr("Sprite Sheet Project Files (*.ssp)"));
    if (!filePath.isEmpty()) {
        ui->graphicsCanvas->saveDrawing(filePath);
    }
}

void MainWindow::loadDrawing() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Drawing"), "", tr("Sprite Sheet Project Files (*.ssp)"));
    if (!filePath.isEmpty()) {
        ui->graphicsCanvas->loadDrawing(filePath);
    }
}

void MainWindow::rojoTheme(){
    ui->centralwidget->setStyleSheet("background-color: rgb(212, 71, 61)");
    ui->scrollArea->setStyleSheet("background-color: rgb(255,255,255)");
    ui->draw->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->erase->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->fill->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->selection->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->addFrame->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->deleteFrame->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->previewWindow->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->graphicsCanvas->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->currentTool->setStyleSheet("background-color: rgb(245, 127, 213)");
    ui->framePicker->setStyleSheet("background-color: rgb(255, 255, 255)");
    QMovie *mov = new QMovie(":/rojoooo.gif");
    mov->start();
    mov->setScaledSize(ui->themePic->size());
    ui->themePic->setMovie(mov);
}

void MainWindow::mochaTheme(){
    ui->centralwidget->setStyleSheet("background-color: rgb(130, 105, 77)");
    ui->scrollArea->setStyleSheet("background-color: rgb(255,255,255)");
    ui->draw->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->erase->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->fill->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->selection->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->addFrame->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->deleteFrame->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->previewWindow->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->graphicsCanvas->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->currentTool->setStyleSheet("background-color: rgb(219, 209, 156)");
    ui->framePicker->setStyleSheet("background-color: rgb(255, 255, 255)");
    int w = ui->themePic->width();
    int h = ui->themePic->height();
    QPixmap pix(":/cuofeebg.png");
    ui->themePic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::darkTheme(){
    ui->centralwidget->setStyleSheet("background-color: rgb(1, 13, 31)");
    ui->scrollArea->setStyleSheet("background-color: rgb(255,255,255)");
    ui->draw->setStyleSheet("background-color: rgb(219, 18, 255)");
    ui->erase->setStyleSheet("background-color: rgb(18, 255, 105)");
    ui->fill->setStyleSheet("background-color: rgb(219, 18, 255)");
    ui->selection->setStyleSheet("background-color: rgb(18, 255, 105)");
    ui->addFrame->setStyleSheet("background-color: rgb(219, 18, 255)");
    ui->deleteFrame->setStyleSheet("background-color: rgb(18, 255, 105)");
    ui->previewWindow->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->graphicsCanvas->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->currentTool->setStyleSheet("background-color: rgb(219, 18, 255)");
    ui->framePicker->setStyleSheet("background-color: rgb(255, 255, 255)");

    QMovie *mov = new QMovie(":/cybaaa.gif");
    mov->start();
    mov->setScaledSize(ui->themePic->size());
    ui->themePic->setMovie(mov);
}

void MainWindow::skyTheme(){
    ui->centralwidget->setStyleSheet("background-color: rgb(190, 250, 250)");
    ui->scrollArea->setStyleSheet("background-color: rgb(255,255,255)");
    ui->draw->setStyleSheet("background-color: rgb(255,255,255)");
    ui->erase->setStyleSheet("background-color: rgb(255,255,255)");
    ui->fill->setStyleSheet("background-color: rgb(255,255,255)");
    ui->selection->setStyleSheet("background-color: rgb(255,255,255)");
    ui->addFrame->setStyleSheet("background-color: rgb(255,255,255)");
    ui->deleteFrame->setStyleSheet("background-color: rgb(255,255,255)");
    ui->previewWindow->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->graphicsCanvas->setStyleSheet("background-color: rgb(255, 255, 255)");
    ui->currentTool->setStyleSheet("background-color: rgb(255,255,255)");
    ui->framePicker->setStyleSheet("background-color: rgb(255, 255, 255)");
    QMovie *mov = new QMovie(":/kratoss.gif");
    mov->start();
    mov->setScaledSize(ui->themePic->size());

    ui->themePic->setMovie(mov);
    ui->themePic->setStyleSheet("(QLabel{border-radius: 25px;}");
}

void MainWindow::on_framePicker_valueChanged(int arg1){
    if(arg1 >=0 && arg1 <=ui->graphicsCanvas->numberOfFrames)
        ui->graphicsCanvas->framePick(arg1);
    currentFrameIndex = arg1;

}

void MainWindow::previewWindowUpdate() {
    //check if frames list is empty

    static int animationIndex = currentFrameIndex; // Static variable to remember the animation frame between timer calls

    // Check if the frames list is empty
    if (!framesViewsList.isEmpty() && animationIndex >= 0 && animationIndex < framesViewsList.count()) {
        // Get the current scene for animation
        QGraphicsScene* currentScene = framesViewsList[animationIndex]->scene();

        // Update the preview window with the new scene
        ui->previewWindow->setScene(currentScene);
        ui->previewWindow->fitInView(currentScene->itemsBoundingRect(), Qt::KeepAspectRatio);

        // Increment the animationIndex for the next frame, wrapping around if necessary
        animationIndex = (animationIndex + 1) % framesViewsList.size();
    }
}

//updating the frames part by adding or deleting frames
void MainWindow::frameUpdate() {
    ui->addFrame->setDisabled(false);
    //check if drawing and the current frame are valid
    if (ui->graphicsCanvas && framesViewsList[currentFrameIndex]) {
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

        QPixmap framePixmap = pixmap.scaled(framesViewsList[currentFrameIndex]->viewport()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsScene *frameScene = framesViewsList[currentFrameIndex]->scene();

        if (!frameScene) {
            frameScene = new QGraphicsScene(framesViewsList[currentFrameIndex]);
            framesViewsList[currentFrameIndex]->setScene(frameScene);
        }
        //clear it for repopulating it with a new pixmap
        frameScene->clear();

        // Adding the scaled pixmap to the frame scene
        frameScene->addPixmap(framePixmap);

        // Fiting to the current frame
        framesViewsList[currentFrameIndex]->fitInView(frameScene->itemsBoundingRect(), Qt::KeepAspectRatio);
        if (firstFPS) {
            //1 second for each
            previewAnimationTimer->start(1000);
            ui->fpsLabel->setText("FPS : 1");
            ui->slider->setValue(1);
            ui->addFrame->setDisabled(false);
            ui->deleteFrame->setDisabled(false);
            ui->gridSizeSldr->setDisabled(true);
            firstFPS = false;
        }
    }
}

void MainWindow::on_addFrame_clicked(){
    frame++;
    ui->graphicsCanvas->newFrame();
    ui->graphicsCanvas->frameChanged(frame);
    ui->framePicker->setMaximum(frame);
    ui->framePicker->setValue(frame);


    //currentFrameIndex = frame;
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

void MainWindow::on_deleteFrame_clicked(){
    // can not delete the last frame
    if(framesViewsList.size() <= 1) {
        return;
    }

    // ensure frame  is valid
    if(frame >= 0 && frame < framesViewsList.size()) {
        QGraphicsView* frameToDelete = framesViewsList.at(frame);

        // If the frame to delete is the one currently being displayed
        if(frame == currentFrameIndex) {
            // Need to display another frame before deletion
            currentFrameIndex = (currentFrameIndex - 1 + framesViewsList.size()) % framesViewsList.size();
            ui->graphicsCanvas->framePick(currentFrameIndex);
            ui->framePicker->setValue(currentFrameIndex);
        }

        // Remove the frame from the UI and delete it
        QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
        layout->removeWidget(frameToDelete);
        framesViewsList.removeAt(frame);
        delete frameToDelete;


        frame = qMax(0, frame - 1);

        // Update the UI to reflect the new state
        ui->graphicsCanvas->framePick(currentFrameIndex);
        ui->framePicker->setValue(currentFrameIndex);
    } else {
        qDebug() << "Invalid frame index";
    }
}

void MainWindow::on_spinBox_valueChanged(int value)
{
    //ui->graphicsCanvas->setScene(frames[value]);
    QGraphicsScene *scene = frames[value];
    ui->graphicsCanvas->setScene(scene);
    currentFrameIndex = value;
    frame = value;
}

// clean the frame number
void MainWindow::cleanFrames(){
    while(framesViewsList.size() > 1){
        on_deleteFrame_clicked();
    }
}

void MainWindow::saveFrame(){
    frame ++;

    ui->framePicker->setMaximum(frame);

    // 创建新的 QGraphicsView 作为新的 frame
    QGraphicsView* newFrameView = new QGraphicsView();

    // 设置新 frame 的大小和场景
    newFrameView->setFixedSize(ui->frame->size());
    QGraphicsScene* newFrameScene = new QGraphicsScene(newFrameView);
    newFrameView->setScene(newFrameScene);
    newFrameScene->setSceneRect(ui->frame->sceneRect());

    // 将这个新 frame 添加到 UI 的适当位置（例如滚动视图下方）
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->scrollAreaWidgetContents->layout());
    layout->addWidget(newFrameView);
    newFrameView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // 将新 frame 添加到 frames 列表并更新当前 frame 索引
    framesViewsList.append(newFrameView);
}
