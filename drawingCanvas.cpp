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
        fillBucket(event->pos());
    }
}

// mouse drag
void drawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (drawActive && drawMode) {
        drawOnGrid(event->pos());
        qDebug()<<"draw";
    }
    if (drawActive && fillActive)
    {
        fillBucket(event->pos());
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
    //qDebug()<<position;

    //find the current the grid we chosed and change its color
    QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));
    if (currentGrid) {
        //change its color transparent or black based on if on erasing
        if (eraseActive)
        {
            color = Qt::transparent;
        }
        else
        {
            color = colorPrev;
        }
        currentGrid->setBrush(QBrush(color));
    }
}

void drawingCanvas::fillBucket(const QPoint &position)
{
//    if (x < 0 || x >= image.width() || y < 0 || y >= image.height())
//        return;
//    if (image.pixelColor(x, y) != oldColor)
//        return;
    QPointF scenePoint = mapToScene(position);
    QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));
    qDebug()<<currentGrid.item
    if (currentGrid)
    {
        currentGrid->setBrush(QBrush(color));
    }

    scenePoint.setX(scenePoint.x() + scaleFactor);
    QGraphicsRectItem *currentGriddy = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));
    currentGriddy->setBrush(QBrush(color));

//    fillBucket(image, x + 1, y, oldColor, newColor);
//    fillBucket(image, x - 1, y, oldColor, newColor);
//    fillBucket(image, x, y + 1, oldColor, newColor);
//    fillBucket(image, x, y - 1, oldColor, newColor);
}
