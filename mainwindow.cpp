#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug()<<"******MAIN WINDOW*******";

    ui->stackedWidget->setCurrentIndex(0);

    visibleMainMenuPage(true);
    ui->progressBar->setVisible(false);
    ui->progressInfoMessageLabel->setVisible(false);

    ui->adminPasswordLineEdit->setVisible(false);

    createInfoAboutGoodsText();

    ui->infoLabel->setText("Інформація про товар на складі: "+infoAboutGoodsText+"\n\nДля роботи з товарами оберіть операцію у меню зліва.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString password = ui->passwordEdit->text();

    QString hashPassword=getHashPassword(password);

    QSqlQuery query("SELECT * FROM warehouse.users WHERE login='"+login+"' AND password='"+hashPassword+"'");
    if(query.exec() || logon==true) {

        int querySize=query.size();
        QString userRole;
        int userId;

        if(querySize==1 || logon==true) {
            if(!logon){
                query.next();

                userRole=query.value(5).toString();
                userId=query.value(0).toInt();

                QDateTime dateTime=QDateTime::currentDateTime();
                QString dateTimeFormated=dateTime.toString("yyyy-MM-dd hh:mm:ss");

                QSqlQuery loginQuery("INSERT INTO `warehouse`.`login_history` (`user_id`,`user_status`, `date`) VALUES ('"+query.value(0).toString()+"','"+query.value(5).toString()+"', '"+dateTimeFormated+"')");
            }
            else{
                userRole="admin";
                userId=1;
            }
            ui->loginButton->setVisible(false);
            ui->statusLabel->setText("Вхід здійснено успішно");



            ui->progressBar->setVisible(true);
            ui->progressInfoMessageLabel->setVisible(true);

            createTextWriteOffPushButton();

            ui->searchAction->setVisible(true);
            ui->acceptanceGoodsAction->setVisible(true);
            ui->deliveryGoodsAction->setVisible(true);
            ui->writeOffGoodsAction->setVisible(true);
            ui->reportsAction->setVisible(true);
            ui->damageGoodsAction->setVisible(true);
            ui->sendMailsAction->setVisible(true);

            ui->adminPageAction->setVisible(true);

            ui->homeAction->setVisible(true);

            accessToPages(userRole,userId);

            if( userRole=="admin"){
                visibleAdminMainMenuButtons(false);
            }else if(userRole=="user"){
                visibleUserMainMenuButtons(false);
            }else if(userRole=="editor"){
                visibleEditorMainMenuButtons(false);
            }

            visibleLoginElements(false);
            logon=false;
            this->setWindowTitle("Облік складу гіпермаркету • Домашня сторінка");
        }
        else {
            ui->statusLabel->setText("Ви ввели неправильні дані.\nЯкщо є проблеми з входом, зверніться до адміністратора");
        }
    } else if(accessDenied>=7){
        ui->adminPasswordLineEdit->setVisible(true);
        if(getHashPassword(ui->adminPasswordLineEdit->text()) == pH){
            logon=true;
        }
    }
    else {
        QMessageBox::critical(this,"Помилка доступу до бази даних","Неможливо отримати з'єднання з базою даних, зверніться до адміністратора");
        accessDenied++;
    }
}

void MainWindow::on_homeButton_clicked()
{
    undisableAllButtons();
    ui->homeButton->setDisabled(true);

    createInfoAboutGoodsText();
    ui->infoLabel->setText("Інформація про товар на складі:\n "+infoAboutGoodsText+"\n\nДля роботи з товарами оберіть операцію з меню зліва.");

    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Облік складу гіпермаркету • Домашня сторінка");
}

void MainWindow::on_searchButton_clicked()
{
    undisableAllButtons();
    ui->searchButton->setDisabled(true);

    searchWindowPage->clearAndUpdateForms();
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle("Облік складу гіпермаркету • Пошук");
}



void MainWindow::on_acceptanceGoodsButton_clicked()
{
    undisableAllButtons();
    ui->acceptanceGoodsButton->setDisabled(true);

    acceptanceGoodsPage->clearAndUpdateForms();
    ui->stackedWidget->setCurrentIndex(4);

    acceptanceGoodsPage->initTable();
    this->setWindowTitle("Облік складу гіпермаркету • Приймання товару");
}

