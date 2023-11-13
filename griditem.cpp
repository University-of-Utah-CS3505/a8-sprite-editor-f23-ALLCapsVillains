#include "GridItem.h"
#include <QPainter>

GridItem::GridItem(double gridDimension, QGraphicsItem *parent) : QGraphicsItem(parent), gridDimension(gridDimension) {}

QRectF GridItem::boundingRect() const {
    // Assuming your scene is as big as the view for simplicity
    return QRectF(0, 0, 500, 500);
}

void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    QPen pen(Qt::gray);
    pen.setWidth(0);
    painter->setPen(pen);
    double scaleFactor = 500 / gridDimension;
    for (double x = 0; x <= gridDimension; ++x) {
        for (double y = 0; y <= gridDimension; ++y) {
            painter->drawRect(x * scaleFactor, y * scaleFactor, scaleFactor, scaleFactor);
        }
    }
}

void GridItem::updateGridDimension(double newGridDimension) {
    gridDimension = newGridDimension;
    update();  // This triggers a repaint of the item
}
