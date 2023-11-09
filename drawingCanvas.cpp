#include "drawingCanvas.h"

drawingCanvas::drawingCanvas(QWidget *parent) : QGraphicsView(parent), drawing(false) {
    scene = new QGraphicsScene(this);

    gridCells = 20;

    scaleFactor =  500 / gridCells; // Adjust this based on your actual cell size

    this->setScene(scene);

    scene->setSceneRect(0, 0, gridCells * scaleFactor, gridCells * scaleFactor);

    drawGrid(this->width(), this->height());
    //Other initialization code
}

void drawingCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        // Snap to grid
        QPointF snappedPoint = snapToGrid(mapToScene(event->pos()));
        // Fill the cell
        scene->addRect(QRectF(snappedPoint, QSize(scaleFactor, scaleFactor)), QPen(Qt::NoPen), QBrush(Qt::black));
        lastPoint = snappedPoint;
    }
}

void drawingCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (drawing && (event->buttons() & Qt::LeftButton)) {
        // Snap to grid
        QPointF snappedPoint = snapToGrid(mapToScene(event->pos()));
        // Check if we moved to a new cell
        if(snappedPoint != lastPoint) {
            // Fill the new cell
            scene->addRect(QRectF(snappedPoint, QSize(scaleFactor, scaleFactor)), QPen(Qt::NoPen), QBrush(Qt::black));
            lastPoint = snappedPoint;
        }
    }
}

void drawingCanvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawing) {
        drawing = false;
    }
}

void drawingCanvas::drawGrid(int width, int height) {
    // Clear the previous grid if needed
    scene->clear();

    QPen pen(Qt::gray);
    pen.setWidth(0); // Set pen width to 0 to draw lines one pixel wide
    QBrush brush(Qt::black); // Brush to fill the cells

    // Loop over each cell in the grid and fill it
    for (int x = 0; x < gridCells; x++) {
        for (int y = 0; y < gridCells; y++) {
            scene->addRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor, pen, brush);
        }
    }

    // Draw grid lines on top of the filled cells
    for (int x = 0; x <= gridCells; x++) {
        scene->addLine(x * scaleFactor, 0, x * scaleFactor, height, pen);
    }

    for (int y = 0; y <= gridCells; y++) {
        scene->addLine(0, y * scaleFactor, width, y * scaleFactor, pen);
    }
}


// Helper function to snap a point to the nearest grid cell top-left corner
QPointF drawingCanvas::snapToGrid(const QPointF& point) {
    int x = static_cast<int>(point.x()) / scaleFactor * scaleFactor;
    int y = static_cast<int>(point.y()) / scaleFactor * scaleFactor;
    return QPointF(x, y);
}
