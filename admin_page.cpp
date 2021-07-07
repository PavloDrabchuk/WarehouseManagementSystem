#include "admin_page.h"
#include "ui_admin_page.h"

Admin_page::Admin_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_page)
{
    ui->setupUi(this);

    qDebug()<<"******ADMIN PAGE*******";

    srand(time(0));

    addNewUsersWindow=new Add_new_users();
    updateUsersWindow=new Update_Users();

    addUsersToComboBox();

    addItemToListWidget();
    addItemToUsersListWidget();

    connect(addNewUsersWindow, SIGNAL(updateUsersList()),SLOT(updateUsersListWidget()));
    connect(updateUsersWindow, SIGNAL(updateUsersList()),SLOT(updateUsersListWidget()));
}

Admin_page::~Admin_page()
{
    delete ui;
}

void Admin_page::clearAndUpdateForms()
{
    ui->generateTableProgressBar->setValue(0);
    for(int i=0;i<ui->tablesListWidget->count();i++){
        ui->tablesListWidget->item(i)->setCheckState(Qt::Unchecked);
    }

    ui->checkAllCheckBox->setCheckState(Qt::Unchecked);
    ui->resultCreateStructureTableLabel->setText("");
    ui->usersComboBox->setCurrentIndex(0);

    addItemToUsersListWidget();
}

void Admin_page::setUserId(int id)
{
    userId=id;
}

int Admin_page::randomBetween(int low, int high)
{
    return   low + rand() % high;
}

void Admin_page::addUsersToComboBox()
{
    ui->usersComboBox->clear();
    ui->usersComboBox->addItem("Оберіть користувача");

    QSqlQuery getAllUserNamesQuery("SELECT `name`,`surname` FROM `warehouse`.`users`");

    while(getAllUserNamesQuery.next()){
        ui->usersComboBox->addItem(getAllUserNamesQuery.value(1).toString()+" "+getAllUserNamesQuery.value(0).toString());
    }
    getAllUserNamesQuery.clear();
}

void Admin_page::addItemToListWidget()
{
    ui->tablesListWidget->clear();
    QListWidgetItem *item;

    QSqlQuery query("SHOW TABLES FROM `warehouse`");

    while(query.next()){
        checkedTables.push_back(query.value(0).toString());
        item=new QListWidgetItem();
        item->setText(query.value(0).toString());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        ui->tablesListWidget->addItem(item);
    }
}

void Admin_page::addItemToUsersListWidget()
{
    ui->usersListWidget->clear();
    checkedUsers.clear();
    QListWidgetItem *item;

    QSqlQuery query("SELECT `name`, `surname`, `login` FROM warehouse.users WHERE `login` != 'admin'");

    while(query.next()){
        checkedUsers.push_back(query.value(2).toString());
        item=new QListWidgetItem();
        item->setText(query.value(1).toString()+" "+query.value(0).toString()+", логін: "+query.value(2).toString());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

        ui->usersListWidget->addItem(item);
    }
}

int Admin_page::createExpirationDate()
{
    int scale=randomBetween(0,100), result=-1;
    if(scale<70) result=88888;
    else result=randomBetween(12,48);

    return result;
}

void Admin_page::on_addNewUserPushButton_clicked()
{
    addNewUsersWindow->clearAndUpdateForms();

    addNewUsersWindow->setModal(true);
    addNewUsersWindow->setWindowTitle("Додавання нового користувача");
    addNewUsersWindow->exec();
}

