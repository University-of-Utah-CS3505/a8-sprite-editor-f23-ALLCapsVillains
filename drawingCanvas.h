/*
 *Drawing Model Header
 *Created by Alex Qi, Matthew Goh, Sam Onwukeme, Yujie He, Jake Crane,  ZengZheng Jiang
 *Assignment 8: Sprite Editor (ZFX 1.0)
 */

#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

class drawingCanvas : public QGraphicsView {
    Q_OBJECT

public:
    /*!
     * \brief drawingCanvas Constructor for the drawingCanvas object
     * \param parent QWidget that this drawingCanvas is based on.
     */
    explicit drawingCanvas(QWidget *parent = nullptr);

    /*!
     * \brief Eraserchange Changes the state of the eraser
     * \param state The new state of the eraser
     */
    void eraserChange(bool state);

    /*!
     * \brief colorChange Changes the color of the drawing tool
     * \param newColor The new color of the drawing tool
     */
    void colorChange(QColor newColor);

    /*!
     * \brief getScene Returns the current drawing area scene
     * \return The current drawing area scene
     */
    QGraphicsScene* getScene();

    /*!
     * \brief addNewFrame Adds a new frame to the animation
     */
    void addNewFrame();

    /*!
     * \brief cleanGrids Clears all grids in the current frame
     */
    void cleanGrids();

    /*!
     * \brief setCurrentFrame Sets the current frame to the given index
     * \param index The index of the new current frame
     */
    void setCurrentFrame(int index);

    /*!
     * \brief getFrameScene Returns the scene of the frame at the given index
     * \param index The index of the frame
     * \return The scene of the frame at the given index
     */
    QGraphicsScene* getFrameScene(int index);

    /*!
     * \brief deleteCurrentFrame Deletes the current frame
     */
    void deleteCurrentFrame();

    /*!
     * \brief clear Clears the current frame
     */
    void clear();

    /*!
     * \brief drawingMode Sets the drawing mode state
     * \param state The new state of the drawing mode
     */
    void drawingMode(bool state);

    /*!
     * \brief fillMode Sets the fill mode state
     * \param state The new state of the fill mode
     */
    void fillMode(bool state);

    /*!
     * \brief selectionMode Sets the selection mode state
     * \param state The new state of the selection mode
     */
    void selectionMode(bool state);

    /*!
     * \brief saveDrawing Saves the current drawing to a file
     * \param filePath The path of the file to save the drawing to
     */
    void saveDrawing(const QString &filePath);

    /*!
     * \brief loadDrawing Loads a drawing from a file
     * \param filePath The path of the file to load the drawing from
     */
    void loadDrawing(const QString &filePath);

    /*!
     * \brief newFrame Creates a new frame
     */
    void newFrame();

    /*!
     * \brief frameChanged Changes the current frame to the one at the given index
     * \param i The index of the new current frame
     */
    void frameChanged(int i);

    /*!
     * \brief frame The current frame index
     */
    int numberOfFrames;

    /*!
     * \brief framePick Picks a frame at the given index
     * \param i The index of the frame to pick
     */
    void framePick(int i);

    /*!
     * \brief deleteFrame Deletes a frame at the given index
     * \param i The index of the frame to delete
     */
    void deleteFrame(int i);

private:

    /*!
     * \brief drawGrid Draws a grid with the given dimension
     */
    void drawGrid();

    /*!
     * \brief mousePressEvent Handles mouse press events
     * \param event The mouse press event
     */
    void mousePressEvent(QMouseEvent *event) override;

    /*!
     * \brief mouseMoveEvent Handles mouse move events
     * \param event The mouse move event
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /*!
     * \brief mouseReleaseEvent Handles mouse release events
     * \param event The mouse release event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /*!
     * \brief drawOnGrid Draws on the grid at the given position
     * \param position The position on the grid to draw
     */
    void drawOnGrid(const QPoint &position);

    /*!
     * \brief fillBucket Fills an area of the grid with the current color
     * \param position The position to start filling from
     * \param scaleX The scale factor in the x direction
     * \param scaleY The scale factor in the y direction
     */
    void fillBucket(const QPointF, double scaleX, double scaleY);

    /*!
     * \brief movePixels Moves selected pixels to a new position
     * \param position The new position to move the pixels to
     */
    void movePixels(const QPointF);

    /*!
     * \brief scene The current drawing scene
     */
    QGraphicsScene *scene;

    /*!
     * \brief frames The frames of the animation
     */
    QMap<int,QGraphicsScene*> frames;

    /*!
     * \brief toolActive Whether a tool is currently active
     */
    bool toolActive;

    /*!
     * \brief drawActive Whether the draw tool is currently active
     */
    bool drawActive;

    /*!
     * \brief eraseActive Whether the erase tool is currently active
     */
    bool eraseActive;

    /*!
     * \brief fillActive Whether the fill tool is currently active
     */
    bool fillActive;

    /*!
     * \brief selectionActive Whether the selection tool is currently active
     */
    bool selectionActive;

    /*!
     * \brief moving Whether the user is currently moving pixels
     */
    bool moving;

    /*!
     * \brief lastMousePosition The last known position of the mouse
     */
    QPointF lastMousePosition;

    /*!
     * \brief gridDimension The dimension of the grid
     */
    double gridDimension;

    /*!
     * \brief localColor The current color
     */
    QColor localColor;

    /*!
     * \brief colorPrev The previous color
     */
    QColor colorPrev;

    /*!
     * \brief scaleFactor The scale factor of the grid
     */
    double scaleFactor;

    /*!
     * \brief pen The pen used for drawing lines
     */
    QPen pen;

    /*!
     * \brief brush The brush used for coloring
     */
    QBrush brush;

    /*!
     * \brief drawingState The color state of each grid cell
     */
    QHash<QPoint, QColor> colorState;

    /*!
     * \brief fullResolutionDrawingState The color state of each grid cell at the highest resolution
     */
    QHash<QPoint, QColor> fullResolutionDrawingState;

    /*!
     * \brief currentFrameIndex The index of the current frame
     */
    int currentFrameIndex;


public slots:
    /*!
    * \brief gridSizeChanged Handles the event when the grid size changes
    * \param newSize The new size of the grid
    */
    void gridSizeChanged(int newSize);

signals:
    /*!
    * \brief drawingFinish Signal that is emitted when the drawing is finished
    * \param currentFrameIndex The index of the current frame
    */
    void drawingFinish(int currentFrameIndex);

    /*!
    * \brief updatePreviewWindow Signal that is emitted to update the preview window
    */
    void updatePreviewWindow();

    /*!
    * \brief cleanFrame Signal that is emitted to clean the current frame
    */
    void cleanFrame();

    /*!
    * \brief addFrame Signal that is emitted to add a new frame
    */
    void addFrame();

};

#endif // DRAWINGCANVAS_H
