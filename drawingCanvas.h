#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>

class drawingCanvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit drawingCanvas(QWidget *parent = nullptr);


protected:
    void drawGrid(double gridDimension);

private:
    QGraphicsScene *scene;

public slots:

    void gridSizeChanged(int newSize);


};

#endif // DRAWINGCANVAS_H
