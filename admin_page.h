#ifndef ADMIN_PAGE_H
#define ADMIN_PAGE_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QListWidgetItem>

#include "add_new_users.h"
#include "update_users.h"

namespace Ui {
class Admin_page;
}

class Admin_page : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_page(QWidget *parent = nullptr);
    ~Admin_page();

    void clearAndUpdateForms();
    void setUserId(int id);

signals:
    void goToHomePage();
    void updateWriteOffGoodsPushButton();

private slots:

    void on_addNewUserPushButton_clicked();

    void on_generateTablePushButton_clicked();

    void on_pushButton_clicked();

    void on_updateUserInfoPushButton_clicked();

    void on_pushButton_2_clicked();

    void on_trunkateTablesPushButton_clicked();

    void on_checkAllCheckBox_stateChanged(int arg1);

    void on_createDatabaseStructurePushButton_clicked();

    void on_checkAllUsersCheckBox_stateChanged(int arg1);

    void on_deleteUsersPushButton_clicked();

    void updateUsersListWidget();

private:
    Ui::Admin_page *ui;

    int randomBetween(int low, int high);

    Add_new_users *addNewUsersWindow;
    Update_Users *updateUsersWindow;

    void addUsersToComboBox();

    void addItemToListWidget();
    void addItemToUsersListWidget();

    QVector<QString> checkedTables;
    QVector<QString> checkedUsers;

    QSqlDatabase dbForNewShema;

    int userId;

    int createExpirationDate();
};

#endif // ADMIN_PAGE_H
