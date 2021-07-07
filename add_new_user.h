#ifndef ADD_NEW_USER_H
#define ADD_NEW_USER_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtDebug>



namespace Ui {
class Add_New_User;
}

class Add_New_User : public QWidget
{
    Q_OBJECT

public:
    explicit Add_New_User(QWidget *parent = nullptr);
    ~Add_New_User();

private slots:
    void on_addUserPushButton_clicked();

private:
    Ui::Add_New_User *ui;


};

#endif // ADD_NEW_USER_H
