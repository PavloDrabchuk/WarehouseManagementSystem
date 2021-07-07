#include "delivery_goods.h"
#include "ui_delivery_goods.h"

Delivery_Goods::Delivery_Goods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Delivery_Goods)
{
    ui->setupUi(this);

    qDebug()<<"******DELIVERY*******";

    ui->countLabel->setVisible(false);

    addCategoriesToComboBox(-1);
    addGroupCategoriesToComboBox();
    addGoodsToComboBox(-1);
    addBatchCodeToComboBox("-1");
}

Delivery_Goods::~Delivery_Goods()
{
    delete ui;
}

void Delivery_Goods::clearAndUpdateForms()
{
    addCategoriesToComboBox(-1);
    addBatchCodeToComboBox("-1");    
    addGroupCategoriesToComboBox();
    addGoodsToComboBox(-1);
    ui->countGoodsSpinBox->setValue(0);
    ui->countLabel->setVisible(false);
}

void Delivery_Goods::addGroupCategoriesToComboBox()
{
    ui->groupGoodsComboBox->clear();
    ui->groupGoodsComboBox->addItem("Оберіть групу товару:");
    QSqlQuery query("SELECT * FROM warehouse.goods_group;");
    while (query.next()) {
        ui->groupGoodsComboBox->addItem(query.value(1).toString());
    }
    query.clear();
}

void Delivery_Goods::addCategoriesToComboBox(int index)
{
    ui->categoryGoodsComboBox->clear();
    ui->categoryGoodsComboBox->addItem("Оберіть категорію");

    QString sqlQuery;
    (index==-1 || index==0) ? sqlQuery="SELECT * FROM warehouse.goods_categories" : sqlQuery ="SELECT * FROM warehouse.goods_categories WHERE group_id="+QString::number(index);

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->categoryGoodsComboBox->addItem(query.value(2).toString());
    }
    query.clear();
}

void Delivery_Goods::addGoodsToComboBox(int index)
{
    ui->nameGoodsComboBox->clear();
    ui->nameGoodsComboBox->addItem("Оберіть товар");

    QSqlQuery getCategoryIdByNameQuery("SELECT `id` FROM warehouse.goods_categories WHERE `name`='"+ui->categoryGoodsComboBox->currentText()+"'");
    getCategoryIdByNameQuery.next();

    QString sqlQuery;
    (index==-1) ? sqlQuery="SELECT DISTINCT `name` FROM warehouse.goods" : sqlQuery ="SELECT DISTINCT `name` FROM warehouse.goods WHERE category_id="+getCategoryIdByNameQuery.value(0).toString();
    getCategoryIdByNameQuery.clear();

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->nameGoodsComboBox->addItem(query.value(0).toString());
    }
    query.clear();
}

void Delivery_Goods::addBatchCodeToComboBox(QString goodsName)
{
    ui->batchCodeComboBox->clear();
    ui->batchCodeComboBox->addItem("Оберіть номер партії");

    QString sqlQuery;
    (goodsName=="-1") ? sqlQuery="SELECT `batch_code` FROM `warehouse`.`goods`" : sqlQuery ="SELECT `batch_code` FROM `warehouse`.`goods` WHERE `name`='"+goodsName+"'";

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->batchCodeComboBox->addItem(query.value(0).toString());
    }
    query.clear();
}

bool Delivery_Goods::checkCountGoods()
{
    QSqlQuery getCategoryIdByNameQuery("SELECT `id` FROM warehouse.goods_categories WHERE `name`='"+ui->categoryGoodsComboBox->currentText()+"'");
    getCategoryIdByNameQuery.next();

    QString sqlQueryGetCount="SELECT count from warehouse.goods WHERE name='"+ui->nameGoodsComboBox->currentText()+"' AND category_id='"+getCategoryIdByNameQuery.value(0).toString()+"'";
    getCategoryIdByNameQuery.clear();

    QSqlQuery queryCountGoods(sqlQueryGetCount);

    if (queryCountGoods.size()!=0){
        queryCountGoods.next();
        return (ui->countGoodsSpinBox->value()>queryCountGoods.value(0).toInt()) ? false : true;
    } else return false;
}

