#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "common.h"

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

    void onUndoBtn();

private:
    Ui::MainWindow *ui;
    QStack<int *> mapRecorder;
    Game g;

};
#endif // MAINWINDOW_H
