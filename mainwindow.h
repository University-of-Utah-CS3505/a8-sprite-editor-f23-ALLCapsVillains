#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QColorDialog>

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
    int frame=0;

private:
    Ui::MainWindow *ui;

public slots:
    void eraseButtonClicked();
private slots:
    void drawButtonClicked();
    void fillButtonClicked();
    void colorButtonClicked();
    void saveDrawing();
    void loadDrawing();
    void rojoTheme();
    void darkTheme();
    void skyTheme();
    void mochaTheme();
    void clearPage();
    void on_addFrame_clicked();
    void on_deleteFrame_clicked();
    void on_framePicker_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
