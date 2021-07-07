#ifndef ADD_CUSTOMERS_H
#define ADD_CUSTOMERS_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class Add_Customers;
}

class Add_Customers : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Customers(QWidget *parent = nullptr);
    ~Add_Customers();

    void clearAndUpdateForms();
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
    Ui::Add_Customers *ui;

    bool checkInputForms();
};

#endif // ADD_CUSTOMERS_H