void Admin_page::on_generateTablePushButton_clicked()
{
    QString groupCategories[3]={"Покриття для підлоги","Будматеріали","Клей"};
    int groupId[5]={1,1,2,2,3}; //{3,3,1,1,2}
    QString nameCategories[5]={"Ламінат", "Плитка", "Гіпсокартон", "Цегла","Клей для плитки"};

    int categoriesId[25]={1,3,3,5,4,4,4,2,2,1,1,1,3,3,5,5,5,5,  4,  2,  2,   1,1,5,3};
    QString nameGoods[25]={"Kronospan","Plato","Belgips","BauGut MKL","Карамейя","Літос","PROKERAM","Sabbia","Суаро","EH","Expert","Artens",
                           "Siniat","УКРМАГНЕЗИТ","Ceresit CM 117","BauGut KLF","Alpol AK 509","Ферозіт 100","Рустик","Golden Tile","Cersanit",
                           "Egger","Krono","Alpol AK 511 PLUS","Knauf"};
    QString descriptions[5]={"Опис 1","","Опис 2", "Товар з нової колекції","Новинка на ринку даний товарів"};
    QString producers[5]={"Виробник 1","Виробник 2","Виробник 3","Виробник 4","Виробник 5"};
    QString producerCountries[5]={"Україна","Білорусь","Індонезія","Франція","Польща"};
    QString colors[7]={"Дуб","Сосна","Червоний","Коричневий","Сірий","Пурпуровий","Вільха"};

    QString userNames[3]={"admin","Андрій","Настя"};
    QString userSurnames[3]={"admin","Васильченко","Роженко"};
    QString userLogins[3]={"admin","user","editor"};
    QString userPasswords[3]={"287aef4ac8b10fe8249d8fc48327b2790cc064e5319e1d9ca87b42b6d128d28fbe44b9f9223092c3898c2310a7feb13a44e7ca02d64e424434149ca4d1bdd284",
                              "819956537a033b3cd735f20b9890f3db64ee391bd6156d435ed36405652cea27683f7b93da38a9aec164ba91f8c0efc03da1cb8d2b57d67590bfa5c8dafed6e6",
                              "c41ad0ebbdf0d4dc08c0ca28ad20b5bee56d27d16b353251c0a2d43c21761050874c185f9ec1c32d42ed38b0c9775b6ab49d8556b1bca4c0b90329d63ac387e6"};
    QString userStatuses[3]={"admin","user","editor"};
    QString userPhones[3]={"+380888888888","+380888888888","+380888888888"};
    QString userEmails[3]={"admin@gmail.com","user@gmail.com","editor@gmail.com"};

    QString customerNames[5]={"Павло","Марта","Іван","Юлія","Оля"};
    QString customerSurnames[5]={"Драбчук","Рожчук","Дерен","Ясенко","Петренко"};
    QString customerEmails[5]={"ravluk2000@gmail.com","marta.r@ukr.net","ivan.deren@gmail.com","yasenko@gmail.com","petrenko.olia@ukr.net"};
    QString customerPhones[5]={"+380668743947","+380675842578","+380508423751","+380504862471","+380960224536"};

    QString insertQuery;

    for(int i=0;i<5;i++){
        insertQuery="INSERT INTO `warehouse`.`goods_categories` (`group_id`, `name`) VALUES ('"+QString::number(groupId[i])+"', '"+nameCategories[i]+"')";
        QSqlQuery query(insertQuery);
        query.clear();
    }

    ui->generateTableProgressBar->setValue(15);

    for(int i=0;i<3;i++){
        insertQuery="INSERT INTO `warehouse`.`goods_group` (`group_name`) VALUES ('"+groupCategories[i]+"')";
        QSqlQuery query(insertQuery);
        query.clear();
    }
    ui->generateTableProgressBar->setValue(32);

    QString dateTimeFormated=QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");

    QString providerId;
    for(int i=0;i<25;i++){
        providerId=QString::number(randomBetween(1,2));
        QSqlQuery idLastGoodQuery("SELECT `id` FROM warehouse.`goods` ORDER BY `id` DESC LIMIT 1");
        idLastGoodQuery.next();
        int idLastGood=idLastGoodQuery.value(0).toInt()+1;
        QString batchCode=QString::number(idLastGood)+"_"+providerId+"_"+QString::number(categoriesId[i])+"_"+dateTimeFormated;

        insertQuery="INSERT INTO `warehouse`.`goods` (`batch_code`,`category_id`, `name`, `count`, `price`, `description`, `producer`, `producing_country`, `guarantee`, `weight`, `width`, `height`, `length`, `color`, `expiration_date`, `production_date`, `provider_id`, `date_arrival`) VALUES "
                    " ('"+batchCode+"','"+QString::number(categoriesId[i])+"', '"+nameGoods[i]+"', '"+QString::number(randomBetween(40,150))+"', '"+QString::number(randomBetween(7000,45000))+"', '"+descriptions[randomBetween(0,4)]+"', '"+producers[randomBetween(0,4)]+"', '"+producerCountries[randomBetween(0,4)]+"', "
                                                                                                                                                                                                                                                                                                                     " '"+QString::number(randomBetween(1,14))+"', '"+QString::number(randomBetween(100,500)*0.1)+"', '"+QString::number(randomBetween(10000,50000)*0.1)+"', '"+QString::number(randomBetween(10000,50000)*0.1)+"', '"+QString::number(randomBetween(10000,50000)*0.1)+"', '"+colors[randomBetween(0,6)]+"', '"+QString::number(createExpirationDate())+"', '"+QString::number(randomBetween(2018,3))+"-"+QString::number(randomBetween(1,12))+"-"+QString::number(randomBetween(1,28))+"', '"+providerId+"', '"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"')";
        QSqlQuery query(insertQuery);
        query.clear();
    }
    ui->generateTableProgressBar->setValue(68);

    for(int i=0;i<4;i++){
        insertQuery="INSERT INTO `warehouse`.`provider` (`name`, `email`, `phones`, `adress`) VALUES ('Постачальник "+QString::number(i+1)+"', 'provider"+QString::number(i+1)+"@gmail.com', '+"+QString::number(randomBetween(1000,9000))+QString::number(randomBetween(1000,9000))+QString::number(randomBetween(10,90))+"', 'Адреса №"+QString::number(i+1)+"')";
        QSqlQuery query(insertQuery);
        query.clear();
    }
    ui->generateTableProgressBar->setValue(76);

    for(int i=0;i<3;i++){
        insertQuery="INSERT INTO `warehouse`.`users` (`name`, `surname`, `login`, `password`, `status`, `phone`, `email`) VALUES ('"+userNames[i]+"', '"+userSurnames[i]+"', '"+userLogins[i]+"', '"+userPasswords[i]+"', '"+userStatuses[i]+"', '"+userPhones[i]+"', '"+userEmails[i]+"')";
        QSqlQuery query(insertQuery);
        query.clear();
    }
    ui->generateTableProgressBar->setValue(87);

    for(int i=0;i<5;i++){
        insertQuery="INSERT INTO `warehouse`.`customers_info` (`name`, `surname`, `email`, `phone`) VALUES ('"+customerNames[i]+"', '"+customerSurnames[i]+"', '"+customerEmails[i]+"', '"+customerPhones[i]+"');";
        QSqlQuery query(insertQuery);
        query.clear();
    }
    ui->generateTableProgressBar->setValue(100);

    addItemToUsersListWidget();
    emit updateWriteOffGoodsPushButton();

    QMessageBox::about(this,"Генерування тестової бази даних","Тестову базу даних успішно згенеровано.");

}

