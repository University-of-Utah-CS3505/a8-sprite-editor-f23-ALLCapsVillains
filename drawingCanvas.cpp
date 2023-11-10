#include "drawingCanvas.h"

drawingCanvas::drawingCanvas(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setFixedWidth(500);
    this->setFixedHeight(500);

    this->setScene(scene);

    this->invalidateScene();

    double startGridDimension = 10;

    scene->setSceneRect(0, 0, startGridDimension * 49, startGridDimension * 49);

    this->setSceneRect(5, 5, scene->width(), scene->height());

    drawGrid(startGridDimension);
    //Other initialization code
}


void drawingCanvas::drawGrid(double gridDimension) {

    double scaleFactor = this->width() / gridDimension;
    // Clear the previous grid if needed
    scene->clear();

    QPen pen(Qt::gray);
    pen.setWidth(0); // Set pen width to 0 to draw lines one pixel wide
    QBrush brush(Qt::black); // Brush to fill the cells

    // Loop over each cell in the grid and fill it
    for (int x = 0; x <= gridDimension; x++) {
        for (int y = 0; y <= gridDimension; y++) {
            scene->addRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor, pen, brush);
        }
    }
}

void drawingCanvas::gridSizeChanged(int newSize) {
    drawGrid(newSize);
}
