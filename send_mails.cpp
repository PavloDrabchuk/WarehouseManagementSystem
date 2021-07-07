#include "send_mails.h"
#include "ui_send_mails.h"

Send_mails::Send_mails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Send_mails)
{
    ui->setupUi(this);

    qDebug()<<"******SEND MAILS*******";

    addItemToListWidget();

    addCustomersWindow=new Add_Customers();

    connect(addCustomersWindow,SIGNAL(updateListWidget()),SLOT(updateListWidget()));

}

Send_mails::~Send_mails()
{
    delete ui;
}

void Send_mails::clearAndUpdateForms()
{
    ui->subjectLineEdit->clear();
    ui->mailTextTextEdit->clear();

    for(int i=0;i<ui->emailsListWidget->count();i++){
        ui->emailsListWidget->item(i)->setCheckState(Qt::Unchecked);
    }
    updateListWidget();
}

void Send_mails::on_sendMailPushButton_clicked()
{
    if(checkInputForms()){
        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Розсилання повідомлень","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){

            QUrl emailUrl;
            QString url="mailto:", mailText;
            int countCheched=0;
            for(int i=0;i<ui->emailsListWidget->count();i++){
                if(ui->emailsListWidget->item(i)->checkState()==Qt::Checked){
                    if(countCheched==0) url+=checkedEmails.at(i);
                    else url+=","+checkedEmails.at(i);
                    countCheched++;
                }
            }
            url+="?";
            if(ui->subjectLineEdit->text()!=""){
                url+="subject="+ui->subjectLineEdit->text();
                url+="&";
            }

            mailText=ui->mailTextTextEdit->toPlainText();
            mailText.replace("\n","%0D%0A");

            url+="body="+mailText;

            emailUrl.setUrl(url);
            QMessageBox::about(this,"Інформація про розсилання повідомлень","Все добре, натисніть \"Ок\", щоб продовжити.");
            QDesktopServices::openUrl(QUrl(url,QUrl::TolerantMode));

        }
    }else{
        QMessageBox::warning(this,"Інформація про розсилання повідомлень","Повідомлення не розіслано.");
    }
}

void Send_mails::on_addCustomersPushButton_clicked()
{
    addCustomersWindow->clearAndUpdateForms();

    addCustomersWindow->setModal(true);
    addCustomersWindow->setWindowTitle("Додавання нового клієнта");
    addCustomersWindow->exec();
}

void Send_mails::updateListWidget()
{
    addItemToListWidget();
}

void Send_mails::addItemToListWidget()
{
    ui->emailsListWidget->clear();
    checkedEmails.clear();
    QListWidgetItem *item;

    QSqlQuery query("SELECT `name`,`surname`,`email` FROM `warehouse`.`customers_info`");

    while(query.next()){
        checkedEmails.push_back(query.value(2).toString());

        item=new QListWidgetItem();
        item->setText(query.value(2).toString() +" - "+query.value(1).toString()+" "+query.value(0).toString());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        ui->emailsListWidget->addItem(item);
    }
}

bool Send_mails::checkInputForms()
{
    bool result=true;
    int countCheched=0;

    for(int i=0;i<ui->emailsListWidget->count();i++){
        if(ui->emailsListWidget->item(i)->checkState()==Qt::Checked){
            countCheched++;
        }
    }

    if(countCheched==0){
        QMessageBox::warning(this,"Помилка при надсиланні повідомлення","Ви не обрали жодної електронної адреси з списку.");
        result=false;
    }
    else if(ui->mailTextTextEdit->toPlainText()==""){
        result=false;
        QMessageBox::warning(this,"Помилка при надсиланні повідомлення","Повідомлення не надіслано, необхідно ввести текст.");
    }else if(ui->subjectLineEdit->text()==""){
        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Розсилання повідомлень","Ви хочете надіслати повідомлення без теми?",QMessageBox::Yes | QMessageBox::No);
        if(answerExit==QMessageBox::No){
            result=false;
        }
    }
    return result;
}

void Send_mails::on_backToMainMenuWindow_clicked()
{
    emit goToHomePage();
}
