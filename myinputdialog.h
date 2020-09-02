#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H

#include "common.h"

namespace Ui {
class MyInputDialog;
}

class MyInputDialog : public QDialog
{
    Q_OBJECT
    Ui::MyInputDialog *ui;

public:
    static QPair<QString, QString> getPair(QWidget *parent, const QString &title, const QString &label, bool *ok = Q_NULLPTR);

signals:
    void onBtnOkClicked();
    void onBtnCancelClicked();

private:
    explicit MyInputDialog(QWidget *parent = nullptr);
    ~MyInputDialog();

};

#endif // MYINPUTDIALOG_H
