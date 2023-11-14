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
    connect(ui->selection, &QPushButton::clicked, this, &MainWindow::selectionButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeGridSize(){
    ui->graphicsCanvas->gridSizeChanged(ui->gridSizeSldr->value());
}

void MainWindow::eraseButtonClicked() {
    QPixmap eraserMap(":/eraser.png");
    eraserMap.setDevicePixelRatio(24);
    QCursor c = QCursor(eraserMap, 0, 0);
    setCursor(c);
    //calling the Eraserchange method for change the earser's status.
    //Active to be not active, or reverse.
    ui->graphicsCanvas->drawingMode(true);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->Eraserchange(true);
    ui->graphicsCanvas->selectionMode(false);
}

void MainWindow::drawButtonClicked()
{
    QPixmap penMap(":/pen.png");
    penMap.setDevicePixelRatio(24);
    QCursor c = QCursor(penMap, 0, 0);
    setCursor(c);
    ui->graphicsCanvas->drawingMode(true);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->Eraserchange(false);
    ui->graphicsCanvas->selectionMode(false);

}

void MainWindow::fillButtonClicked()
{
    QPixmap fillBucketMap(":/fillBucket.png");
    fillBucketMap.setDevicePixelRatio(18);
    QCursor c = QCursor(fillBucketMap, 0, 0);
    setCursor(c);
    ui->graphicsCanvas->drawingMode(false);
    ui->graphicsCanvas->fillMode(true);
    ui->graphicsCanvas->selectionMode(false);

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

void MainWindow::selectionButtonClicked()
{
    QPixmap handMap(":/hand.png");
    handMap.setDevicePixelRatio(18);
    QCursor c = QCursor(handMap, 0, 0);
    setCursor(c);
    ui->graphicsCanvas->drawingMode(false);
    ui->graphicsCanvas->fillMode(false);
    ui->graphicsCanvas->selectionMode(true);
}

