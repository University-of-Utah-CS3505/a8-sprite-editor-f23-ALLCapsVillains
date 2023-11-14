#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QMap>

class drawingCanvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit drawingCanvas(QWidget *parent = nullptr);
    void Eraserchange(bool state);
    void colorChange(QColor newColor);

    void drawingMode(bool state);
    void fillMode(bool state);
    void selectionMode(bool state);


protected:
    void drawGrid(double gridDimension);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawOnGrid(const QPoint &position);
    void fillBucket(const QPointF, double scaleX, double scaleY);
    void movePixels(const QPointF);

private:
    QGraphicsScene *scene;
    bool toolActive;
    bool eraseActive;
    bool drawActive;
    bool fillActive;
    bool selectionActive;

    bool moving;
    QPointF lastMousePosition;
    double currentGridDimension;

    double gridDimension;
    QColor color = Qt::black; // default color to black
    QColor colorPrev = Qt::black; // Saves the previous color to go back to
    double scaleFactor; // holds the length of each square on grid

public slots:

    void gridSizeChanged(int newSize);


};

#endif // DRAWINGCANVAS_H
