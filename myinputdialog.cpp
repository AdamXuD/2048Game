#include "myinputdialog.h"
#include "ui_myinputdialog.h"


MyInputDialog::MyInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyInputDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton_ok, &QPushButton::clicked, this, &MyInputDialog::onBtnOkClicked);
    connect(ui->pushButton_cancel, &QPushButton::clicked, this, &MyInputDialog::onBtnCancelClicked);
#if DEBUGMODE == true
    ui->lineEdit_serverAddress->setText("127.0.0.1:12345");
    ui->lineEdit_userName->setText("Adam");
#endif
}

MyInputDialog::~MyInputDialog()
{
    delete ui;
}

QPair<QString, QString> MyInputDialog::getPair(QWidget *parent, const QString &title, const QString &label, bool *ok)
{
    bool _ok = false;
    MyInputDialog d(parent);
    d.setWindowTitle(title);
    d.ui->label_tips->setText(label);
    d.ui->lineEdit_userName->setMaxLength(32);
    connect(&d, &MyInputDialog::onBtnOkClicked, [&](){ _ok = true; d.close(); });
    connect(&d, &MyInputDialog::onBtnCancelClicked, [&](){ _ok = false; d.close(); });
    while(true)
    {
        d.exec();
        if(!_ok) break;
        int pos = 0;
        QString serverAddress = d.ui->lineEdit_serverAddress->text();
        QRegExp regExpAdd("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\:([0-9]|[1-9]\\d|[1-9]\\d{2}|[1-9]\\d{3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])\\b");
        QRegExpValidator patternAddr(regExpAdd);
        if(patternAddr.validate(serverAddress, pos) != QValidator::Acceptable)
            QMessageBox::warning(nullptr, "格式错误", "服务器地址格式错误，请重新输入服务器地址！\n格式为：IP:端口号\n请注意冒号必须为半角符号！");
        else
            break;
    }

    if(ok != nullptr) *ok = _ok;

    if(_ok)
        return QPair<QString, QString>(d.ui->lineEdit_serverAddress->text(), d.ui->lineEdit_userName->text());
    else
        return QPair<QString, QString>();
}
