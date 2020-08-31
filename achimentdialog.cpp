#include "achimentdialog.h"
#include "ui_achimentdialog.h"

AchimentDialog::AchimentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AchimentDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
    setWindowTitle("Achiment List");
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->btn_OK, &QPushButton::clicked, this, &AchimentDialog::close);

    show();
}

void AchimentDialog::showAchimentDialog(QWidget *parent, QVariantList list)
{
    AchimentDialog *d = new AchimentDialog(parent);

    d->ui->tableWidget_achievement->setColumnCount(2);
    d->ui->tableWidget_achievement->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    d->ui->tableWidget_achievement->setHorizontalHeaderLabels(QStringList() << "Name" << "Score");

    for(int i = 0; i < list.size(); i++)
    {
        QTableWidgetItem *first = new QTableWidgetItem;
        first->setText(list[i].toMap()["uname"].toString());
        QTableWidgetItem *second = new QTableWidgetItem;
        second->setText(QString("%1").arg(list[i].toMap()["score"].toInt()));

        d->ui->tableWidget_achievement->setRowCount(d->ui->tableWidget_achievement->rowCount() + 1);
        d->ui->tableWidget_achievement->setItem(i, 0, first);
        d->ui->tableWidget_achievement->setItem(i, 1, second);
    }

}

AchimentDialog::~AchimentDialog()
{
    delete ui;
}
