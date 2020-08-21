#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("2048");

    ui->tableWidget_map->setRowCount(4);
    ui->tableWidget_map->setColumnCount(4);

    ui->tableWidget_map->verticalHeader()->setVisible(false);
    ui->tableWidget_map->horizontalHeader()->setVisible(false);
    ui->tableWidget_map->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_map->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget_map->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_map->setFocusPolicy(Qt::NoFocus);

    QFont font;
    font.setPointSize(18);
    font.setBold(true);
    ui->tableWidget_map->setFont(font);

    QLabel *tips = new QLabel(this);
    tips->setText("使用W, A, S, D或者方向键进行控制！");
    ui->statusbar->addWidget(tips);

    start();
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
    return QColor(199, 237, 253);
}

void MainWindow::printMap()
{
    ui->tableWidget_map->clearContents();
    ui->label_score->setText(QString("%1").arg(g.score));
    for(int row = 0; row < 4; row++)
        for(int col = 0; col < 4; col++)
            if(g.map[row][col] != 0)
            {
                QTableWidgetItem *i = new QTableWidgetItem(QString("%1").arg(g.map[row][col]));
                i->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget_map->setItem(row, col, i);
                ui->tableWidget_map->item(row, col)->setBackgroundColor(getItemColor(g.map[row][col]));
            }

}

void MainWindow::start()
{
    memset(g.map, 0, sizeof(int[4][4]));
    g.score = 0;
    g.randNum();
    g.randNum();
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
        printMap();
    }
    if(g.isOver())
    {
        if(QMessageBox::Yes == QMessageBox::information(this, "Game Over", QString("Game Over. Your score is %1!\nWould you want to restart? ").arg(g.score), QMessageBox::Yes | QMessageBox::No))
            start();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

