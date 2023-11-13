#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <drawingCanvas.h>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMenu * menu = menuBar()->addMenu("File");

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
    connect(ui->graphicsCanvas, &drawingCanvas::drawingFinish, this, &MainWindow::previewWindowUpdate);
}

MainWindow::~MainWindow()
{
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
    //check if drawing and preview window are valid
    if (ui->graphicsCanvas && ui->previewWindow && ui->frame) {
        // Capture contents and the rectangle boundaries of the drawing canvas
        QGraphicsScene* drawingScene = ui->graphicsCanvas->getScene();
        QRectF sceneRectBound = drawingScene->sceneRect();

        //drawing the drawing scene onto the pixmap for using
        QPixmap pixmap(sceneRectBound.size().toSize());
        QPainter painter(&pixmap);
        //make all white to clean all grid first
        painter.fillRect(pixmap.rect(), Qt::white);
        drawingScene->render(&painter, QRectF(), sceneRectBound);

        // smaller the pixmap to fit the preview window size
        QPixmap previewPixmap = pixmap.scaled(ui->previewWindow->viewport()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsScene *previewScene = ui->previewWindow->scene();

        QPixmap framePixmap = pixmap.scaled(ui->frame->viewport()->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsScene *frameScene = ui->frame->scene();

        //check if the preview window's scene has been set or not
        if (!previewScene) {
            previewScene = new QGraphicsScene(ui->previewWindow);
            ui->previewWindow->setScene(previewScene);
        }

        if (!frameScene) {
            frameScene = new QGraphicsScene(ui->frame);
            ui->frame->setScene(frameScene);
        }
        //clear it for repopulating it with a new pixmap
        previewScene->clear();
        frameScene->clear();

        // Adding the scaled pixmap to the preview window scene
        frameScene->addPixmap(framePixmap);
        previewScene->addPixmap(framePixmap);

        // Fiting to the preview window
        ui->previewWindow->fitInView(previewScene->itemsBoundingRect(), Qt::KeepAspectRatio);
        ui->frame->fitInView(frameScene->itemsBoundingRect(), Qt::KeepAspectRatio);
    }
}

