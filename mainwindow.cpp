#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("2048");

    windowInit();
    actionInit();

    start();
}

void MainWindow::windowInit()
{
    ui->tableWidget_map->setRowCount(4);
    ui->tableWidget_map->setColumnCount(4);

    ui->tableWidget_map->verticalHeader()->setVisible(false);
    ui->tableWidget_map->horizontalHeader()->setVisible(false);
    ui->tableWidget_map->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_map->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_map->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_map->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_map->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFont font;
    font.setPointSize(18);
    font.setBold(true);
    ui->tableWidget_map->setFont(font);

    QLabel *tips = new QLabel(this);
    tips->setText("使用W, A, S, D或者方向键进行控制！");
    ui->statusbar->addWidget(tips);

    ui->btn_undo->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::actionInit()
{
    /*对战时子窗口函数*/
//    QDockWidget * doc = new QDockWidget("工具",this);
//    doc->setFixedSize(300, 300);
//    doc->setAllowedAreas(Qt::NoDockWidgetArea);
//    doc->setFloating(true);
//    QPushButton *btn = new QPushButton(doc);
//    doc->setWidget(btn);
//    connect(btn, &QPushButton::clicked, [=](){ doc->move(this->pos().x() + 650, doc->pos().y()); });
    /*对战时子窗口函数*/

    /*连接服务器Action*/
    connect(ui->action_connectServer, &QAction::triggered, [=](){
        bool isOk = false;
        QPair<QString, QString> info = MyInputDialog::getPair(this, "连接服务器", "请输入服务器地址与用户名：", &isOk);
        if(!isOk) return;
        QStringList address = info.first.split(":");
        QString IP = address.first();
        unsigned int port = address.last().toUInt();
        if(this->socket == nullptr || isOnline == false)
        {
            if(this->socket != nullptr)
                delete this->socket;
            this->socket = new QTcpSocket(this);
        }
        socket->connectToHost(IP, port);
        if(socket->waitForConnected())
        {
            this->isOnline = true;
            connect(socket, &QTcpSocket::readyRead, [=](){
                Msg msg;
                receiveFromServer(msg);
                MsgHandler(msg);
            });
            Msg msg(msgType::userName, info.second);
            sendToServer(msg);
        }
    });
    /*连接服务器Action*/

    /*服务器信息Action*/
    connect(ui->action_serverInfo, &QAction::triggered, this, &MainWindow::showServerInfo);
    /*服务器信息Action*/

    /*使用教程Action*/
    connect(ui->action_instruction, &QAction::triggered, [](){
        QDesktopServices::openUrl(QUrl("https://github.com/AdamXuD/2048Game/blob/master/README.md"));
    });
    /*使用教程Action*/

    /*关于本游戏Action*/
    connect(ui->action_aboutGame, &QAction::triggered, [=](){
        QMessageBox::information(this, "关于本游戏", QString("<p> 作者：<a href=\"https://paopaopaoge.xyz/\" target=\"_blank\">AdamXuD</a> </p> <p> 项目名称：<a href=\"https://github.com/AdamXuD/2048Game\" target=\"_blank\">2048Game</a> </p> <p> 版本号： %1 </p>").arg(VERSION));
    });
    /*关于本游戏Action*/

    /*匹配对手Action*/
    connect(ui->action_match, &QAction::triggered, [=](){
            Msg msg(msgType::matchQuery);
            sendToServer(msg);
    });
    /*匹配对手Action*/

    /*拉取个人成绩列表Action*/
    connect(ui->action_queryPersonal, &QAction::triggered, [=](){
            Msg msg(msgType::personalAchievementQuery);
            sendToServer(msg);
    });
    /*拉取个人成绩列表Action*/

    /*拉取所有成绩列表Action*/
    connect(ui->action_queryAll, &QAction::triggered, [=](){
            Msg msg(msgType::allAchievementQuery);
            sendToServer(msg);
    });
    /*拉取所有成绩列表Action*/

    /*回退（Undo）操作Action*/
    connect(ui->btn_undo, &QPushButton::clicked, [=](){
        g.undo();
        printMap();
    });
    /*回退（Undo）操作Action*/



}

QColor MainWindow::getItemColor(int num)
{
    switch(num)
    {
    case 2: return QColor(199, 237, 253);
    case 4: return QColor(175, 215, 237);
    case 8: return QColor(92,167, 186);
    case 16: return QColor(147, 224, 255);
    case 32: return QColor(174, 221, 129);
    case 64: return QColor(107, 194, 53);
    case 128: return QColor(6, 126, 67);
    case 256: return QColor(38, 157, 128);
    case 512: return QColor("#FF9A9E");
    case 1024: return QColor("#FFD0C4");
    case 2048: return QColor("#FFECD2");
    case 4096: return QColor("#FCB69F");
    case 8192: return QColor(160, 191, 124);
    case 16384: return QColor(101, 147, 74);
    case 32768: return QColor(64, 116, 52);
    case 65536: return QColor(3, 35, 14);
    default: return QColor(255, 255, 255);
    }
    return QColor(255, 255, 255);
}

void MainWindow::printMap()
{
    ui->tableWidget_map->clearContents();
    ui->label_score->setText(QString("%1").arg(g.data.score));
    for(int row = 0; row < 4; row++)
        for(int col = 0; col < 4; col++)
            if(g.data.map[row][col] != 0)
            {
                QTableWidgetItem *i = new QTableWidgetItem(QString("%1").arg(g.data.map[row][col]));
                i->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_map->setItem(row, col, i);
                ui->tableWidget_map->item(row, col)->setBackgroundColor(getItemColor(g.data.map[row][col]));
            }

}

void MainWindow::start()
{
    g.start();
    printMap();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up: g.onUpKey(); break;
    case Qt::Key_S:
    case Qt::Key_Down: g.onDownKey(); break;
    case Qt::Key_A:
    case Qt::Key_Left: g.onLeftKey(); break;
    case Qt::Key_D:
    case Qt::Key_Right: g.onRightKey(); break;
    }

    if(g.isJustMove)
    {
        g.randNum();
        g.isJustMove = false;
        g.data.step++;
        g.recordMap();
        printMap();
    }
    if(g.isOver())
    {
        if(isOnline)
            if(QMessageBox::Yes == QMessageBox::information(this, "Upload Your Score", QString("Game Over. Your score is %1 !\nWould you like to upload your score to our server?\n(The score will safely save on cloud and you can reach them any time.)").arg(g.data.score), QMessageBox::Yes | QMessageBox::No))
            {
                Msg msg(msgType::uploadScore, QString("%1").arg(g.data.score));
                sendToServer(msg);
            }

        if(QMessageBox::Yes == QMessageBox::information(this, "Game Over", QString("Game Over. Your score is %1 !\nWould you like to restart? ").arg(g.data.score), QMessageBox::Yes | QMessageBox::No))
            start();
    }
}

