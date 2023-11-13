#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include "GridItem.h"

class drawingCanvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit drawingCanvas(QWidget *parent = nullptr);
    void Eraserchange(bool state);
    void colorChange(QColor newColor);
    void drawingMode(bool state);
    void fillMode(bool state);

    //getting the drawing area scene for copy
    QGraphicsScene* getScene() const;

protected:
    void drawGrid(double gridDimension);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawOnGrid(const QPoint &position);
    void fillBucket(const QPointF, int scaleX, int scaleY);

private:
    QGraphicsScene *scene;
    bool drawActive;
    bool eraseActive;
    bool drawMode;
    bool fillActive;
    double gridDimension;
    QColor color = Qt::black; // default color to black
    QColor colorPrev = Qt::black; // Saves the previous color to go back to
    double scaleFactor;
    QPen pen;
    QBrush brush;
    GridItem *gridItem;

public slots:

    void gridSizeChanged(int newSize);

signals:
    //for expressing draw has been finished
    void drawingFinish();
};

#endif // DRAWINGCANVAS_H
