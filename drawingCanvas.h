#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QMap>
#include "GridItem.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

class drawingCanvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit drawingCanvas(QWidget *parent = nullptr);
    void Eraserchange(bool state);
    void colorChange(QColor newColor);
  
    //getting the drawing area scene for copy
    QGraphicsScene* getScene();

    void addNewFrame();
    void cleanGrids();
    void setCurrentFrame(int index);
    QGraphicsScene* getFrameScene(int index);
    void deleteCurrentFrame();
    void clear();

    void drawingMode(bool state);
    void fillMode(bool state);
    void selectionMode(bool state);

    void saveDrawing(const QString &filePath);
    void loadDrawing(const QString &filePath);

    void newFrame();
    void frameChanged(int i);
    int frame;
    void framePick(int i);
    void deleteFrame(int i);

protected:
    void drawGrid(double newGridDimension);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void drawOnGrid(const QPoint &position);
    void fillBucket(const QPointF, double scaleX, double scaleY);
    void movePixels(const QPointF);



private:
    QGraphicsScene *scene;
    bool toolActive;
    QMap<int,QGraphicsScene*> frames ;
    bool eraseActive;
    bool drawActive;
    bool fillActive;
    bool selectionActive;

    bool moving;
    QPointF lastMousePosition;
    double currentGridDimension;

    double gridDimension;
    QColor localColor = Qt::black; // default color to black
    QColor colorPrev = Qt::black; // Saves the previous color to go back to
  
    double scaleFactor; // holds the length of each square on grid
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
