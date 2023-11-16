#include "drawingCanvas.h"
#include <QGraphicsItem>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include<QDebug>

drawingCanvas::drawingCanvas(QWidget *parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);

    pen = QPen(Qt::gray);
    pen.setWidth(0);
    brush = QBrush(Qt::transparent);

    currentFrameIndex = 0;
    frame = 0;

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

void drawingCanvas::selectionMode(bool state)
{
    selectionActive = state;
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

    QPen pen(Qt::black);
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
            QPointF delta = mapToScene(event->pos()) - lastMousePosition;

            if (delta.x() >= scaleFactor || delta.x() <= -scaleFactor || delta.y() >= scaleFactor || delta.y() <= -scaleFactor)
            {
                movePixels(delta);
                lastMousePosition = mapToScene(event->pos());
            }
        }
    }
}

// mouse release
void drawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    toolActive = false;

    if (moving)
    {
        moving = false;
    }
    emit drawingFinish(currentFrameIndex);
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

    //find the current the grid we chosed and change its color
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

void drawingCanvas::fillBucket(QPointF scenePoint, double scaleX, double scaleY) {
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
            rect->setBrush(QBrush(Qt::transparent));
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

void drawingCanvas::movePixels(QPointF delta)
{
    int dx = delta.x() / scaleFactor;
    int dy = delta.y() / scaleFactor;

    QVector<QVector<QColor>> newGridColors(gridDimension, QVector<QColor>(gridDimension, Qt::transparent));
    // Move colors to new positions
    for (int y = 0; y < gridDimension; y++) {
        for (int x = 0; x < gridDimension; x++) {
            int newX = (x + dx);
            int newY = (y + dy);
            if (newX >= 0 && newY >= 0 && newX < gridDimension && newY < gridDimension) {
                QGraphicsItem *item = scene->itemAt(x * scaleFactor, y * scaleFactor, QTransform());
                QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
                if (rectItem) {
                    newGridColors[newY][newX] = rectItem->brush().color();
                }
            }
        }
    }

    // Update grid colors
    for (int y = 0; y < gridDimension; y++) {
        for (int x = 0; x < gridDimension; x++) {
            QGraphicsItem *item = scene->itemAt(x * scaleFactor, y * scaleFactor, QTransform());
            QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            if (rectItem) {
                rectItem->setBrush(QBrush(newGridColors[y][x]));
            }
        }
    }
}

void drawingCanvas::saveDrawing(const QString &filePath) {
    QJsonArray framesArray;
    scaleFactor = this->width() / gridDimension;

    for (auto it = frames.begin(); it != frames.end(); ++it) {
        scene = it.value();
        QJsonArray gridArray;

        for (int x = 0; x < gridDimension; x++) {
            QJsonArray rowArray;
            for (int y = 0; y < gridDimension; y++) {
                QPointF scenePoint = mapToScene(QPoint(x * scaleFactor + scaleFactor / 2, y * scaleFactor + scaleFactor / 2));
                QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));

                QColor color = item ? item->brush().color() : Qt::transparent;
                QString colorStr = QString::number(color.red()) + "," + QString::number(color.green()) + "," + QString::number(color.blue()) + "," + QString::number(color.alpha());
                rowArray.append(colorStr);
            }
            gridArray.append(rowArray);
        }

        QJsonObject frameObject;
        frameObject["grid"] = gridArray;
        framesArray.append(frameObject);
    }

    QJsonObject jsonObject;
    jsonObject["frames"] = framesArray;

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();
    }

    scene = frames.value(currentFrameIndex);
}


void drawingCanvas::loadDrawing(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject jsonObject = doc.object();
    QJsonArray framesArray = jsonObject["frames"].toArray();

    scaleFactor = this->width() / gridDimension;

    for (auto &scene : frames) {
        delete scene;
    }
    frames.clear();
    currentFrameIndex = 0;
    frame = -1;
    emit cleanFrame();

    for (const QJsonValue &frameVal : framesArray) {
        frame++;
        QGraphicsScene* newScene = new QGraphicsScene(this);

        QJsonObject frameObject = frameVal.toObject();
        QJsonArray gridArray = frameObject["grid"].toArray();

        for (int x = 0; x < gridArray.size(); ++x) {
            QJsonArray rowArray = gridArray[x].toArray();
            for (int y = 0; y < rowArray.size(); ++y) {
                QStringList rgba = rowArray[y].toString().split(",");
                if (rgba.size() == 4) {
                    QColor color(rgba[0].toInt(), rgba[1].toInt(), rgba[2].toInt(), rgba[3].toInt());
                    QGraphicsRectItem *item = new QGraphicsRectItem(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor);
                    item->setBrush(QBrush(color));
                    newScene->addItem(item);
                }
            }
        }

        frames.insert(frame, newScene);
        if(currentFrameIndex != 0){
            emit addFrame();
        } else {
            this->setScene(frames.value(0));
            scene = frames.value(0);
        }

        emit drawingFinish(currentFrameIndex);
        currentFrameIndex++;
    }

    file.close();

    if (!frames.isEmpty()) {
        scene = frames.value(0);
        currentFrameIndex = 0;
    }
}
