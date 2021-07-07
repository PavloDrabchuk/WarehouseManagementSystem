#ifndef SEND_MAILS_H
#define SEND_MAILS_H

#include <QWidget>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QVector>
#include <QMessageBox>

#include "add_customers.h"

namespace Ui {
class Send_mails;
}

class Send_mails : public QWidget
{
    Q_OBJECT

public:
    explicit Send_mails(QWidget *parent = nullptr);
    ~Send_mails();

    void clearAndUpdateForms();

signals:
    void goToHomePage();


private slots:
    void on_sendMailPushButton_clicked();

    void on_addCustomersPushButton_clicked();

    void updateListWidget();

    void on_backToMainMenuWindow_clicked();

private:
    Ui::Send_mails *ui;
    QVector<QString> checkedEmails;

    Add_Customers *addCustomersWindow;

    void addItemToListWidget();

    bool checkInputForms();

};

#endif // SEND_MAILS_H