void MainWindow::sendToServer(Msg &msg)
{
    if(!isOnline)
    {
        QMessageBox::warning(this, "请先连接服务器", "服务器未连接，请先连接服务器！");
        return;
    }

    QString data = msg.packUp();
    socket->write(data.toUtf8().data(), data.size());
#if DEBUGMODE == true
    qDebug() << "Send A New Msg..." << endl;
    qDebug() << "type:" << msg.type << endl;
    qDebug() << "content:" << msg.content.toUtf8().data() << endl;
#endif
}

void MainWindow::receiveFromServer(Msg &msg)
{
    QString data = socket->readAll();
    msg.unpack(data.toUtf8().data());
#if DEBUGMODE == true
    qDebug() << "Get A New Msg..." << endl;
    qDebug() << "type:" << msg.type << endl;
    qDebug() << "content:" << msg.content.toUtf8().data() << endl;
#endif

}

void MainWindow::MsgHandler(Msg &msg)
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
    {
        AchimentDialog::showAchimentDialog(this, QJsonDocument::fromJson(msg.content.toUtf8()).array().toVariantList());
        break;
    }
    case msgType::allAchievement:
    {
        AchimentDialog::showAchimentDialog(this, QJsonDocument::fromJson(msg.content.toUtf8()).array().toVariantList());
        break;
    }

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showServerInfo()
{
    if(!isOnline)
        QMessageBox::warning(this, "请先连接服务器", "服务器未连接，请先连接服务器！");
    else
        QMessageBox::information(this, "服务器公告", this->notice);
}
