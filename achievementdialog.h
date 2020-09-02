#ifndef ACHIMENTDIALOG_H
#define ACHIMENTDIALOG_H

#include "common.h"

namespace Ui {
class AchimentDialog;
}

class AchievementDialog : public QDialog
{
    Q_OBJECT
    Ui::AchimentDialog *ui;

public:
    static void showAchievementDialog(QWidget *parent = nullptr, QVariantList list = QVariantList(), const bool isLocal = false);

private:
    explicit AchievementDialog(QWidget *parent = nullptr);
    ~AchievementDialog();

};

#endif // ACHIMENTDIALOG_H
