#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QGraphicsItem>

class GridItem : public QGraphicsItem {
public:
    explicit GridItem(double gridDimension, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void updateGridDimension(double newGridDimension);

private:
    double gridDimension;
};

#endif // GRIDITEM_H
