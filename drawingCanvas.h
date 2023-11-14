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
    QGraphicsScene* getScene();

    void addNewFrame();
    void cleanGrids();

    //    void newFrame();
    //    void frameChanged(int i);
    //    int frame;
    //    void framePick(int i);
    //    void deleteFrame(int i);
    //    QMap<int,QGraphicsScene*> frames;
    //void addNewFrame();
    void setCurrentFrame(int index);
    QGraphicsScene* getFrameScene(int index);
    void deleteCurrentFrame();


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
    QHash<QPoint, QColor> drawingState; // Store the color state of each grid cell
    QHash<QPoint, QColor> fullResolutionDrawingState; // Maintain this at the highest resolution

    QMap<int, QGraphicsScene*> frameScenes;
    //int currentFrameIndex;
    int currentFrameIndex;

    //frames



public slots:

    void gridSizeChanged(int newSize);

signals:
    //for expressing draw has been finished
    void drawingFinish(int currentFrameIndex);
    void updatePreviewWindow();
};

#endif // DRAWINGCANVAS_H
