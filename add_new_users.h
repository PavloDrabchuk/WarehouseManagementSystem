#ifndef ADD_NEW_USERS_H
#define ADD_NEW_USERS_H

#include <QDialog>
#include <QtDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>

namespace Ui {
class Add_new_users;
}

class Add_new_users : public QDialog
{
    Q_OBJECT

public:
    explicit Add_new_users(QWidget *parent = nullptr);
    ~Add_new_users();

    void clearAndUpdateForms();

signals:
    void updateUsersList();

private slots:
    void on_addUserPushButton_clicked();

    void on_backPushButton_clicked();

    void on_passwordRepeatLineEdit_editingFinished();

    void on_loginLineEdit_editingFinished();

    void on_nameLineEdit_editingFinished();

    void on_surnameLineEdit_editingFinished();

    void on_phoneNumberLineEdit_editingFinished();

    void on_emailLineEdit_editingFinished();

    void on_passwordLineEdit_editingFinished();

private:
    Ui::Add_new_users *ui;

    QString getHashPassword(QString pass);

    bool checkInputForms();
};

#endif // ADD_NEW_USERS_H
