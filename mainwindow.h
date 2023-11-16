/*
 *Main Window View
 *Created by Alex Qi, Matthew Goh, Sam Onwukeme, Yujie He, Jake Crane,  ZengZheng Jiang
 *Assignment 8: Sprite Editor (ZFX 1.0)
 *
 *Style Reviewed by Sam.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QColorDialog>
#include<QGraphicsView>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    /*!
    * \brief MainWindow Constructor for the MainWindow object
    * \param parent QWidget that this MainWindow is based on.
    */
    MainWindow(QWidget *parent = nullptr);

    /*!
    * \brief ~MainWindow Destructor for the MainWindow object
    */
    ~MainWindow();

    /*!
    * \brief changeGridSize Changes the size of the grid
    */
    void changeGridSize();

    /*!
    * \brief previewWindowUpdate Updates the preview window
    */
    void previewWindowUpdate();

    /*!
    * \brief frameUpdate Updates the frame at the given index
    */
    void frameUpdate();

    /*!
    * \brief fpsChanged Handles the event when the frames per second (fps) changes
    * \param fps The new frames per second value
    */
    void fpsChanged(int fps);

    /*!
    * \brief frame Holds the current frame number
    */
    int frame=0;

private:
    /*!
    * \brief ui Pointer to the user interface for the MainWindow
    */
    Ui::MainWindow *ui;

    /*!
    * \brief framesViewsList List of QGraphicsView objects representing the frames
    */
    QList<QGraphicsView*> framesViewsList;

    /*!
    * \brief previewAnimationTimer Timer for controlling the preview animation
    */
    QTimer* previewAnimationTimer;

    /*!
    * \brief currentFrameIndex Index of the current frame
    */
    int currentFrameIndex;

    /*!
    * \brief frames Map of frame indices to QGraphicsScene objects representing the frames
    */
    QMap<int,QGraphicsScene*> frames;

    /*!
    * \brief previousScene Pointer to the QGraphicsScene object representing the previous scene
    */
    QGraphicsScene *previousScene;

    /*!
    * \brief nextScene Pointer to the QGraphicsScene object representing the next scene
    */
    QGraphicsScene *nextScene;

    /*!
    * \brief firstFPS Boolean indicating whether this is the first frames per second (fps) value
    */
    bool firstFPS;

private slots:
    /*!
    * \brief eraseButtonClicked Handles the event when the erase button is clicked
    */
    void eraseButtonClicked();

    /*!
    * \brief drawButtonClicked Handles the event when the draw button is clicked
    */
    void drawButtonClicked();

    /*!
    * \brief fillButtonClicked Handles the event when the fill button is clicked
    */
    void fillButtonClicked();

    /*!
    * \brief colorButtonClicked Handles the event when the color button is clicked
    */
    void colorButtonClicked();

    /*!
     * \brief selectionButtonClicked Handles the event when the selection button is clicked
     */
    void selectionButtonClicked();

    /*!
     * \brief cursorButtonClicked Handles the event when the cursor button is clicked
     */
    void cursorButtonClicked();

    /*!
     * \brief onSpinBoxValueChanged Handles the event when the value of the spin box changes
     * \param value The new value of the spin box
     */
    void onSpinBoxValueChanged(int value);

    /*!
     * \brief onAddFrameClicked Handles the event when the add frame button is clicked
     */
    void onAddFrameClicked();

    /*!
     * \brief onDeleteFrameClicked Handles the event when the delete frame button is clicked
     */
    void onDeleteFrameClicked();

    /*!
     * \brief onFramePickerValueChanged Handles the event when the value of the frame picker changes
     * \param value The new value of the frame picker
     */
    void onFramePickerValueChange(int value);

    /*!
     * \brief saveDrawing Saves the current drawing
     */
    void saveDrawing();

    /*!
     * \brief loadDrawing Loads a saved drawing
     */
    void loadDrawing();

    /*!
     * \brief rojoTheme Applies the Rojo theme
     */
    void rojoTheme();

    /*!
     * \brief darkTheme Applies the Dark theme
     */
    void darkTheme();

    /*!
     * \brief skyTheme Applies the Sky theme
     */
    void skyTheme();

    /*!
     * \brief mochaTheme Applies the Mocha theme
     */
    void mochaTheme();

    /*!
     * \brief cleanFrame Cleans the current frame
     */
    void cleanFrames();

    /*!
     * \brief addFrame Saves frame into model's preview window collection
     */
    void saveFrame();
};

#endif
