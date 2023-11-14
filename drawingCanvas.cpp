#include "drawingCanvas.h"
#include <QGraphicsItem>

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

    double startGridDimension = 10;

    scene->setSceneRect(0, 0, startGridDimension * 49, startGridDimension * 49);

    this->setSceneRect(5, 5, scene->width(), scene->height());

    drawGrid(startGridDimension);
    //sam
    //frames.insert(0,scene);
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
    color = newColor;
}

// draw colors on the grids
void drawingCanvas::drawOnGrid(const QPoint &position) {
    QPointF scenePoint = mapToScene(position);
    int gridX = static_cast<int>(floor(scenePoint.x() / scaleFactor));
    int gridY = static_cast<int>(floor(scenePoint.y() / scaleFactor));
    QPoint gridPos(gridX, gridY);

    QColor currentColor = eraseActive ? Qt::transparent : color;
    drawingState[gridPos] = currentColor; // Update the visible drawing state

    // Also update the full-resolution drawing state
    fullResolutionDrawingState[gridPos] = currentColor;

    QGraphicsRectItem *currentGrid = qgraphicsitem_cast<QGraphicsRectItem*>(scene->itemAt(scenePoint, QTransform()));
    if (currentGrid) {
        currentGrid->setBrush(QBrush(currentColor));
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

//sam's version
//void drawingCanvas::newFrame(){
//    frame++;
//    QGraphicsScene* frameAdded = new QGraphicsScene(this);
//    frames.insert(frame,frameAdded);
//}

//void drawingCanvas::frameChanged(int i){
//    this->setScene(frames.value(i));
//    scene = frames.value(i);
//    drawGrid(10);

//}
//void drawingCanvas::framePick(int i){
//    this->setScene(frames.value(i));
//    scene = frames.value(i);

//}

//void drawingCanvas::deleteFrame(int i){
//    if(frames.count() > 0){
//        frame --;


//        this->setScene(frames.value(i-1));
//        scene = frames.value(i-1);

//    }

//}

//void drawingCanvas::addNewFrame() {
//    // Create a new scene for the new frame
//    QGraphicsScene* newScene = new QGraphicsScene(this);
//    // Add the new scene to the map
//    frameScenes.insert(++currentFrameIndex, newScene);
//    // Optionally, emit a signal to update the UI
//    //emit frameAdded(currentFrameIndex);
//}

//void drawingCanvas::setCurrentFrame(int index) {
//    if(frameScenes.contains(index)) {
//        // Set the current scene to the selected frame's scene
//        this->setScene(frameScenes.value(index));
//        currentFrameIndex = index;
//        // Optionally, emit a signal to update the UI
//        //emit frameChanged(index);
//    }
//}

//QGraphicsScene* drawingCanvas::getFrameScene(int index) {
//    return frameScenes.value(index, nullptr);
//}

//void drawingCanvas::deleteCurrentFrame() {
//    if(frameScenes.size() > 1 && frameScenes.contains(currentFrameIndex)) {
//        QGraphicsScene* sceneToDelete = frameScenes.take(currentFrameIndex);
//        delete sceneToDelete;
//        // Adjust the current frame index and set the scene
//        setCurrentFrame(--currentFrameIndex);
//        // Optionally, emit a signal to update the UI
//        //emit frameDeleted(currentFrameIndex);
//    }
//}