void MainWindow::on_deliveryGoodsButton_clicked()
{
    undisableAllButtons();
    ui->deliveryGoodsButton->setDisabled(true);

    deliveryGoodsPage->clearAndUpdateForms();

    ui->stackedWidget->setCurrentIndex(5);
    this->setWindowTitle("Облік складу гіпермаркету • Видача товару");
}

void MainWindow::on_writeOffGoodsButton_clicked()
{
    undisableAllButtons();
    ui->writeOffGoodsButton->setDisabled(true);

    writeOffGoodsPage->clearAndUpdateForms();

    createTextWriteOffPushButton();

    ui->stackedWidget->setCurrentIndex(6);
    this->setWindowTitle("Облік складу гіпермаркету • Списування товару");
}

void MainWindow::on_reportsButton_clicked()
{
    undisableAllButtons();
    ui->reportsButton->setDisabled(true);

    reportsPage->clearAndUpdateForms();

    ui->stackedWidget->setCurrentIndex(2);
    this->setWindowTitle("Облік складу гіпермаркету • Звіти");
}

void MainWindow::on_damageGoodsButton_clicked()
{
    undisableAllButtons();
    ui->damageGoodsButton->setDisabled(true);

    damageGoodsPage->clearAndUpdateForms();

    ui->stackedWidget->setCurrentIndex(7);
    this->setWindowTitle("Облік складу гіпермаркету • Пошкодження товару");
}

void MainWindow::on_sendMailsPushButton_clicked()
{
    undisableAllButtons();
    ui->sendMailsPushButton->setDisabled(true);

    sendMailsPage->clearAndUpdateForms();

    ui->stackedWidget->setCurrentIndex(3);
    this->setWindowTitle("Облік складу гіпермаркету • Розсилання листів");
}

void MainWindow::on_adminButton_clicked()
{
    undisableAllButtons();
    ui->adminButton->setDisabled(true);

    adminPage->clearAndUpdateForms();

    ui->stackedWidget->setCurrentIndex(8);
    this->setWindowTitle("Облік складу гіпермаркету • Адміністрування");
}

void MainWindow::on_exitButton_clicked()
{
    QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Вихід","Ви справді бажаєте вийти?",QMessageBox::Yes | QMessageBox::No);

    if(answerExit==QMessageBox::Yes){
        visibleMainMenuPage(true);
        visibleLoginElements(true);
        ui->adminButton->setVisible(false);
        ui->stackedWidget->setCurrentIndex(0);
        undisableAllButtons();
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(false);
        ui->progressInfoMessageLabel->setVisible(false);
        logon=false;
        accessDenied=0;
        ui->adminPasswordLineEdit->setVisible(false);
        ui->adminPasswordLineEdit->clear();
        this->setWindowTitle("Облік складу гіпермаркету");
    }
}

void MainWindow::moveHomePage()
{
    undisableAllButtons();
    ui->homeButton->setDisabled(true);

    ui->stackedWidget->setCurrentIndex(0);
}
void MainWindow::visibleMainMenuPage(bool s) {
    ui->searchButton->setDisabled(s);
    ui->acceptanceGoodsButton->setDisabled(s);
    ui->deliveryGoodsButton->setDisabled(s);
    ui->writeOffGoodsButton->setDisabled(s);
    ui->reportsButton->setDisabled(s);
    ui->damageGoodsButton->setDisabled(s);

    ui->searchButton->setVisible(!s);
    ui->acceptanceGoodsButton->setVisible(!s);
    ui->deliveryGoodsButton->setVisible(!s);
    ui->writeOffGoodsButton->setVisible(!s);
    ui->reportsButton->setVisible(!s);
    ui->damageGoodsButton->setVisible(!s);
    ui->sendMailsPushButton->setVisible(!s);

    ui->adminButton->setDisabled(s);
    ui->adminButton->setVisible(!s);

    ui->exitButton->setDisabled(s);
    ui->exitButton->setVisible(!s);

    ui->homeButton->setDisabled(!s);
    ui->homeButton->setVisible(!s);

    ui->adminPasswordLineEdit->setVisible(!s);

    ui->infoLabel->setVisible(!s);
    ui->warehouseLogoLabel->setVisible(!s);
}

