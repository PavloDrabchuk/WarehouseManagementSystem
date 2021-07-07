#include "update_users.h"
#include "ui_update_users.h"

Update_Users::Update_Users(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Update_Users)
{
    ui->setupUi(this);

    qDebug()<<"******UPDATE USERS*******";
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

Update_Users::~Update_Users()
{
    delete ui;
}

void Update_Users::setUserId(int id)
{
    userId=id;
}

void Update_Users::on_backPushButton_clicked()
{
    this->close();
}

QString Update_Users::getHashPassword(QString pass)
{
    QByteArray pswNsalt (pass.toStdString().c_str()) ;
    pswNsalt.append("&B3vS&K/d$&XT*{l`X~{u${~d5{UC)OJ=:V]Tl1<pfn;m:zl+C~;v~~X+u~3*Y&U") ;
    QString hashedSaltedPsw = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Keccak_512).toHex() ;
    return hashedSaltedPsw;
}

bool Update_Users::checkInputForms()
{
    bool result=true;

    if(ui->loginLineEdit->text().length()>0 && ui->loginLineEdit->text().length()<6){
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

    if(ui->passwordLineEdit->text().length()>0 && ui->passwordLineEdit->text().length()<6){
        result=false;
        ui->errorShortPasswordLabel->setVisible(true);
    }

    query.clear();
    return result;
}

void Update_Users::insertUserInfo(int userId)
{
    QSqlQuery getUserByIdQuery("SELECT * FROM warehouse.users WHERE `id`='"+QString::number(userId)+"'");

    getUserByIdQuery.next();
    ui->nameLineEdit->setText(getUserByIdQuery.value(1).toString());
    ui->surnameLineEdit->setText(getUserByIdQuery.value(2).toString());
    ui->loginLineEdit->setPlaceholderText(getUserByIdQuery.value(3).toString());

    if(getUserByIdQuery.value(5).toString()=="admin"){
        ui->statusComboBox->setCurrentIndex(2);
    }  else if(getUserByIdQuery.value(5).toString()=="editor"){
        ui->statusComboBox->setCurrentIndex(1);
    } else if(getUserByIdQuery.value(5).toString()=="user"){
        ui->statusComboBox->setCurrentIndex(0);
    }

    ui->phoneNumberLineEdit->setText(getUserByIdQuery.value(6).toString());
    ui->emailLineEdit->setText(getUserByIdQuery.value(7).toString());

    getUserByIdQuery.clear();
}

void Update_Users::clearAndUpdateForms()
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

void Update_Users::on_passwordRepeatLineEdit_editingFinished()
{
    if(ui->passwordLineEdit->text()!=ui->passwordRepeatLineEdit->text()){
        ui->errorPasswordPutLabel->setVisible(true);
    }else{
        ui->errorPasswordPutLabel->setVisible(false);
    }
}

void Update_Users::on_loginLineEdit_editingFinished()
{
    if(ui->loginLineEdit->text().length()>0 && ui->loginLineEdit->text().length()<6){
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

void Update_Users::on_nameLineEdit_editingFinished()
{
    if(ui->nameLineEdit->text()=="" || ui->nameLineEdit->text()=="-"){
        ui->errorNameLabel->setVisible(true);
    }else{
        ui->errorNameLabel->setVisible(false);
    }
}

void Update_Users::on_surnameLineEdit_editingFinished()
{
    if(ui->surnameLineEdit->text()=="" || ui->surnameLineEdit->text()=="-"){
        ui->errorSurnameLabel->setVisible(true);
    }else{
        ui->errorSurnameLabel->setVisible(false);
    }
}

void Update_Users::on_passwordLineEdit_editingFinished()
{
    if(ui->passwordLineEdit->text().length()>0  && ui->passwordLineEdit->text().length()<6){
        ui->errorShortPasswordLabel->setVisible(true);
    }else{
        ui->errorShortPasswordLabel->setVisible(false);
    }
}

void Update_Users::on_updateUserInfoPushButton_clicked()
{
    if(checkInputForms()){
        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Зміна інформації про користувача","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){

            QString status=ui->statusComboBox->currentText();
            QString dbStatus;

            if(status=="Користувач")dbStatus="user";
            if(status=="Редактор")dbStatus="editor";
            if(status=="Адміністратор")dbStatus="admin";

            ui->errorPasswordPutLabel->setVisible(false);

            QString updateQuery="UPDATE `warehouse`.`users` SET";
            if(ui->nameLineEdit->text()!="") updateQuery+=" `name` = '"+ui->nameLineEdit->text()+"',";
            if(ui->surnameLineEdit->text()!="") updateQuery+=" `surname` = '"+ui->surnameLineEdit->text()+"',";
            if(ui->loginLineEdit->text()!="") updateQuery+=" `login` = '"+ui->loginLineEdit->text()+"',";
            if(ui->phoneNumberLineEdit->text()!="") updateQuery+=" `phone` = '"+ui->phoneNumberLineEdit->text()+"',";
            if(ui->emailLineEdit->text()!="") updateQuery+=" `email` = '"+ui->emailLineEdit->text()+"',";
            if(ui->passwordLineEdit->text()!="") updateQuery+=" `password` = '"+getHashPassword(ui->passwordLineEdit->text())+"',";

            updateQuery+=" `status` = '"+dbStatus+"'";

            updateQuery+=" WHERE (`id` = '"+QString::number(userId)+"')";

            QSqlQuery query(updateQuery);
            query.clear();
            QMessageBox::about(this,"Інформація про зміну інформації користувача","Інформацію про користувача успішно змінено.");

            emit updateUsersList();

            this->close();
        }
    }else{
        QMessageBox::warning(this,"Помилка при зміні інформації про користувача","Інформацію про користувача не змінено, перевірте правильність введених даних.");
    }
}
