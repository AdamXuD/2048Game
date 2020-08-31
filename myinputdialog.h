#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H

#include "common.h"

namespace Ui {
class MyInputDialog;
}

class MyInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyInputDialog(QWidget *parent = nullptr);
    ~MyInputDialog();

    static QPair<QString, QString> getPair(QWidget *parent, const QString &title, const QString &label, bool *ok = Q_NULLPTR);

signals:
    void onBtnOkClicked();
    void onBtnCancelClicked();

private:
    Ui::MyInputDialog *ui;
};

#endif // MYINPUTDIALOG_H
