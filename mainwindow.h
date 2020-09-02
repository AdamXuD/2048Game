#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "common.h"

#include "myinputdialog.h"
#include "achievementdialog.h"
#include "network.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QStack<int *> mapRecorder;

    Game game;
    Network network;
    Database db;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QColor getItemColor(int num);

    void keyPressEvent(QKeyEvent * event);
    
    void showServerInfo();

    void actionInit();
    void windowInit();

    void overHandler();
    void printer();
    void offlineWarning();
    void showAchievement(QVariantList list);

private:

};
#endif // MAINWINDOW_H
