#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMessageBox>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void start();
    void printMap();

    QColor getItemColor(int num);

    void keyPressEvent(QKeyEvent * event);

private:
    Ui::MainWindow *ui;
    Game g;

};
#endif // MAINWINDOW_H
