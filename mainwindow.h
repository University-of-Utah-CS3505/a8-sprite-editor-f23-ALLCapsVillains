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

private:
    Ui::MainWindow *ui;
    QColor color;

public slots:
    void eraseButtonClicked();
private slots:
    void drawButtonClicked();
    void colorButtonClicked();
};

#endif // MAINWINDOW_H
