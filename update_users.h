#ifndef UPDATE_USERS_H
#define UPDATE_USERS_H

#include <QDialog>
#include <QtDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>

namespace Ui {
class Update_Users;
}

class Update_Users : public QDialog
{
    Q_OBJECT

public:
    explicit Update_Users(QWidget *parent = nullptr);
    ~Update_Users();

    void setUserId(int id);
    void insertUserInfo(int userId);
    void clearAndUpdateForms();

signals:
    void updateUsersList();

private slots:

    void on_backPushButton_clicked();

    void on_passwordRepeatLineEdit_editingFinished();

    void on_loginLineEdit_editingFinished();

    void on_nameLineEdit_editingFinished();

    void on_surnameLineEdit_editingFinished();

    void on_passwordLineEdit_editingFinished();

    void on_updateUserInfoPushButton_clicked();

private:
    Ui::Update_Users *ui;

    QString getHashPassword(QString pass);

    bool checkInputForms();

    int userId;
};

#endif // UPDATE_USERS_H
