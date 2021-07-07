#include "add_new_user.h"
#include "ui_add_new_user.h"

Add_New_User::Add_New_User(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Add_New_User)
{
    ui->setupUi(this);
    QStringList statusList={"Користувач", "Редактор", "Адміністратор"};
    ui->statusComboBox->addItems(statusList);


}

Add_New_User::~Add_New_User()
{
    delete ui;
}

void Add_New_User::on_addUserPushButton_clicked()
{
    //INSERT INTO `warehouse`.`users` (`name`, `surname`, `login`, `password`, `status`, `phone`, `email`) VALUES ('v', 'f', 'l', 'vffv', 'user', 'j', 'b');
    QString insertQuery="INSERT INTO `warehouse`.`users` (`name`, `surname`, `login`, `password`, `status`, `phone`, `email`) VALUES ('v', 'f', 'l', 'vffv', 'user', 'j', 'b')";
    QSqlQuery query(insertQuery);
    qDebug()<<" >>> sql:  "+insertQuery;
    query.clear();
}