void Admin_page::on_pushButton_clicked()
{
    emit goToHomePage();
}

void Admin_page::on_updateUserInfoPushButton_clicked()
{
    if(ui->usersComboBox->currentIndex()!=0){
        updateUsersWindow->clearAndUpdateForms();
        QString getUserIdBySurnameAndNameQuery="SELECT `id` FROM warehouse.users WHERE CONCAT(`users`.`surname`,' ',`users`.`name`)='"+ui->usersComboBox->currentText()+"'";
        QSqlQuery getUserIdBySurnameAndName(getUserIdBySurnameAndNameQuery);
        getUserIdBySurnameAndName.next();

        updateUsersWindow->setUserId(getUserIdBySurnameAndName.value(0).toInt());
        updateUsersWindow->insertUserInfo(getUserIdBySurnameAndName.value(0).toInt());

        updateUsersWindow->setModal(true);
        updateUsersWindow->setWindowTitle("Зміна інформації про користувача");
        updateUsersWindow->exec();
    } else {
        QMessageBox::warning(this,"Зміна інформації про користувача","Оберіть користувача.");
    }
}

void Admin_page::on_pushButton_2_clicked()
{
    QStringList tables={};
    QSqlQuery getTablesQuery("SHOW TABLES FROM `warehouse`");
    while (getTablesQuery.next()) {
        tables.push_back(getTablesQuery.value(0).toString());
    }
    getTablesQuery.clear();

    for(QString t:tables){
        QSqlQuery getCreateTableScriptQuery("SHOW CREATE TABLE  `warehouse`.`"+t+"`");
        getCreateTableScriptQuery.next();
        getCreateTableScriptQuery.clear();
    }

    QSqlDatabase DB = QSqlDatabase::addDatabase("QMYSQL");
    DB.setPort(3306);
    DB.setHostName("127.0.0.1");
    DB.setUserName("WarehouseAdmin");
    DB.setPassword("pX?t892s3p#*3PR@4S&#");
    if(DB.open()) {
        QSqlQuery Query(DB);
        if (Query.exec("CREATE SCHEMA `warehouse`")) {
            QMessageBox::about(this,"Створення бази даних","Базу даних успішно створено.");
        }
        else {
            QMessageBox::warning(this,"Створення бази даних","Помилка запиту, можливо база даних вже існує.");
        }
    } else {
        QMessageBox::critical(this,"Створення бази даних","Помилка доступу.");
    }


}

