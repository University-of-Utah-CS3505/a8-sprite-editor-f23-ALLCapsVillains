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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawGrid(int width, int height);
    QPointF snapToGrid(const QPointF& point);

private:
    QGraphicsScene *scene;
    QPointF lastPoint;
    int scaleFactor; // Adjust this based on your actual cell size
    int gridCells;
    bool drawing; // to check if we are currently drawing

};

#endif // DRAWINGCANVAS_H
