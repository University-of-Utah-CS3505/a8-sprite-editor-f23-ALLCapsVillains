/*
 *Main Class
 *Created by Christian Hartman and Jacob Crane
 *Assignment 6: QT Simon Game
 *
 * Jake Github: @jakecolacoo
 * Christian Github: @Is-This-One-Avaiable
 *
 * Creative Element: We implemented difficulty modes that the user can select at the beginning of the turn.
 * Selecting higher difficulties will increase the initial number of flashes in the sequence and increases
 * the number of flashes added to the sequence every round. The speed of the flashes are sped up accordingly
 * to appropriately set the game's pace.
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