void Admin_page::on_trunkateTablesPushButton_clicked()
{

    QMessageBox::StandardButton answerExit = QMessageBox::question(this,
                                                                   "Очищення таблиці","Ви справді хочете очистити вибрані таблиці?",
                                                                   QMessageBox::Yes | QMessageBox::No);


    if(answerExit==QMessageBox::Yes){

        QString name, surname, login, password, status, phone, email;

        int countTables=0;
        for(int i=0;i<ui->tablesListWidget->count();i++) {
            if(ui->tablesListWidget->item(i)->checkState()==Qt::Checked){

                if(ui->tablesListWidget->item(i)->text()=="users"){
                    QSqlQuery getAdminInfoById("SELECT * FROM warehouse.users WHERE `id`='"+QString::number(userId)+"'");
                    getAdminInfoById.next();

                    name= getAdminInfoById.value(1).toString();
                    surname= getAdminInfoById.value(2).toString();
                    login= getAdminInfoById.value(3).toString();
                    password= getAdminInfoById.value(4).toString();
                    status= getAdminInfoById.value(5).toString();
                    phone= getAdminInfoById.value(6).toString();
                    email= getAdminInfoById.value(7).toString();

                    getAdminInfoById.clear();
                }

                QSqlQuery truncateTableQuery("TRUNCATE TABLE warehouse.`"+ui->tablesListWidget->item(i)->text()+"`");
                truncateTableQuery.clear();
                countTables++;

                if(ui->tablesListWidget->item(i)->text()=="users"){
                    QString insertQuery="INSERT INTO `warehouse`.`users` (`name`, `surname`, `login`, `password`, `status`, `phone`, `email`) VALUES ('"+name+"', '"+surname+"', '"+login+"', '"+password+"', '"+status+"', '+"+phone+"', '"+email+"')";
                    QSqlQuery query(insertQuery);
                    query.clear();
                }
            }
        }

        if (countTables==0) {
            QMessageBox::about(this,"Очищення таблиць","Жодної таблиці не вибрано.");
        } else {
            emit updateWriteOffGoodsPushButton();
            addItemToUsersListWidget();
            QMessageBox::about(this,"Очищення таблиць","Вибрані таблиці успішно очищено.");
        }
    }


}