bool Delivery_Goods::checkGoodsAndForm()
{
    QString sqlQuery="SELECT * FROM warehouse.goods WHERE DATE_ADD(`production_date`, INTERVAL `expiration_date` MONTH)>CURDATE() AND `name`='"+ui->nameGoodsComboBox->currentText()+"'";
    QSqlQuery queryCount(sqlQuery);

    if(queryCount.size()!=0 &&
            ui->groupGoodsComboBox->currentIndex()!=0 &&
            ui->categoryGoodsComboBox->currentIndex()!=0 &&
            ui->nameGoodsComboBox->currentIndex()!=0 &&
            ui->batchCodeComboBox->currentIndex()!=0){
        return true;
    }else {
        return false;
    }
}

int Delivery_Goods::getCountGoods(QString name)
{
    QString sqlQueryGetCount="SELECT count FROM warehouse.goods WHERE batch_code='"+name+"'";
    QSqlQuery queryCountGoods(sqlQueryGetCount);

    if (queryCountGoods.size()!=0){
        queryCountGoods.next();
        return queryCountGoods.value(0).toInt();
    } else return -1;
}

void Delivery_Goods::on_sendButton_clicked()
{
    QSqlQuery getCategoryIdByNameQuery("SELECT `id` FROM warehouse.goods_categories WHERE `name`='"+ui->categoryGoodsComboBox->currentText()+"'");
    getCategoryIdByNameQuery.next();

    if(checkCountGoods() && checkGoodsAndForm()){

        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Видача товару","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){
            QSqlQuery updateQuery("UPDATE `warehouse`.`goods` SET `count` = `count`-"+ui->countGoodsSpinBox->text()+" WHERE (`name` = '"+ui->nameGoodsComboBox->currentText()+"' AND `category_id`='"+getCategoryIdByNameQuery.value(0).toString()+"')");



            if(getCountGoods(ui->batchCodeComboBox->currentText())<=0){
                ui->countLabel->setText("Цей товар закінчився");
            } else {
                ui->countLabel->setText("На складі є: "+QString::number(getCountGoods(ui->batchCodeComboBox->currentText())));
            }
            QMessageBox::about(this,"Інформація про видачу товару","Товар успішно видано.");

            QSqlQuery addToDeliveryHistoryQuery("INSERT INTO `warehouse`.`delivery_goods_history` (`batch_code`, `category_id`, `name`, `date`, `count`) VALUES ('"+ui->batchCodeComboBox->currentText()+"','"+getCategoryIdByNameQuery.value(0).toString()+"', '"+ui->nameGoodsComboBox->currentText()+"', '"+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"', '"+ui->countGoodsSpinBox->text()+"')");
           getCategoryIdByNameQuery.clear();
            emit updateWriteOffGoodsPushButton();
        }

    }
    else {
        QMessageBox::warning(this,"Інформація про видачу товару","Товар не видано, перевірте правильність введених даних.");
    }
}

void Delivery_Goods::on_goToHomeButton_clicked()
{
    emit goToHomePage();
}

void Delivery_Goods::on_groupGoodsComboBox_currentIndexChanged(int index)
{
    addCategoriesToComboBox(index);
    ui->countLabel->setText("");
}

void Delivery_Goods::on_categoryGoodsComboBox_currentIndexChanged(int index)
{
    addGoodsToComboBox(index);
}


void Delivery_Goods::on_nameGoodsComboBox_currentTextChanged(const QString &arg1)
{
    addBatchCodeToComboBox(arg1);
}

void Delivery_Goods::on_batchCodeComboBox_currentTextChanged(const QString &arg1)
{
    if(getCountGoods(arg1)!=-1){
        ui->countLabel->setVisible(true);
        if(getCountGoods(arg1)==0){
            ui->countLabel->setText("Цей товар відсутній на складі.");
        } else {
            ui->countLabel->setText("На складі є: "+QString::number(getCountGoods(arg1)));
        }
    }
}
