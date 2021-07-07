#include "write_off_goods.h"
#include "ui_write_off_goods.h"

Write_Off_Goods::Write_Off_Goods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Write_Off_Goods)
{
    ui->setupUi(this);

    qDebug()<<"******WRITE OFF*******";

    visibleExpiredElements(false);
    visibleDamageElements(false);
    ui->messageLabel->setText("");
}

Write_Off_Goods::~Write_Off_Goods()
{
    delete ui;
}

void Write_Off_Goods::visibleExpiredElements(bool status)
{
    ui->write_offGoodsButton->setVisible(status);
}

void Write_Off_Goods::visibleDamageElements(bool status)
{
    ui->deleteDamageGoodsPushButton->setVisible(status);
}

void Write_Off_Goods::clearAndUpdateForms()
{
    ui->messageLabel->setText("");
    QSqlQueryModel *model=new QSqlQueryModel;
    ui->goodsTableView->setModel(model);
    ui->deleteDamageGoodsPushButton->setVisible(false);
    ui->write_offGoodsButton->setVisible(false);
}

void Write_Off_Goods::on_getExpiredGoodsButton_clicked()
{
    ui->deleteDamageGoodsPushButton->setVisible(false);

    QString queryExpiredGoods = "SELECT `goods`.`id`,`goods`.`batch_code`,`goods_categories`.`name`,`goods`.`name`,`goods`.`count`,`goods`.`producer`,`goods`.`production_date`,`goods`.`expiration_date` FROM warehouse.`goods`  LEFT JOIN `warehouse`.`goods_categories` ON `goods_categories`.`id`=`goods`.`category_id` WHERE DATE_ADD(`production_date`, INTERVAL `expiration_date` MONTH)<CURDATE()";

    QSqlQuery q(queryExpiredGoods);
    if(q.size()!=0){
        visibleExpiredElements(true);
        QSqlQueryModel *model=new QSqlQueryModel;
        model->setQuery(queryExpiredGoods);

        QVector<std::string> columnNames={"ID","Номер партії","Категорія","Назва","Кількість","Виробник","Дата виробництва","Термін придатності\n(міс.)"};

        for(int i=0, length=columnNames.length();i<length;i++){
            model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
        }

        ui->goodsTableView->setModel(model);
        ui->write_offGoodsButton->setVisible(true);
        ui->messageLabel->setText("");
    }  else {
        ui->messageLabel->setVisible(true);
        ui->messageLabel->setText("Протермінований товар відсутній на складі.");
        ui->write_offGoodsButton->setVisible(false);
        QSqlQueryModel *model=new QSqlQueryModel;
        ui->goodsTableView->setModel(model);
    }
}

void Write_Off_Goods::on_getDamagedGoodsButton_clicked()
{
    ui->write_offGoodsButton->setVisible(false);
    QString queryDamagedGoods = "SELECT `damaged_goods`.`batch_code`, `damaged_goods`.`name_goods`,CONCAT(`users`.`name`,' ',`users`.`surname`) AS 'user_name',`damaged_goods`.`count`,`damaged_goods`.`cause_damage`,`damaged_goods`.`pest`,`damaged_goods`.`date_of_damage`,`damaged_goods`.`registration_date` "
                                " FROM warehouse.`damaged_goods` "
                                " LEFT JOIN warehouse.`users` ON `damaged_goods`.`user_id`=`users`.`id` WHERE `damaged_goods`.`status`='Не утилізований'";

    QSqlQuery q(queryDamagedGoods);
    if(q.size()!=0){
        ui->goodsTableView->setVisible(true);

        visibleDamageElements(true);
        QSqlQueryModel *model=new QSqlQueryModel;
        model->setQuery(queryDamagedGoods);

        QVector<std::string> columnNames={"Номер партії","Назва","Працівник","Кількість","Причина","Хто пошкодив","Дата пошкодження","Дата реєстрації"};

        for(int i=0, length=columnNames.length();i<length;i++){
            model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
        }

        ui->goodsTableView->setModel(model);

        ui->deleteDamageGoodsPushButton->setVisible(true);
        ui->messageLabel->setText("");
    }  else {
        ui->messageLabel->setVisible(true);
        ui->messageLabel->setText("Пошкоджений товар відсутній на складі.");
        ui->deleteDamageGoodsPushButton->setVisible(false);

        QSqlQueryModel *model=new QSqlQueryModel;
        ui->goodsTableView->setModel(model);
    }
}

void Write_Off_Goods::on_goToHomePageButton_clicked()
{
    emit goToHomePage();
}

void Write_Off_Goods::on_write_offGoodsButton_clicked()
{
    QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Списування протермінованого товару","Ви справді бажаєте списати даний товар?",QMessageBox::Yes | QMessageBox::No);

    if(answerExit==QMessageBox::Yes){
        QString sqlQuery="INSERT INTO warehouse.expired_goods (batch_code, goods_id, category_id, goods_name, count, producer,production_date,expiration_date,write_off_date) (SELECT `goods`.`batch_code`,  `goods`.`id`,`goods`.`category_id`,`goods`.`name`,`goods`.`count`,`goods`.`producer`,`goods`.`production_date`,`goods`.`expiration_date`,now() FROM goods  WHERE DATE_ADD(`production_date`, INTERVAL `expiration_date` MONTH)<CURDATE())";
        QSqlQuery query(sqlQuery);
        QSqlQuery("DELETE FROM warehouse.goods WHERE DATE_ADD(`production_date`, INTERVAL `expiration_date` MONTH)<CURDATE()");
        QMessageBox::about(this,"Інформація про списування протермінованого товару","Товар успішно списано.");

        ui->messageLabel->setVisible(true);
        ui->messageLabel->setText("Протермінований товар відсутній на складі.");
        ui->write_offGoodsButton->setVisible(false);

        QSqlQueryModel *model=new QSqlQueryModel;
        ui->goodsTableView->setModel(model);

        emit updateWriteOffGoodsPushButton();
    }
}

void Write_Off_Goods::on_deleteDamageGoodsPushButton_clicked()
{
    QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Утилізація товару","Ви справді бажаєте утилізувати увесь пошкоджений товар?",QMessageBox::Yes | QMessageBox::No);

    if(answerExit==QMessageBox::Yes){
        QString sqlQuery="INSERT INTO `warehouse`.`recycled goods` (`recycled goods`.`batch_code`, `recycled goods`.`name_goods`, `recycled goods`.`count`,`recycled goods`.`damage_date`, `recycled goods`.`date`) "
                         " (SELECT damaged_goods.batch_code,damaged_goods.name_goods,damaged_goods.`count`,damaged_goods.date_of_damage,now() FROM warehouse.damaged_goods WHERE damaged_goods.status='Не утилізований')";
        QSqlQuery query(sqlQuery);

        QSqlQuery("UPDATE `warehouse`.`damaged_goods` SET `status` = 'Утилізований' WHERE (`status` = 'Не утилізований')");
        QMessageBox::about(this,"Інформація про утилізацію пошкодженого товару","Товар успішно утилізовано.");

        ui->messageLabel->setVisible(true);
        ui->messageLabel->setText("Пошкоджений товар відсутній на складі.");
        ui->deleteDamageGoodsPushButton->setVisible(false);

        QSqlQueryModel *model=new QSqlQueryModel;
        ui->goodsTableView->setModel(model);

        emit updateWriteOffGoodsPushButton();
    }
}