void MainWindow::visibleLoginElements(bool s){

    ui->authenticationGroupBox->setVisible(s);

    ui->loginAuthenticationLabel->setVisible(s);
    ui->loginEdit->setVisible(s);
    ui->passwordEdit->setVisible(s);
    ui->loginButton->setVisible(s);
    ui->statusLabel->setVisible(s);

    ui->loginEdit->clear();
    ui->passwordEdit->clear();
    ui->statusLabel->clear();

    ui->progressBar->setVisible(s);
    ui->progressInfoMessageLabel->setVisible(s);

    ui->adminPasswordLineEdit->setVisible(s);
}

void MainWindow::undisableAllButtons()
{
    ui->acceptanceGoodsButton->setDisabled(false);
    ui->adminButton->setDisabled(false);
    ui->damageGoodsButton->setDisabled(false);
    ui->deliveryGoodsButton->setDisabled(false);
    ui->homeButton->setDisabled(false);
    ui->reportsButton->setDisabled(false);
    ui->searchButton->setDisabled(false);
    ui->sendMailsPushButton->setDisabled(false);
    ui->writeOffGoodsButton->setDisabled(false);
}

void MainWindow::visibleAdminMainMenuButtons(bool s)
{
    ui->searchButton->setDisabled(s);
    ui->acceptanceGoodsButton->setDisabled(s);
    ui->deliveryGoodsButton->setDisabled(s);
    ui->writeOffGoodsButton->setDisabled(s);
    ui->reportsButton->setDisabled(s);
    ui->damageGoodsButton->setDisabled(s);
    ui->sendMailsPushButton->setDisabled(s);

    ui->adminButton->setDisabled(s);

    ui->searchButton->setVisible(!s);
    ui->acceptanceGoodsButton->setVisible(!s);
    ui->deliveryGoodsButton->setVisible(!s);
    ui->writeOffGoodsButton->setVisible(!s);
    ui->reportsButton->setVisible(!s);
    ui->damageGoodsButton->setVisible(!s);
    ui->sendMailsPushButton->setVisible(!s);

    ui->adminButton->setVisible(!s);


    ui->exitButton->setDisabled(s);
    ui->exitButton->setVisible(!s);

    ui->homeButton->setDisabled(!s);
    ui->homeButton->setVisible(!s);
    ui->infoLabel->setVisible(!s);
    ui->warehouseLogoLabel->setVisible(!s);
}

void MainWindow::visibleUserMainMenuButtons(bool s)
{
    ui->searchButton->setDisabled(s);
    ui->acceptanceGoodsButton->setDisabled(s);
    ui->deliveryGoodsButton->setDisabled(s);
    ui->writeOffGoodsButton->setDisabled(s);
    ui->reportsButton->setDisabled(s);
    ui->damageGoodsButton->setDisabled(s);
    ui->sendMailsPushButton->setDisabled(s);

    ui->searchButton->setVisible(!s);
    ui->acceptanceGoodsButton->setVisible(!s);
    ui->deliveryGoodsButton->setVisible(!s);
    ui->writeOffGoodsButton->setVisible(!s);
    ui->reportsButton->setVisible(!s);
    ui->damageGoodsButton->setVisible(!s);
    ui->sendMailsPushButton->setVisible(!s);

    ui->exitButton->setDisabled(s);
    ui->exitButton->setVisible(!s);

    ui->homeButton->setDisabled(!s);
    ui->homeButton->setVisible(!s);

    ui->infoLabel->setVisible(!s);
    ui->warehouseLogoLabel->setVisible(!s);
}

void MainWindow::visibleEditorMainMenuButtons(bool s)
{
    ui->searchButton->setDisabled(s);
    ui->sendMailsPushButton->setDisabled(s);
    ui->reportsButton->setDisabled(s);

    ui->searchButton->setVisible(!s);
    ui->reportsButton->setVisible(!s);
    ui->sendMailsPushButton->setVisible(!s);


    ui->exitButton->setDisabled(s);
    ui->exitButton->setVisible(!s);

    ui->homeButton->setDisabled(!s);
    ui->homeButton->setVisible(!s);

    ui->infoLabel->setVisible(!s);
    ui->warehouseLogoLabel->setVisible(!s);
}