void Admin_page::on_checkAllCheckBox_stateChanged(int arg1)
{
    switch (arg1) {
    case 0:{
        for(int i=0;i<ui->tablesListWidget->count();i++){
            ui->tablesListWidget->item(i)->setCheckState(Qt::Unchecked);
        }
        break;
    }
    case 2:{
        for(int i=0;i<ui->tablesListWidget->count();i++){
            ui->tablesListWidget->item(i)->setCheckState(Qt::Checked);
        }
        break;
    }
    default:{
        break;
    }
    }
}

void Admin_page::on_createDatabaseStructurePushButton_clicked()
{
    QString resultCreating="";
    ui->resultCreateStructureTableLabel->setText(resultCreating);

    QString tablesList[13] ={"acceptance_goods_history","customers_info","damaged_goods","delivery_goods_history","expired_goods","goods",
                             "goods_categories","goods_group","login_history","provider","recycled goods","reports","users"};
    QString createStructureDataBaseQueryList[13] = {"  CREATE TABLE `warehouse`.`acceptance_goods_history` (  `id` int NOT NULL AUTO_INCREMENT,  `name` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `date` datetime NOT NULL,  `number` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `provider_id` int DEFAULT NULL,  `count` int NOT NULL,  `cost` double NOT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`customers_info` (  `id` int NOT NULL AUTO_INCREMENT,  `name` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `surname` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `email` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `phone` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`damaged_goods` (  `id` int NOT NULL AUTO_INCREMENT,  `batch_code` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `id_goods` int DEFAULT NULL,  `name_goods` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `user_id` int DEFAULT NULL,  `count` int DEFAULT NULL,  `cause_damage` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,  `pest` set('Покупець','Працівник') COLLATE utf8_unicode_ci DEFAULT NULL,  `date_of_damage` date DEFAULT NULL,  `registration_date` datetime DEFAULT NULL,  `status` set('Утилізований','Не утилізований') COLLATE utf8_unicode_ci DEFAULT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`delivery_goods_history` (  `id` int NOT NULL AUTO_INCREMENT,  `batch_code` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `category_id` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `name` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `date` datetime DEFAULT NULL,  `count` int DEFAULT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`expired_goods` (  `id` int NOT NULL AUTO_INCREMENT,  `batch_code` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `goods_id` int NOT NULL,  `category_id` int NOT NULL,  `goods_name` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `count` int NOT NULL,  `producer` varchar(45) COLLATE utf8_unicode_ci NOT NULL,  `production_date` date NOT NULL,  `expiration_date` int NOT NULL,  `write_off_date` datetime NOT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`goods` (  `id` int NOT NULL AUTO_INCREMENT,  `batch_code` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `category_id` int DEFAULT NULL,  `name` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `count` int DEFAULT NULL,  `price` double DEFAULT NULL,  `description` varchar(400) COLLATE utf8_unicode_ci DEFAULT NULL,  `producer` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `producing_country` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `guarantee` int DEFAULT NULL,  `weight` int DEFAULT NULL,  `width` int DEFAULT NULL,  `height` int DEFAULT NULL,  `length` int DEFAULT NULL,  `color` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `expiration_date` int DEFAULT NULL,  `production_date` date DEFAULT NULL,  `provider_id` int DEFAULT NULL,  `date_arrival` datetime DEFAULT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`goods_categories` (`id` int NOT NULL AUTO_INCREMENT, `group_id` int DEFAULT NULL, `name` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL, PRIMARY KEY (`id`), UNIQUE KEY `name_UNIQUE` (`name`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`goods_group` (  `id` int NOT NULL AUTO_INCREMENT,  `group_name` varchar(45) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT NULL,  `description` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,  PRIMARY KEY (`id`),  UNIQUE KEY `group_name_UNIQUE` (`group_name`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`login_history` (  `id` int NOT NULL AUTO_INCREMENT,  `user_id` int DEFAULT NULL,  `user_status` varchar(10) COLLATE utf8_unicode_ci DEFAULT NULL,  `date` datetime DEFAULT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`provider` (  `id` int NOT NULL AUTO_INCREMENT,  `name` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `email` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `phones` varchar(60) COLLATE utf8_unicode_ci DEFAULT NULL,  `adress` varchar(100) COLLATE utf8_unicode_ci DEFAULT NULL,  PRIMARY KEY (`id`),  UNIQUE KEY `name_UNIQUE` (`name`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`recycled goods` (  `id` int NOT NULL AUTO_INCREMENT,  `batch_code` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `name_goods` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `count` int DEFAULT NULL,  `damage_date` date DEFAULT NULL,  `date` datetime DEFAULT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`reports` (  `id` int NOT NULL AUTO_INCREMENT,  `number` int DEFAULT NULL,  `operation` set('1','2','3','4','5') COLLATE utf8_unicode_ci DEFAULT NULL,  `filename` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  `date` datetime DEFAULT NULL,  PRIMARY KEY (`id`)) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci; ",
                                                    "  CREATE TABLE `warehouse`.`users` ( `id` int NOT NULL AUTO_INCREMENT,  `name` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,  `surname` varchar(30) COLLATE utf8_unicode_ci DEFAULT NULL,  `login` varchar(20) COLLATE utf8_unicode_ci DEFAULT NULL,  `password` varchar(128) COLLATE utf8_unicode_ci DEFAULT NULL,  `status` set('admin','user','editor') COLLATE utf8_unicode_ci DEFAULT NULL,  `phone` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,  `email` varchar(45) COLLATE utf8_unicode_ci DEFAULT NULL,  PRIMARY KEY (`id`),  UNIQUE KEY `login_UNIQUE` (`login`)) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;"};
    int position=0;
    for(int i=0;i<13;i++){
        QSqlQuery query;
        query.prepare(createStructureDataBaseQueryList[i]);

        if(query.exec()){
            resultCreating+=" • "+tablesList[i]+" -  ✔️\n";
        } else {
            resultCreating+=" • "+tablesList[i]+" - ❌\n";
        }
        ui->resultCreateStructureTableLabel->setText(resultCreating);
        query.clear();
        qDebug()<<"pos: "<<i<<" -- q: "<<createStructureDataBaseQueryList[i];
        position++;
    }
    QMessageBox::about(this,"Створення структури бази даних","Створення структури бази даних завершено.");
}

