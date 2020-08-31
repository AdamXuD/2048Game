#ifndef ACHIMENTDIALOG_H
#define ACHIMENTDIALOG_H

#include "common.h"

namespace Ui {
class AchimentDialog;
}

class AchimentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AchimentDialog(QWidget *parent = nullptr);
    ~AchimentDialog();

    static void showAchimentDialog(QWidget *parent = nullptr, QVariantList list = QVariantList());

private:
    Ui::AchimentDialog *ui;
};

#endif // ACHIMENTDIALOG_H
