#include "achievementdialog.h"
#include "ui_achievementdialog.h"

AchievementDialog::AchievementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AchimentDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("历史成绩");
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->btn_OK, &QPushButton::clicked, this, &AchievementDialog::close);

    show();
}

void AchievementDialog::showAchievementDialog(QWidget *parent, QVariantList list, const bool isLocal)
{
    AchievementDialog *d = new AchievementDialog(parent);

    d->ui->tableWidget_achievement->setSelectionMode(QAbstractItemView::NoSelection);
    d->ui->tableWidget_achievement->setFocusPolicy(Qt::NoFocus);
    d->ui->tableWidget_achievement->setEditTriggers(QAbstractItemView::NoEditTriggers);

    d->ui->tableWidget_achievement->setColumnCount(4);
    d->ui->tableWidget_achievement->setHorizontalHeaderLabels(QStringList() << "玩家" << "分数" << "最大数字" << "上传时间");
    d->ui->tableWidget_achievement->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->tableWidget_achievement->horizontalHeader()->setStretchLastSection(true);

    for(int i = 0; i < list.size(); i++)
    {
        d->ui->tableWidget_achievement->setRowCount(d->ui->tableWidget_achievement->rowCount() + 1);

        QTableWidgetItem *first = new QTableWidgetItem;
        first->setText(list[i].toMap()["uname"].toString());
        d->ui->tableWidget_achievement->setItem(i, 0, first);

        QTableWidgetItem *second = new QTableWidgetItem;
        second->setText(QString("%1").arg(list[i].toMap()["score"].toInt()));
        d->ui->tableWidget_achievement->setItem(i, 1, second);

        QTableWidgetItem *third = new QTableWidgetItem;
        third->setText(QString("%1").arg(list[i].toMap()["maxNum"].toInt()));
        d->ui->tableWidget_achievement->setItem(i, 2, third);

        QTableWidgetItem *forth = new QTableWidgetItem;
        forth->setText(list[i].toMap()["uploadTime"].toString());
        d->ui->tableWidget_achievement->setItem(i, 3, forth);

        if(isLocal)
        {
            QString hasUndo = "是否使用过Undo:" + QString(list[i].toMap()["hasUndo"].toBool() ? "是" : "否");
            first->setToolTip(hasUndo);
            second->setToolTip(hasUndo);
            third->setToolTip(hasUndo);
            forth->setToolTip(hasUndo);
        }
    }
}

AchievementDialog::~AchievementDialog()
{
    delete ui;
}
