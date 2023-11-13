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
    currentGridDimension = startGridDimension;

    scene->setSceneRect(0, 0, startGridDimension * 49, startGridDimension * 49);

    this->setSceneRect(5, 5, scene->width(), scene->height());

    drawGrid(startGridDimension);
    //Other initialization code
}


void drawingCanvas::drawGrid(double gridDimension) {

    scaleFactor = this->width() / gridDimension;
    currentGridDimension = gridDimension;

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
    toolActive = true;
    if (drawActive)
    {
        drawOnGrid(event->pos());
    }

    if (fillActive)
    {
        QPointF fillPoint = mapToScene(event->pos());
        fillBucket(fillPoint, 0, 0);
    }

    if (selectionActive)
    {
        moving = true;
        lastMousePosition = mapToScene(event->pos());
        delta.setX(0);
        delta.setY(0);
    }
}

// mouse drag
void drawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (toolActive && drawActive) {
        drawOnGrid(event->pos());
    }

    if (toolActive && fillActive)
    {
        QPointF fillPoint = mapToScene(event->pos());
        fillBucket(fillPoint, 0, 0);
    }

    if (toolActive && selectionActive)
    {
        if (moving)
        {
            delta = mapToScene(event->pos()) - lastMousePosition;
            qDebug()<<"last mouse: "<<lastMousePosition;
            qDebug()<<"current mouse: "<<mapToScene(event->pos());
            qDebug()<<"change: "<<delta;
            //lastMousePosition = mapToScene(event->pos());
        }
    }
}
// mouse release
void drawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    toolActive = false;

    if (moving)
    {
        moving = false;
        movePixels(delta);
    }
}

void drawingCanvas::Eraserchange(bool state) {
    //change the erase active status, when click the erase button
    eraseActive = state;
}

void drawingCanvas::drawingMode(bool state) {
    //change the erase active status, when click the erase button
    drawActive = state;
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

void drawingCanvas::selectionMode(bool state)
{
    selectionActive = state;
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

void drawingCanvas::movePixels(QPointF delta)
{
//    for (int x = 0; x <= gridDimension; x++) {
//        for (int y = 0; y <= gridDimension; y++) {
//            scene->addRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor, pen, brush);
//        }
//    }
    for (int x = 0; x <= currentGridDimension; x++) {
        for (int y = 0; y <= currentGridDimension; y++) {
            QPointF coor;
            coor.setX(x * scaleFactor);
            coor.setY(y * scaleFactor);
            QPointF change = coor + delta;
            QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(coor, QTransform()));
            if (change.x() >= scaleFactor || change.y() >= scaleFactor)
            {
                QGraphicsRectItem *movedGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(change, QTransform()));
                if (change.x() < 0 || change.x() >= 500 || change.y() < 0 || change.y() >= 500)
                {

                }
                else
                {
                    movedGrid->setBrush(currentGrid->brush().color());
                }
            }
        }
    }
}
