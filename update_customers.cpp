#include "update_customers.h"
#include "ui_update_customers.h"

Update_Customers::Update_Customers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update_Customers)
{
    ui->setupUi(this);
qDebug()<<"******ADD CUSTOMERS*******";
    ui->nameErrorMessageLabel->setVisible(false);
    ui->surnameErrorMessageLabel->setVisible(false);
    ui->emailErrorMessageLabel->setVisible(false);
    ui->phoneNumberErrorMessageLabel->setVisible(false);
}

Update_Customers::~Update_Customers()
{
    delete ui;
}

void Update_Customers::clearAndUpdateForms()
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

void Update_Customers::setUserId(int id)
{
    userId=id;
}

void Update_Customers::on_addCustomersPushButton_clicked()
{
    //    QString name=ui->nameLineEdit->text(),
    //            surname=ui->surnameLineEdit->text(),
    //            email=ui->emailLineEdit->text(),
    //            phoneNumber=ui->phoneNumberLineEdit->text();
    //    if(checkInputForms()){
    //        QSqlQuery addCostomersQuery("INSERT INTO `warehouse`.`customers_info` (`name`, `surname`, `email`, `phone`) "
    //                                    "VALUES ('"+name+"', '"+surname+"', '"+email+"', '"+phoneNumber+"')");
    //    }

    if(checkInputForms()){

        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Додавання нового користувача","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){
            //emit addGoodToTable();
            //this->close();
            QString name=ui->nameLineEdit->text(),
                    surname=ui->surnameLineEdit->text(),
                    email=ui->emailLineEdit->text(),
                    phoneNumber=ui->phoneNumberLineEdit->text();



            QSqlQuery addCostomersQuery("INSERT INTO `warehouse`.`customers_info` (`name`, `surname`, `email`, `phone`) "
                                        "VALUES ('"+name+"', '"+surname+"', '"+email+"', '"+phoneNumber+"')");
            addCostomersQuery.clear();
            qDebug()<<"added!";
            QMessageBox::about(this,"Інформація про створення нового клієнта","Клієнта успішно додано.");
            emit updateListWidget();
            this->close();
        }else{
            //ui->errorPasswordPutLabel->setVisible(true);
            //qDebug()<<"not added!";
            //QMessageBox::critical(this,"Помилка при створенні клієнта","Сталась невідома помилка, спробуйте ще раз пізніше.");
        }
    }else{
        QMessageBox::warning(this,"Помилка при створенні клієнта","Клієнта не створено, перевірте правильність введених даних.");
    }

}



void Update_Customers::on_backPushButton_clicked()
{
    emit updateListWidget();
    this->close();
}

bool Update_Customers::checkInputForms()
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

void Update_Customers::on_surnameLineEdit_editingFinished()
{
    if(ui->surnameLineEdit->text()=="" || ui->surnameLineEdit->text()=="-"){
        ui->surnameErrorMessageLabel->setVisible(true);
    }else{
        ui->surnameErrorMessageLabel->setVisible(false);
    }
}

void Update_Customers::on_nameLineEdit_editingFinished()
{
    if(ui->nameLineEdit->text()=="" || ui->nameLineEdit->text()=="-"){
        ui->nameErrorMessageLabel->setVisible(true);
    }else{
        ui->nameErrorMessageLabel->setVisible(false);
    }
}

void Update_Customers::on_emailLineEdit_editingFinished()
{
    if(ui->emailLineEdit->text()=="" || ui->emailLineEdit->text()=="-"){
        ui->emailErrorMessageLabel->setVisible(true);
    }else{
        ui->emailErrorMessageLabel->setVisible(false);
    }
}

void Update_Customers::on_phoneNumberLineEdit_editingFinished()
{
    if(ui->phoneNumberLineEdit->text()=="" || ui->phoneNumberLineEdit->text()=="-"){
        ui->phoneNumberErrorMessageLabel->setVisible(true);
    }else{
        ui->phoneNumberErrorMessageLabel->setVisible(false);
    }
}
