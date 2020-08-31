#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include "common.h"

#include "myinputdialog.h"
#include "achimentdialog.h"
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    QStack<int *> mapRecorder;
    Game g;

    QTcpSocket *socket = nullptr;
    bool isOnline = false;
    QString notice;


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void start();
    void printMap();

    QColor getItemColor(int num);

    void keyPressEvent(QKeyEvent * event);
    
    void sendToServer(Msg &msg);
    void receiveFromServer(Msg &msg);
    void MsgHandler(Msg &msg);

    void showServerInfo();

    void actionInit();
    void windowInit();
    void test();

private:

};
#endif // MAINWINDOW_H
