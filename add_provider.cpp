#include "add_provider.h"
#include "ui_add_provider.h"

Add_Provider::Add_Provider(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Provider)
{
    ui->setupUi(this);
    qDebug()<<"******ADD PROVIDER*******";
    this->setWindowIcon(QIcon("images/logo_warehouse.png"));

    ui->nameErrorMessageLabel->setVisible(false);
    ui->adressErrorMessageLabel->setVisible(false);
    ui->emailErrorMessageLabel->setVisible(false);
    ui->phoneNumberErrorMessageLabel->setVisible(false);

}

Add_Provider::~Add_Provider()
{
    delete ui;
}

void Add_Provider::clearAndUpdateForms()
{
    ui->adressLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->phoneNumberLineEdit->clear();

    ui->adressErrorMessageLabel->setVisible(false);
    ui->nameErrorMessageLabel->setVisible(false);
    ui->emailErrorMessageLabel->setVisible(false);
    ui->phoneNumberErrorMessageLabel->setVisible(false);
}

void Add_Provider::on_addProviderPushButton_clicked()
{
    if(checkInputForms()){
        QSqlQuery getCountProviderByName("SELECT * FROM warehouse.provider WHERE `name`='"+ui->nameLineEdit->text()+"'");
        getCountProviderByName.next();
        if(getCountProviderByName.size()==0){

            QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Додавання нового постачальника","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

            if(answerExit==QMessageBox::Yes) {
                QString name=ui->nameLineEdit->text(),
                        adress=ui->adressLineEdit->text(),
                        email=ui->emailLineEdit->text(),
                        phoneNumber=ui->phoneNumberLineEdit->text();


                QSqlQuery addCostomersQuery("INSERT INTO `warehouse`.`provider` (`name`, `email`, `phones`, `adress`) "
                                            "VALUES ('"+name+"', '"+email+"', '"+phoneNumber+"', '"+adress+"')");
                addCostomersQuery.clear();

                QMessageBox::about(this,"Інформація про створення нового постачальника","Постачальника успішно додано.");
                emit updateListWidget();
                this->close();
            }
        }
        else {
            QMessageBox::warning(this,"Помилка при створенні постачальника","Такий постачальник вже існує.");
        }
    }else{
        QMessageBox::warning(this,"Помилка при створенні постачальника","Постачальника не додано, перевірте правильність введених даних.");
    }
}

void Add_Provider::on_backPushButton_clicked()
{
    emit updateListWidget();
    this->close();
}

bool Add_Provider::checkInputForms()
{
    bool result=true;

    if(ui->nameLineEdit->text()==""){
        result = false;
        ui->nameErrorMessageLabel->setVisible(true);
    }
    if(ui->adressLineEdit->text()==""){
        result = false;
        ui->adressErrorMessageLabel->setVisible(true);
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

void Add_Provider::on_nameLineEdit_editingFinished()
{
    if(ui->nameLineEdit->text()=="" || ui->nameLineEdit->text()=="-"){
        ui->nameErrorMessageLabel->setVisible(true);
    }else{
        ui->nameErrorMessageLabel->setVisible(false);
    }
}

void Add_Provider::on_emailLineEdit_editingFinished()
{
    if(ui->emailLineEdit->text()=="" || ui->emailLineEdit->text()=="-"){
        ui->emailErrorMessageLabel->setVisible(true);
    }else{
        ui->emailErrorMessageLabel->setVisible(false);
    }
}

void Add_Provider::on_phoneNumberLineEdit_editingFinished()
{
    if(ui->phoneNumberLineEdit->text()=="" || ui->phoneNumberLineEdit->text()=="-"){
        ui->phoneNumberErrorMessageLabel->setVisible(true);
    }else{
        ui->phoneNumberErrorMessageLabel->setVisible(false);
    }
}

void Add_Provider::on_adressLineEdit_editingFinished()
{
    if(ui->adressErrorMessageLabel->text()=="" || ui->adressErrorMessageLabel->text()=="-"){
        ui->adressErrorMessageLabel->setVisible(true);
    }else{
        ui->adressErrorMessageLabel->setVisible(false);
    }
}
