#include "connection.h"

Connection::Connection()
{  
    qDebug()<<"******CONNECTION*******";

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("warehouse");
    db.setUserName("WarehouseAdmin");
    db.setPassword("admin_password");

    bool ok = db.open();
      if (ok){
        qDebug()<<"connection is OK!";
    } else {
        qDebug()<<"connection is failed";
    }
}
Connection::~Connection(){
    db.close();
}


