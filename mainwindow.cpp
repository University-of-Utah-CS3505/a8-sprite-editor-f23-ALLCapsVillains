#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <drawingCanvas.h>

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
    //QPixmap dmap("C:/Users/Samuel/Downloads/pen.png");
    //QPixmap emap("C:/Users/Samuel/Downloads/pen.png");
    //QPixmap fmap("C:/Users/Samuel/Downloads/pen.png");
    //QPixmap smap("C:/Users/Samuel/Downloads/pen.png");
    //QIcon ButtonIcon(dmap);
    //ui->draw->setIcon(ButtonIcon);
    //ui->erase->setIcon(ButtonIcon);
    //ui->fill->setIcon(ButtonIcon);
    //ui->selection->setIcon(ButtonIcon);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeGridSize(){
    ui->graphicsCanvas->gridSizeChanged(ui->gridSizeSldr->value());
}