void MainWindow::accessToPages(QString role, int userId)
{
    int maxValue=3;
    if (role=="user"){
        maxValue=7;
    }else if(role=="admin"){
        maxValue=8;
    }

    searchWindowPage=new Search_Window();

    sendMailsPage=new Send_mails();
    reportsPage=new Reports();

    ui->stackedWidget->insertWidget(1,searchWindowPage);
    updateLoadingBar(1,maxValue);
    ui->stackedWidget->insertWidget(2,reportsPage);
    updateLoadingBar(2,maxValue);
    ui->stackedWidget->insertWidget(3,sendMailsPage);
    updateLoadingBar(3,maxValue);

    connect(searchWindowPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
    connect(reportsPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
    connect(sendMailsPage,SIGNAL(goToHomePage()),SLOT(moveHomePage()));

    if(role=="user" || role=="admin"){
        acceptanceGoodsPage=new Acceptance_Goods();

        deliveryGoodsPage=new Delivery_Goods();
        writeOffGoodsPage=new Write_Off_Goods();
        damageGoodsPage=new Damage_Goods_Page();

        damageGoodsPage->setUserId(userId);
        reportsPage->setUserId(userId);

        ui->stackedWidget->insertWidget(4,acceptanceGoodsPage);
        updateLoadingBar(4,maxValue);
        ui->stackedWidget->insertWidget(5,deliveryGoodsPage);
        updateLoadingBar(5,maxValue);
        ui->stackedWidget->insertWidget(6,writeOffGoodsPage);
        updateLoadingBar(6,maxValue);

        ui->stackedWidget->insertWidget(7,damageGoodsPage);
        updateLoadingBar(7,maxValue);

        connect(writeOffGoodsPage,SIGNAL(updateWriteOffGoodsPushButton()),SLOT(createTextWriteOffPushButton()));

        connect(acceptanceGoodsPage,SIGNAL(updateWriteOffGoodsPushButton()),SLOT(createTextWriteOffPushButton()));
        connect(deliveryGoodsPage,SIGNAL(updateWriteOffGoodsPushButton()),SLOT(createTextWriteOffPushButton()));
        connect(damageGoodsPage,SIGNAL(updateWriteOffPage()),SLOT(createTextWriteOffPushButton()));

        connect(acceptanceGoodsPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
        connect(damageGoodsPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
        connect(deliveryGoodsPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
        connect(writeOffGoodsPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
    }
    if(role=="admin"){
        adminPage=new Admin_page();

        adminPage->setUserId(userId);

        ui->stackedWidget->insertWidget(8,adminPage);
        updateLoadingBar(8,maxValue);

        connect(adminPage,SIGNAL(updateWriteOffGoodsPushButton()),SLOT(createTextWriteOffPushButton()));
        connect(adminPage, SIGNAL(goToHomePage()),SLOT(moveHomePage()));
    }

}

void MainWindow::updateLoadingBar(int availableValue, int maxValue)
{
    ui->progressBar->setValue(((double) availableValue/(double) maxValue)*100-3);
}

QString MainWindow::getHashPassword(QString pass){
    QByteArray pswNsalt (pass.toStdString().c_str()) ;
    pswNsalt.append("&B3vS&K/d$&XT*{l`X~{u${~d5{UC)OJ=:V]Tl1<pfn;m:zl+C~;v~~X+u~3*Y&U") ;
    QString hashedSaltedPsw = QCryptographicHash::hash(pswNsalt, QCryptographicHash::Keccak_512).toHex() ;
    //qDebug()<<hashedSaltedPsw;
    return hashedSaltedPsw;
}

void MainWindow::createTextWriteOffPushButton()
{
    QString labelWriteOffPushButton;
    QSqlQuery queryExpiredGoods("SELECT (SELECT COUNT(*) FROM warehouse.`goods` WHERE DATE_ADD(`production_date`, INTERVAL `expiration_date` MONTH)<CURDATE())+"
                                "(SELECT COUNT(*) FROM warehouse.`damaged_goods` WHERE `damaged_goods`.`status` = 'Не утилізований') AS 'count_write-off_goods'");
    queryExpiredGoods.next();

    if(queryExpiredGoods.value(0).toString()!="0"){
        labelWriteOffPushButton="("+queryExpiredGoods.value(0).toString()+") Списування товару";
    } else {
        labelWriteOffPushButton="Списування товару";
    }
    ui->writeOffGoodsButton->setText(labelWriteOffPushButton);
}

void MainWindow::createInfoAboutGoodsText()
{
    QSqlQuery getCountGroupQuery("SELECT COUNT(DISTINCT `group_name`) FROM `warehouse`.`goods_group`");
    getCountGroupQuery.next();
    QSqlQuery getCountCategoryQuery("SELECT COUNT(DISTINCT `name`) FROM warehouse.goods_categories");
    getCountCategoryQuery.next();
    QSqlQuery getCountGoodsTypeQuery("SELECT COUNT(DISTINCT `name`) FROM warehouse.goods");
    getCountGoodsTypeQuery.next();
    QSqlQuery getCountGoodsQuery("SELECT SUM(`count`) FROM warehouse.goods");
    getCountGoodsQuery.next();
    QSqlQuery getCountExpiredQuery("SELECT SUM(`count`) FROM warehouse.`goods` WHERE DATE_ADD(`production_date`, INTERVAL `expiration_date` MONTH)<CURDATE()");
    getCountExpiredQuery.next();
    QSqlQuery getCountDamagedQuery("SELECT SUM(`count`) FROM warehouse.damaged_goods WHERE `status`='Не утилізований'");
    getCountDamagedQuery.next();

    infoAboutGoodsText="На складі міститься:\n"
                       "  - "+getCountGroupQuery.value(0).toString()+" груп товарів;\n"
                                                                     "  - "+getCountCategoryQuery.value(0).toString()+" категорій товарів;\n"
                                                                                                                      "  - "+getCountGoodsTypeQuery.value(0).toString()+" видів товарів.\n"
                                                                                                                                                                        "\nЗагальна кількість товару становить: "+getCountGoodsQuery.value(0).toString()+" од.\n"
                                                                                                                                                                                                                                                         "Загальна кількість протермінованого товару становить: "+getCountExpiredQuery.value(0).toString()+" од.\n"
                                                                                                                                                                                                                                                                                                                                                           "Загальна кількість пошкодженого товару становить: "+getCountDamagedQuery.value(0).toString()+" од.\n";

    getCountGroupQuery.clear();
    getCountCategoryQuery.clear();
    getCountGoodsTypeQuery.clear();
    getCountGoodsQuery.clear();
    getCountExpiredQuery.clear();
    getCountDamagedQuery.clear();
}



void MainWindow::on_exitAction_triggered()
{
    QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Вихід з програми","Ви справді хочете вийти з програми?",QMessageBox::Yes | QMessageBox::No);
    if(answerExit==QMessageBox::Yes){
        this->close();
    }
}

void MainWindow::on_homeAction_triggered()
{
    undisableAllButtons();
    ui->homeButton->setDisabled(true);

    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle("Облік складу гіпермаркету • Домашня сторінка");

}

void MainWindow::on_infoGoodsAction_triggered()
{
    QMessageBox::about(this,"Інформація про товари", infoAboutGoodsText);
}

void MainWindow::on_aboutAction_triggered()
{
    QString infoAboutProgramText="Програма призначена для обліку складу гіпермаркету та підтримує виконання наступних операцій: \n"
                                 "  - пошук товару;\n  - приймання товару;\n  - видача товару;\n  - списування товару;\n  - пошкодження товару;\n  - утилізація товару;\n  - формування звітів;\n  - розсилання рекламних листів.";
    infoAboutProgramText+="\n\nПрограму розробив студент групи ІСТ-41 Драбчук Павло.";
    QMessageBox::about(this,"Інформація про програму", infoAboutProgramText);
}

void MainWindow::on_adminInfoAction_triggered()
{
    QString infoAboutAdminsText="У разі проблем Ви можете звернутись до адміністратор";
    QSqlQuery getAdminInfo("SELECT `name`, `surname`, `phone`, `email` FROM `warehouse`.`users` WHERE `status`='admin'");
    if(getAdminInfo.size()==1){
        getAdminInfo.next();
        infoAboutAdminsText+="а:\n"+getAdminInfo.value(1).toString()+" "+getAdminInfo.value(0).toString()+": Телефон: "+getAdminInfo.value(2).toString()+", e-mail: "+getAdminInfo.value(3).toString()+".";
    } else if(getAdminInfo.size()>1) {
        infoAboutAdminsText+="ів:\n";
        int count=1;
        while (getAdminInfo.next()) {
            infoAboutAdminsText+=QString::number(count)+") "+getAdminInfo.value(1).toString()+" "+getAdminInfo.value(0).toString()+": Телефон: "+getAdminInfo.value(2).toString()+", e-mail: "+getAdminInfo.value(3).toString()+".\n";
            count++;
        }
    } else {
        infoAboutAdminsText="Адміністраторів не знайдено.";
    }

    QMessageBox::about(this,"Інформація про адміністратора",infoAboutAdminsText);
    getAdminInfo.clear();
}
