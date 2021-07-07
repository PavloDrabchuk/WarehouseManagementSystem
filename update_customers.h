#ifndef UPDATE_CUSTOMERS_H
#define UPDATE_CUSTOMERS_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class Update_Customers;
}

class Update_Customers : public QDialog
{
    Q_OBJECT

public:
    explicit Update_Customers(QWidget *parent = nullptr);
    ~Update_Customers();

    void clearAndUpdateForms();

    void setUserId(int id);
signals:
    void updateListWidget();

private slots:
    void on_addCustomersPushButton_clicked();



    void on_backPushButton_clicked();

    void on_surnameLineEdit_editingFinished();

    void on_nameLineEdit_editingFinished();

    void on_emailLineEdit_editingFinished();

    void on_phoneNumberLineEdit_editingFinished();

private:
    Ui::Update_Customers *ui;

    bool checkInputForms();

    int userId;
};

#endif // UPDATE_CUSTOMERS_H
