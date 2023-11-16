/*
 *Main Class
 *Created by Alex Qi, Matthew Goh, Sam Onwukeme, Yujie He, Jake Crane,  ZengZheng Jiang
 *Assignment 8: Sprite Editor (ZFX 1.0)
 *
 * Jake Github: @jakecolacoo
 * Alex Github: @Qibola
 * Matt Github: @Mattgoh
 * Sam Github: @reignoverall
 * Mario Github: @Mariohyj
 * Constantine Github: @CycleBird
 *
 * Style Reviewed by Constantine.
*/

#include "mainwindow.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
