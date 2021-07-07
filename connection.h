#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class Connection
{
    QSqlDatabase db;
public:
    Connection();
    ~Connection();
};

#endif // CONNECTION_H
