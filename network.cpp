#include "network.h"

Network::Network(QObject *parent) : QObject(parent)
{
    connect(&socket, &QTcpSocket::connected, [=](){ emit ServerConnected(); this->isOnline = true; } );
    connect(&socket, &QTcpSocket::disconnected,  [=](){ emit ServerDisconnected(); this->isOnline = false; });
}

bool Network::isConnected()
{
    return this->isOnline;
}

void Network::setOfflineWarning(std::function<void ()> func)
{
    this->offlineWarning = func;
}

void Network::setShowAchievement(std::function<void (QVariantList)> func)
{
    this->showAchievement = func;
}

void Network::setShowServerInfo(std::function<void()> func)
{
    this->showServerInfo = func;
}

void Network::setConnectFailed(std::function<void()> func)
{
    this->connectFailed = func;
}

void Network::connectToServer(QString name, QString IP, int port)
{
    if(isOnline)
    {
        socket.disconnectFromHost();
        disconnect(&socket, &QTcpSocket::readyRead, this, &Network::readMsg);
    }
    socket.connectToHost(IP, port);
    if(socket.waitForConnected())
    {
        connect(&socket, &QTcpSocket::readyRead, this, &Network::readMsg);
        sendUserName(name);
    }
    else
        connectFailed();
}

void Network::sendUserName(QString userName)
{
    sendMsg(msgType::userName, userName);
}

void Network::sendUploadScore(int score, int maxNum)
{
    QJsonObject obj;
    obj["score"] = score;
    obj["maxNum"] = maxNum;
    QJsonDocument doc;
    doc.setObject(obj);
    sendMsg(msgType::uploadScore, doc.toJson());
}

void Network::sendMatchQuery()
{
    sendMsg(msgType::matchQuery);
}

void Network::sendPersonalAchievementQuery()
{
    sendMsg(msgType::personalAchievementQuery);
}

void Network::sendAllAchievementQuery()
{
    sendMsg(msgType::allAchievementQuery);
}

void Network::readMsg()
{
    Msg msg;
    receiveFromServer(msg);
    MsgHandler(msg);
}

void Network::sendMsg(msgType type, QString content)
{
    if(!isOnline)
        offlineWarning();
    Msg msg(type, content);
    sendToServer(msg);
}

void Network::sendToServer(Msg &msg)
{
    QString data = msg.packUp();
    socket.write(data.toUtf8().data(), data.size());

#if DEBUGMODE == true
    qDebug() << "Send A New Msg..." << endl;
    qDebug() << "type:" << msg.type << endl;
    qDebug() << "content:" << msg.content.toUtf8().data() << endl;
#endif

}

void Network::receiveFromServer(Msg &msg)
{
    QString data = socket.readAll();
    msg.unpack(data.toUtf8().data());

#if DEBUGMODE == true
    qDebug() << "Get A New Msg..." << endl;
    qDebug() << "type:" << msg.type << endl;
    qDebug() << "content:" << msg.content.toUtf8().data() << endl;
#endif

}

void Network::MsgHandler(Msg &msg)
{
    switch ((int)msg.type)
    {
    case msgType::connectSuccess:
    {
        this->notice = msg.content;
        showServerInfo();
        break;
    }
    case msgType::personalAchievement:
    case msgType::allAchievement:
    {
        showAchievement(QJsonDocument::fromJson(msg.content.toUtf8()).array().toVariantList());
        break;
    }
    }

}

QString Network::getServerNotice()
{
    return this->notice;
}