void Admin_page::on_checkAllUsersCheckBox_stateChanged(int arg1)
{
    switch (arg1) {
    case 0:{
        for(int i=0;i<ui->usersListWidget->count();i++){
            ui->usersListWidget->item(i)->setCheckState(Qt::Unchecked);
        }
        break;
    }
    case 2:{
        for(int i=0;i<ui->usersListWidget->count();i++){
            ui->usersListWidget->item(i)->setCheckState(Qt::Checked);
        }
        break;
    }
    default:{
        break;
    }
    }
}

void Admin_page::on_deleteUsersPushButton_clicked()
{
    int countCheckedUsers=0;
    for(int i=0;i<ui->usersListWidget->count();i++) {
        if(ui->usersListWidget->item(i)->checkState()==Qt::Checked){
            QSqlQuery deleteUserQuery("DELETE FROM `warehouse`.`users` WHERE (`login` = '"+checkedUsers.at(i)+"')");
            deleteUserQuery.clear();
            countCheckedUsers++;
        }
    }

    if (countCheckedUsers==0) {
        QMessageBox::about(this,"Видалення користувачів","Жодного користувача не вибрано.");
    } else {
        QMessageBox::about(this,"Видалення користувачів","Вибрані користувачі успішно видалено.");
        addItemToUsersListWidget();
    }
}

void Admin_page::updateUsersListWidget()
{
    addItemToUsersListWidget();
}
