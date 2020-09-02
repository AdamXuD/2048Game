#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"

class Network : public QObject
{
    Q_OBJECT
    QTcpSocket socket;
    bool isOnline = false;
    QString notice;

    std::function<void()> offlineWarning;
    std::function<void(QVariantList)> showAchievement;
    std::function<void()> showServerInfo;
    std::function<void()> connectFailed;

public:
    explicit Network(QObject *parent = nullptr);

    void setOfflineWarning(std::function<void()> func);
    void setShowAchievement(std::function<void(QVariantList)> func);
    void setShowServerInfo(std::function<void()> func);
    void setConnectFailed(std::function<void()> func);
    QString getServerNotice();
    bool isConnected();

    void connectToServer(QString name, QString IP, int port);

    void sendUserName(QString userName);
    void sendUploadScore(int score, int maxNum);
    void sendMatchQuery();
    void sendPersonalAchievementQuery();
    void sendAllAchievementQuery();

private:
    void sendMsg(msgType type, QString content = QString());
    void readMsg();
    void sendToServer(Msg &msg);
    void receiveFromServer(Msg &msg);
    void MsgHandler(Msg &msg);

signals:
    void ServerConnected();
    void ServerDisconnected();

};

#endif // NETWORK_H
