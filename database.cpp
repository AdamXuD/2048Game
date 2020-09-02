#include "database.h"

Database::Database()
{
    this->db = new QSqlDatabase;
    *db = QSqlDatabase::addDatabase("QSQLITE");
    db->setDatabaseName(QCoreApplication::applicationDirPath() + "/gamedata.dat");
#if DEBUGMODE == true
    if(!db->open())
        qDebug() << "QDatabase Error : " << db->lastError().text().toUtf8().data() << endl;
#endif
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS tableScore (uid INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, score INTEGER DEFAULT (0), maxNum INTEGER NOT NULL DEFAULT (0), hasUndo BOOLEAN DEFAULT false NOT NULL, uploadTime DATETIME NOT NULL DEFAULT (datetime('now', 'localtime')));");
}

Database::~Database()
{
    delete this->db;
}

void Database::insertScore(int score, int maxNum, bool hasUndo)
{
    QSqlQuery query;
    QString command = QString("INSERT INTO tableScore VALUES (null, %1, %2, %3, null);").arg(score).arg(maxNum).arg(hasUndo);
#if DEBUGMODE == true
    if(!query.exec(command))
        qDebug() << "SQL query ERROR at " << command.toUtf8().data() << " : " << db->lastError().text().toUtf8().data() << endl;
#endif
}

QVariantList Database::queryScore()
{
    QSqlQuery query;
    QString command;
        command = QString("SELECT score, maxNum, hasUndo, uploadTime FROM tableScore ORDER BY score DESC;");

    if(!query.exec(command))
    {
#if DEBUGMODE == true
        qDebug() << "SQL query ERROR at " << command.toUtf8().data() << " : " << db->lastError().text().toUtf8().data() << endl;
#endif
        return QVariantList();
    }

    if(query.size() == 0)
        return QVariantList();

    QVariantList resultList;
    QVariantMap tmpObj;
    while(query.next())
    {
        tmpObj.clear();

        tmpObj["uname"] = "me";
        tmpObj["score"] = query.value(0).toInt();
        tmpObj["maxNum"] = query.value(1).toInt();
        tmpObj["hasUndo"] = query.value(2).toBool();
        tmpObj["uploadTime"] = query.value(3).toString();

        resultList.push_back(tmpObj);
    }
    return resultList;
}
