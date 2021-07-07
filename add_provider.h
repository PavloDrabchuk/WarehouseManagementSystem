#ifndef ADD_PROVIDER_H
#define ADD_PROVIDER_H

#include <QDialog>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class Add_Provider;
}

class Add_Provider : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Provider(QWidget *parent = nullptr);
    ~Add_Provider();

    void clearAndUpdateForms();
signals:
    void updateListWidget();

private slots:
    void on_addProviderPushButton_clicked();

    void on_backPushButton_clicked();

    void on_nameLineEdit_editingFinished();

    void on_emailLineEdit_editingFinished();

    void on_phoneNumberLineEdit_editingFinished();

    void on_adressLineEdit_editingFinished();

private:
    Ui::Add_Provider *ui;

    bool checkInputForms();
};

#endif // ADD_PROVIDER_H
