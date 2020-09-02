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

    game.setPrinter(std::bind(&MainWindow::printer, this));
    game.setOverHandler(std::bind(&MainWindow::overHandler, this));
    network.setOfflineWarning(std::bind(&MainWindow::offlineWarning, this));
    network.setShowAchievement(std::bind(&MainWindow::showAchievement, this, std::placeholders::_1));
    network.setShowServerInfo(std::bind(&MainWindow::showServerInfo, this));
    network.setConnectFailed([=](){ QMessageBox::warning(this, "连接失败", "服务器连接失败，可能是目标服务器不在线，也可能是目标服务器地址填写不正确！"); });

    game.start();

}

MainWindow::~MainWindow()
{
    delete ui;
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

    ui->statusbar->addPermanentWidget(ui->label_hasUndo);
    ui->label_hasUndo->setToolTip("本局游戏内是否使用过Undo按钮。若使用过Undo按钮，则本局游戏成绩将不被允许保存至服务器中。");

    QLabel *isOnline = new QLabel(this);
    isOnline->setText("离线");
    isOnline->setStyleSheet("color:red");
    ui->statusbar->addPermanentWidget(isOnline);
    connect(&network, &Network::ServerConnected, isOnline, [=](){ isOnline->setText("在线"); isOnline->setStyleSheet("color:green;"); });
    connect(&network, &Network::ServerDisconnected, isOnline, [=](){ isOnline->setText("离线"); isOnline->setStyleSheet("color:red;"); });

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
        network.connectToServer(info.second, IP, port);
    });
    /*连接服务器Action*/

    /*服务器信息Action*/
    connect(ui->action_serverInfo, &QAction::triggered, this, &MainWindow::showServerInfo);
    /*服务器信息Action*/

    /*使用教程Action*/
    connect(ui->action_instruction, &QAction::triggered, [](){ QDesktopServices::openUrl(QUrl("https://github.com/AdamXuD/2048Game/blob/master/README.md")); });
    /*使用教程Action*/

    /*关于本游戏Action*/
    connect(ui->action_aboutGame, &QAction::triggered, [=](){
        QMessageBox::information(this, "关于本游戏", QString("<p> 作者：<a href=\"https://paopaopaoge.xyz/\" target=\"_blank\">AdamXuD</a> </p> <p> 项目名称：<a href=\"https://github.com/AdamXuD/2048Game\" target=\"_blank\">2048Game</a> </p> <p> 版本号： %1 </p>").arg(VERSION));
    });
    /*关于本游戏Action*/

    /*匹配对手Action*/
    connect(ui->action_match, &QAction::triggered, [=](){ network.sendMatchQuery(); });
    /*匹配对手Action*/

    /*拉取个人成绩列表Action*/
    connect(ui->action_queryPersonal, &QAction::triggered, [=](){ network.sendPersonalAchievementQuery(); });
    /*拉取个人成绩列表Action*/

    /*拉取所有成绩列表Action*/
    connect(ui->action_queryAll, &QAction::triggered, [=](){ network.sendAllAchievementQuery(); });
    /*拉取所有成绩列表Action*/

    /*回退（Undo）操作Btn*/
    connect(ui->btn_undo, &QPushButton::clicked, [=](){
        if(network.isConnected() && game.getGameFlag().hasUndo == false)
            if(QMessageBox::No == QMessageBox::warning(this, "您确定吗?", "您现在是在线游戏模式！\n本局游戏内您一旦使用过撤回按钮，本次成绩将不再允许上传至服务器。\n您确定您要使用撤回按钮吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
                return;
        game.undo();
    });
    /*回退（Undo）操作Btn*/

    /*重新开始（Restart）操作Btn*/
    connect(ui->btn_restart, &QPushButton::clicked, [=](){ game.start(); });
    /*重新开始（Restart）操作Btn*/

    /*从本地查询成绩Action*/
    connect(ui->action_queryFromLocal, &QAction::triggered, [=](){ AchievementDialog::showAchievementDialog(this, db.queryScore(), true); });
    /*从本地查询成绩Action*/
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

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Key key;
    switch (event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up: key = Key::UP; break;
    case Qt::Key_S:
    case Qt::Key_Down: key = Key::DOWN; break;
    case Qt::Key_A:
    case Qt::Key_Left: key = Key::LEFT; break;
    case Qt::Key_D:
    case Qt::Key_Right: key = Key::RIGHT; break;
    }
    game.moveOperation(key);
}

void MainWindow::overHandler()
{
    if(network.isConnected() && game.getGameFlag().hasUndo == false && game.getGameFlag().hasUploaded == false)
        if(QMessageBox::Yes == QMessageBox::information(this, "是否上传成绩", QString("游戏结束！ 您的分数是 %1 ！\n是否将成绩上传至服务器呢？\n（服务器会长期保存您的成绩，并且在线状态下您能随时查询您的成绩）").arg(game.getGameData().score), QMessageBox::Yes | QMessageBox::No))
        {
            network.sendUploadScore(game.getGameData().score, game.getMaxNum());
            game.setUploaded();
        }

    if(QMessageBox::Yes == QMessageBox::information(this, "游戏结束", QString("游戏结束！ 您的分数是 %1 ！\n您希望马上重新开始新一局游戏吗？").arg(game.getGameData().score), QMessageBox::Yes | QMessageBox::No))
    {
        db.insertScore(game.getGameData().score, game.getMaxNum(), game.getGameFlag().hasUndo);
        game.start();
    }
}

void MainWindow::printer()
{
    ui->label_hasUndo->setText(game.getGameFlag().hasUndo ? "true" : "false");
    ui->label_hasUndo->setStyleSheet("color:" + QString(game.getGameFlag().hasUndo ? "red" : "green"));

    ui->tableWidget_map->clearContents();
    ui->label_score->setText(QString("%1").arg(game.getGameData().score));
    for(int row = 0; row < 4; row++)
        for(int col = 0; col < 4; col++)
            if(game.getGameData().map[row][col] != 0)
            {
                QTableWidgetItem *i = new QTableWidgetItem(QString("%1").arg(game.getGameData().map[row][col]));
                i->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_map->setItem(row, col, i);
                ui->tableWidget_map->item(row, col)->setBackgroundColor(getItemColor(game.getGameData().map[row][col]));
            }

}

void MainWindow::offlineWarning()
{
    QMessageBox::warning(this, "请先连接服务器", "服务器未连接，请先连接服务器！");
}

void MainWindow::showServerInfo()
{
    if(!network.isConnected())
        offlineWarning();
    else
        QMessageBox::information(this, "服务器公告", network.getServerNotice());
}

void MainWindow::showAchievement(QVariantList list)
{
    AchievementDialog::showAchievementDialog(this, list);
}

