#include "drawingCanvas.h"
#include <QGraphicsItem>

#include<QDebug>

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

    scaleFactor = this->width() / gridDimension;
    qDebug()<<scaleFactor;
    // Clear the old grids
    scene->clear();

    QPen pen(Qt::gray);
    pen.setWidth(0);
    QBrush brush(Qt::transparent);

    //filling all grids
    for (int x = 0; x <= gridDimension; x++) {
        for (int y = 0; y <= gridDimension; y++) {
            scene->addRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor, pen, brush);
        }
    }
}

void drawingCanvas::gridSizeChanged(int newSize) {
    drawGrid(newSize);
}

// mouse clic
void drawingCanvas::mousePressEvent(QMouseEvent *event) {
    drawActive = true;
    if (drawMode)
    {
        drawOnGrid(event->pos());
    }

    if (fillActive)
    {
        QPointF fillPoint = mapToScene(event->pos());
        fillBucket(fillPoint, 0, 0);
    }
}

// mouse drag
void drawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (drawActive && drawMode) {
        drawOnGrid(event->pos());
    }
    if (drawActive && fillActive)
    {
        QPointF fillPoint = mapToScene(event->pos());
        fillBucket(fillPoint, 0, 0);
    }
}

// mouse release
void drawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    drawActive = false;
}

void drawingCanvas::Eraserchange(bool state) {
    //change the erase active status, when click the erase button
    eraseActive = state;
}

void drawingCanvas::drawingMode(bool state) {
    //change the erase active status, when click the erase button
    drawMode = state;
}

void drawingCanvas::fillMode(bool state) {
    //change the erase active status, when click the erase button
    fillActive = state;
}

void drawingCanvas::colorChange(QColor newColor)
{
    colorPrev = newColor;
    color = newColor;
}

// draw colors on the grids
void drawingCanvas::drawOnGrid(const QPoint &position) {
    //find the view coordinate corresponding to the mouse location
    QPointF scenePoint = mapToScene(position);

    //find the current the grid we chosed and change its color
    QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));
    if (currentGrid) {
        //change its color transparent or previous color based on if erase is active
        if (eraseActive)
        {
            color = Qt::transparent;
        }
        else
        {
            // Go back to the color selected by the user before they selected erase button
            color = colorPrev;
        }
        currentGrid->setBrush(QBrush(color));
    }
}

void drawingCanvas::fillBucket(QPointF scenePoint, int scaleX, int scaleY)
{
    // Select the square above/below or left/right to check and then fill if passes the checks
    scenePoint.setX(scenePoint.x() + scaleX);
    scenePoint.setY(scenePoint.y() + scaleY);
    QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));

    // If the cooridnates are out of range of the graphics canvas, return
    if (scenePoint.x() < 0 || scenePoint.x() >= 500 || scenePoint.y() < 0 || scenePoint.y() >= 500)
        return;

    // If you reach a color that matches the selected color, return
    if (currentGrid->brush().color() != Qt::transparent)
    {
        return;
    }

    // Set the space of the grid to the color
    if (currentGrid)
    {
        currentGrid->setBrush(QBrush(colorPrev));
    }

    // Flood fill algorithm to recurse on all sides
    fillBucket(scenePoint, scaleFactor, 0);
    fillBucket(scenePoint, -scaleFactor, 0);
    fillBucket(scenePoint, 0, scaleFactor);
    fillBucket(scenePoint, 0, -scaleFactor);
}
