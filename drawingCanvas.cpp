#include "drawingCanvas.h"
#include <QGraphicsItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include<QDebug>

drawingCanvas::drawingCanvas(QWidget *parent) : QGraphicsView(parent), gridItem(nullptr) {
    scene = new QGraphicsScene(this);

    pen = QPen(Qt::gray);
    pen.setWidth(0);
    brush = QBrush(Qt::transparent);

    gridItem = new GridItem(gridDimension);
    scene->addItem(gridItem);

    currentFrameIndex = 0;

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setFixedWidth(500);
    this->setFixedHeight(500);

    this->setScene(scene);

    this->invalidateScene();

    gridDimension = 10;

    scene->setSceneRect(0, 0, gridDimension * 49, gridDimension * 49);

    this->setSceneRect(5, 5, scene->width(), scene->height());

    drawGrid(gridDimension);
    //sam
    frames.insert(0,scene);
}


void drawingCanvas::newFrame(){
    frame++;
    QGraphicsScene* frameAdded = new QGraphicsScene(this);
    frames.insert(frame,frameAdded);
}

void drawingCanvas::frameChanged(int i){
    this->setScene(frames.value(i));
    scene = frames.value(i);
    drawGrid(gridDimension);

}
void drawingCanvas::framePick(int i){
    this->setScene(frames.value(i));
    scene = frames.value(i);

}

void drawingCanvas::deleteFrame(int i){
    if(frames.count() > 0){
        frame--;

    this->setScene(frames.value(i-1));
    scene = frames.value(i-1);

    }

}

void drawingCanvas::drawGrid(double newGridDimension) {
    // Calculate the new scale factor based on the new grid dimension
    double newScaleFactor = this->width() / newGridDimension;

    // Clear the current grid from the scene
    scene->clear();

    // Create the new grid based on the new grid dimension
    for (int x = 0; x < newGridDimension; ++x) {
        for (int y = 0; y < newGridDimension; ++y) {
            scene->addRect(x * newScaleFactor, y * newScaleFactor, newScaleFactor, newScaleFactor, pen, brush);
        }
    }

    // Use the full-resolution drawing state to redraw the visible cells
    QHash<QPoint, QColor> visibleDrawingState; // Stores the visible state after resizing
    for (const QPoint &point : fullResolutionDrawingState.keys()) {
        int scaledX = static_cast<int>(point.x() * (newScaleFactor / scaleFactor));
        int scaledY = static_cast<int>(point.y() * (newScaleFactor / scaleFactor));

        if (scaledX < newGridDimension && scaledY < newGridDimension) {
            // The cell is visible in the new grid
            visibleDrawingState[QPoint(scaledX, scaledY)] = fullResolutionDrawingState[point];

    QPen pen(Qt::gray);
    pen.setWidth(0);
    QBrush brush(Qt::transparent);

            QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scaledX * newScaleFactor, scaledY * newScaleFactor, QTransform()));
            if (rect) {
                rect->setBrush(QBrush(fullResolutionDrawingState[point]));
            }
        }
    }


    // Update the visible drawing state
    drawingState = visibleDrawingState;
    scaleFactor = newScaleFactor; // Update the scale factor
    gridDimension = newGridDimension; // Update the grid dimension
}

void drawingCanvas::gridSizeChanged(int newSize) {
    gridDimension = newSize;

    drawGrid(gridDimension);
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
    emit drawingFinish(currentFrameIndex);
    emit updatePreviewWindow();
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
    localColor = newColor;
}

// draw colors on the grids
void drawingCanvas::drawOnGrid(const QPoint &position) {
    QPointF scenePoint = mapToScene(position);
    int gridX = static_cast<int>(floor(scenePoint.x() / scaleFactor));
    int gridY = static_cast<int>(floor(scenePoint.y() / scaleFactor));
    QPoint gridPos(gridX, gridY);

    QColor currentColor = Qt::transparent;
    drawingState[gridPos] = currentColor; // Update the visible drawing state

    // Also update the full-resolution drawing state
    fullResolutionDrawingState[gridPos] = currentColor;

    QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));
    if (currentGrid) {
        //change its color transparent or previous color based on if erase is active
        if (eraseActive)
        {
            localColor = Qt::transparent;
        }
        else
        {
            // Go back to the color selected by the user before they selected erase button
            localColor = colorPrev;
        }
        currentGrid->setBrush(QBrush(localColor));
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

//getting the drawing area scene for copying to preview window
QGraphicsScene* drawingCanvas::getScene() {
    return scene;
}

// just clean the grids
void drawingCanvas::cleanGrids() {
    for (auto &item : scene->items()) {
        QGraphicsRectItem *rect = qgraphicsitem_cast<QGraphicsRectItem*>(item);
        if (rect) {
            rect->setBrush(QBrush(Qt::white));
        }
    }
}
void drawingCanvas::addNewFrame() {
    cleanGrids();
    currentFrameIndex ++;
}

void drawingCanvas::clear(){
    scene->clear();
    drawGrid(gridDimension);
}


void drawingCanvas::saveDrawing(const QString &filePath) {
    QJsonArray jsonArray;

    scaleFactor = this->width() / gridDimension;

    //Get every pixel color
    for (int x = 0; x < gridDimension; x++) {
        QJsonArray rowArray;
        for (int y = 0; y < gridDimension; y++) {
            QPointF scenePoint = mapToScene(QPoint(x * scaleFactor + scaleFactor / 2, y * scaleFactor + scaleFactor / 2));
            QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));

            QColor color = item ? item->brush().color() : Qt::transparent;
            QString colorStr = color.name(QColor::HexArgb);
            rowArray.append(colorStr);
        }
        jsonArray.append(rowArray);
    }

    //Name it
    QJsonObject jsonObject;
    jsonObject["grid"] = jsonArray;

    //Save it to the chose file
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();
    }
}

void drawingCanvas::loadDrawing(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject jsonObject = doc.object();
    QJsonArray gridArray = jsonObject["grid"].toArray();

    scaleFactor = this->width() / gridDimension;

    //Read every grid
    for (int x = 0; x < gridArray.size(); ++x) {
        QJsonArray rowArray = gridArray[x].toArray();
        for (int y = 0; y < rowArray.size(); ++y) {
            QColor color(rowArray[y].toString());
            QPointF scenePoint = mapToScene(QPoint(x * scaleFactor, y * scaleFactor));
            QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));

            if (item) {
                item->setBrush(QBrush(color));
            } else {
                QPen pen(Qt::gray);
                pen.setWidth(0);
                scene->addRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor, pen, QBrush(color));
            }
        }
    }

    file.close();
}
