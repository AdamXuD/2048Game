#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"

class Database
{
    QSqlDatabase *db;
public:
    Database();
    ~Database();

    void insertScore(int score, int maxNum, bool hasUndo);
    QVariantList queryScore();

};

#endif // DATABASE_H
