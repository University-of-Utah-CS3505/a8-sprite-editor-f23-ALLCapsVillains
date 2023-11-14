#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QColorDialog>
#include<QGraphicsView>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeGridSize();
    void previewWindowUpdate();
    void frameUpdate(int index);
    void fpsChanged(int fps);

private:
    Ui::MainWindow *ui;
    QList<QGraphicsView*> framesViewsList;
    QTimer* previewAnimationTimer;

    int currentFrameIndex;

    QMap<int,QGraphicsScene*> frames;
    QGraphicsScene *previousScene;
    QGraphicsScene *nextScene;

public slots:
    void eraseButtonClicked();
private slots:
    void drawButtonClicked();
    void fillButtonClicked();
    void colorButtonClicked();
    void on_addFrame_clicked();
    void on_deleteFrame_clicked();
    void on_spinBox_valueChanged(int arg1);
};

#endif
