#include "add_new_users.h"
#include "ui_add_new_users.h"

Add_new_users::Add_new_users(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_new_users)
{
    ui->setupUi(this);

    qDebug()<<"******ADD NEW USERS*******";
    this->setWindowIcon(QIcon("images/logo_warehouse.png"));

    QStringList statusList={"Користувач","Редактор","Адміністратор"};
    ui->statusComboBox->addItems(statusList);

    ui->errorNameLabel->setVisible(false);
    ui->errorSurnameLabel->setVisible(false);
    ui->errorLoginLabel->setVisible(false);
    ui->errorShortPasswordLabel->setVisible(false);
    ui->errorPasswordPutLabel->setVisible(false);
    ui->errorPhoneNumberLabel->setVisible(false);
    ui->errorEmailLabel->setVisible(false);
}

Add_new_users::~Add_new_users()
{
    delete ui;
}

void Add_new_users::clearAndUpdateForms()
{
    ui->nameLineEdit->clear();
    ui->surnameLineEdit->clear();
    ui->loginLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->passwordRepeatLineEdit->clear();
    ui->phoneNumberLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->statusComboBox->setCurrentIndex(0);

    ui->errorNameLabel->setVisible(false);
    ui->errorSurnameLabel->setVisible(false);
    ui->errorLoginLabel->setVisible(false);
    ui->errorShortPasswordLabel->setVisible(false);
    ui->errorPasswordPutLabel->setVisible(false);
    ui->errorPhoneNumberLabel->setVisible(false);
    ui->errorEmailLabel->setVisible(false);

}

void Add_new_users::on_addUserPushButton_clicked()
{
    if(checkInputForms()){

        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Додавання нового користувача","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){
            QString status=ui->statusComboBox->currentText();
            QString dbStatus;

            if(status=="Користувач") dbStatus="user";
            if(status=="Редактор") dbStatus="editor";
            if(status=="Адміністратор") dbStatus="admin";

            ui->errorPasswordPutLabel->setVisible(false);
            QString insertQuery="INSERT INTO `warehouse`.`users` (`name`, `surname`, `login`, `password`, `status`, `phone`, `email`) VALUES "
                                " ('"+ui->nameLineEdit->text()+"', '"+ui->surnameLineEdit->text()+"', '"+ui->loginLineEdit->text()+"', '"+getHashPassword(ui->passwordLineEdit->text())+"', '"+dbStatus+"', '"+ui->phoneNumberLineEdit->text()+"', '"+ui->emailLineEdit->text()+"')";
            QSqlQuery query(insertQuery);
            query.clear();
            QMessageBox::about(this,"Інформація про створення нового користувача","Користувача успішно додано.");
            emit updateUsersList();
            this->close();
        }
    }else{
        QMessageBox::warning(this,"Помилка при створенні користувача","Користувача не створено, перевірте правильність введених даних.");
    }
}

void Add_new_users::on_backPushButton_clicked()
{
    this->close();
}

QString Add_new_users::getHashPassword(QString pass)
{
    QByteArray pswNsalt (pass.toStdString().c_str()) ;
    pswNsalt.append("&B3vS&K/d$&XT*{l`X~{u${~d5{UC)OJ=:V]Tl1<pfn;m:zl+C~;v~~X+u~3*Y&U") ;
    QString hashedSaltedPsw = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Keccak_512).toHex() ;
    return hashedSaltedPsw;
}

bool Add_new_users::checkInputForms()
{
    bool result=true;

    if(ui->nameLineEdit->text()=="" || ui->nameLineEdit->text()=="-"){
        result=false;
        ui->errorNameLabel->setVisible(true);
    }
    if(ui->surnameLineEdit->text()=="" || ui->surnameLineEdit->text()=="-"){
        result=false;
        ui->errorSurnameLabel->setVisible(true);
    }

    if(ui->loginLineEdit->text().length()<6){
        result= false;
        ui->errorLoginLabel->setVisible(true);
    }

    QSqlQuery query("SELECT DISTINCT `login` FROM `warehouse`.`users`");
    while (query.next()) {

        if(query.value(0)==ui->loginLineEdit->text()){
            result=false;
            break;
        }
    }
    if(ui->passwordLineEdit->text()!=ui->passwordRepeatLineEdit->text()){
        result=false;
        ui->errorPasswordPutLabel->setVisible(true);
    }
    if(ui->passwordLineEdit->text().length()<6){
        result=false;
        ui->errorShortPasswordLabel->setVisible(true);
    }

    if(ui->phoneNumberLineEdit->text()=="" || ui->phoneNumberLineEdit->text()=="-"){
        result=false;
        ui->errorPhoneNumberLabel->setVisible(true);
    }
    if(ui->emailLineEdit->text()=="" || ui->emailLineEdit->text()=="-"){
        result=false;
        ui->errorEmailLabel->setVisible(true);
    }
    return result;
}



void Add_new_users::on_passwordRepeatLineEdit_editingFinished()
{
    if(ui->passwordLineEdit->text()!=ui->passwordRepeatLineEdit->text()){
        ui->errorPasswordPutLabel->setVisible(true);
    } else {
        ui->errorPasswordPutLabel->setVisible(false);
    }
}



void Add_new_users::on_loginLineEdit_editingFinished()
{
    if(ui->loginLineEdit->text().length()<6){
        ui->errorLoginLabel->setVisible(true);
        ui->errorLoginLabel->setText("Короткий логін");
    }else{
        ui->errorLoginLabel->setVisible(false);
    }

    QSqlQuery query("SELECT DISTINCT `login` FROM `warehouse`.`users`");
    while (query.next()) {

        if(query.value(0)==ui->loginLineEdit->text()){
            ui->errorLoginLabel->setVisible(true);
            ui->errorLoginLabel->setText("Такий логін вже існує");
            break;
        }
    }
}

void Add_new_users::on_nameLineEdit_editingFinished()
{
    if(ui->nameLineEdit->text()=="" || ui->nameLineEdit->text()=="-"){
        ui->errorNameLabel->setVisible(true);
    }else{
        ui->errorNameLabel->setVisible(false);
    }
}

void Add_new_users::on_surnameLineEdit_editingFinished()
{
    if(ui->surnameLineEdit->text()=="" || ui->surnameLineEdit->text()=="-"){
        ui->errorSurnameLabel->setVisible(true);
    }else{
        ui->errorSurnameLabel->setVisible(false);
    }
}

void Add_new_users::on_phoneNumberLineEdit_editingFinished()
{
    if(ui->phoneNumberLineEdit->text()=="" || ui->phoneNumberLineEdit->text()=="-"){
        ui->errorPhoneNumberLabel->setVisible(true);
    }else{
        ui->errorPhoneNumberLabel->setVisible(false);
    }
}

void Add_new_users::on_emailLineEdit_editingFinished()
{
    if(ui->emailLineEdit->text()=="" || ui->emailLineEdit->text()=="-"){
        ui->errorEmailLabel->setVisible(true);
    }else{
        ui->errorEmailLabel->setVisible(false);
    }
}

void Add_new_users::on_passwordLineEdit_editingFinished()
{
    if(ui->passwordLineEdit->text().length()<6){
        ui->errorShortPasswordLabel->setVisible(true);
    }else{
        ui->errorShortPasswordLabel->setVisible(false);
    }
}
