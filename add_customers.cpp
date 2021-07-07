#include "add_customers.h"
#include "ui_add_customers.h"

Add_Customers::Add_Customers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Customers)
{
    ui->setupUi(this);
    qDebug()<<"******ADD CUSTOMERS*******";
    this->setWindowIcon(QIcon("images/logo_warehouse.png"));

    ui->nameErrorMessageLabel->setVisible(false);
    ui->surnameErrorMessageLabel->setVisible(false);
    ui->emailErrorMessageLabel->setVisible(false);
    ui->phoneNumberErrorMessageLabel->setVisible(false);



}

Add_Customers::~Add_Customers()
{
    delete ui;
}

void Add_Customers::clearAndUpdateForms()
{
    ui->surnameLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->phoneNumberLineEdit->clear();

    ui->surnameErrorMessageLabel->setVisible(false);
    ui->nameErrorMessageLabel->setVisible(false);
    ui->emailErrorMessageLabel->setVisible(false);
    ui->phoneNumberErrorMessageLabel->setVisible(false);
}

void Add_Customers::on_addCustomersPushButton_clicked()
{
    if(checkInputForms()){

        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Додавання нового користувача","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes) {
            QString name=ui->nameLineEdit->text(),
                    surname=ui->surnameLineEdit->text(),
                    email=ui->emailLineEdit->text(),
                    phoneNumber=ui->phoneNumberLineEdit->text();

            QSqlQuery addCostomersQuery("INSERT INTO `warehouse`.`customers_info` (`name`, `surname`, `email`, `phone`) "
                                        "VALUES ('"+name+"', '"+surname+"', '"+email+"', '"+phoneNumber+"')");
            addCostomersQuery.clear();

            QMessageBox::about(this,"Інформація про створення нового клієнта","Клієнта успішно додано.");
            emit updateListWidget();
            this->close();
        }
    }else{
        QMessageBox::warning(this,"Помилка при створенні клієнта","Клієнта не створено, перевірте правильність введених даних.");
    }
}

void Add_Customers::on_backPushButton_clicked()
{
    emit updateListWidget();
    this->close();
}

bool Add_Customers::checkInputForms()
{
    bool result=true;

    if(ui->nameLineEdit->text()==""){
        result = false;
        ui->nameErrorMessageLabel->setVisible(true);
    }
    if(ui->surnameLineEdit->text()==""){
        result = false;
        ui->surnameErrorMessageLabel->setVisible(true);
    }
    if(ui->emailLineEdit->text()==""){
        result = false;
        ui->emailErrorMessageLabel->setVisible(true);
    }
    if(ui->phoneNumberLineEdit->text()==""){
        result = false;
        ui->phoneNumberErrorMessageLabel->setVisible(true);
    }
    return result;
}

void Add_Customers::on_surnameLineEdit_editingFinished()
{
    if(ui->surnameLineEdit->text()=="" || ui->surnameLineEdit->text()=="-"){
        ui->surnameErrorMessageLabel->setVisible(true);
    }else{
        ui->surnameErrorMessageLabel->setVisible(false);
    }
}

void Add_Customers::on_nameLineEdit_editingFinished()
{
    if(ui->nameLineEdit->text()=="" || ui->nameLineEdit->text()=="-"){
        ui->nameErrorMessageLabel->setVisible(true);
    }else{
        ui->nameErrorMessageLabel->setVisible(false);
    }
}

void Add_Customers::on_emailLineEdit_editingFinished()
{
    if(ui->emailLineEdit->text()=="" || ui->emailLineEdit->text()=="-"){
        ui->emailErrorMessageLabel->setVisible(true);
    }else{
        ui->emailErrorMessageLabel->setVisible(false);
    }
}

void Add_Customers::on_phoneNumberLineEdit_editingFinished()
{
    if(ui->phoneNumberLineEdit->text()=="" || ui->phoneNumberLineEdit->text()=="-"){
        ui->phoneNumberErrorMessageLabel->setVisible(true);
    }else{
        ui->phoneNumberErrorMessageLabel->setVisible(false);
    }
}
