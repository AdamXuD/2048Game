#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"
//#include "mainwindow.h"

class Network : public QObject
{
    Q_OBJECT
    friend class MainWindow;
    QTcpSocket *socket = nullptr;
    bool isOnline = false;
    QString notice;

public:
    explicit Network(QObject *parent = nullptr);

signals:

};

#endif // NETWORK_H
