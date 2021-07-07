#include "damage_goods_page.h"
#include "ui_damage_goods_page.h"


Damage_Goods_Page::Damage_Goods_Page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Damage_Goods_Page)
{
    ui->setupUi(this);

    qDebug()<<"******DAMAGE*******";

    addCategoriesToComboBox();
    addNameGoodsToComboBox(-1);
    addPestToComboBox();

    QDate today=QDate::currentDate();
    ui->dateDamageDateEdit->setDate(today);
}

Damage_Goods_Page::~Damage_Goods_Page()
{
    delete ui;
}

void Damage_Goods_Page::on_addInfoAboutDamageGoodsPushButton_clicked()
{
    if(checkCountGoods() && checkInputForm()){
        QString nameGoods=ui->nameGoodsComboBox->currentText(),
                countDamageGoods=ui->countDamageGoodsSpinBox->text(),
                causeDamageGoods=ui->causeDamageLineEdit->text(),
                pest=ui->pestComboBox->currentText(),
                dateDamage=ui->dateDamageDateEdit->date().toString("yyyy-MM-dd"),
                idGoods=getIdGoods(ui->codeBatchComboBox->currentText()),
                batchCode=ui->codeBatchComboBox->currentText();

        QString insertQuery="INSERT INTO `warehouse`.`damaged_goods` (`batch_code`,`id_goods`, `name_goods`, `user_id`, `count`, `cause_damage`, `pest`, `date_of_damage`, `registration_date`, `status`) VALUES "
                            " ('"+batchCode+"', '"+idGoods+"', '"+nameGoods+"', '"+QString::number(userId)+"', '"+countDamageGoods+"', '"+causeDamageGoods+"', '"+pest+"', '"+dateDamage+"', now(), 'Не утилізований')";

        QSqlQuery query(insertQuery);
        query.clear();

        QSqlQuery updateQuery("UPDATE `warehouse`.`goods` SET `count` = `count`-"+ui->countDamageGoodsSpinBox->text()+" WHERE (`name` = '"+ui->nameGoodsComboBox->currentText()+"' AND `batch_code`='"+ui->codeBatchComboBox->currentText()+"')");
        QMessageBox::about(this,"Інформація про занесення інформації про пошкоджений товар","Інформацію успішно додано.");
emit updateWriteOffPage();
    }else{
        QMessageBox::warning(this,"Помилка при занесенні інформації про пошкоджений товар","Дані не збережено, перевірте правильність введених даних.");
    }
}

void Damage_Goods_Page::addCategoriesToComboBox()
{
    ui->categoryGoodsComboBox->clear();
    ui->categoryGoodsComboBox->addItem("Оберіть категорію");

    QString sqlQuery="SELECT * FROM warehouse.goods_categories";

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->categoryGoodsComboBox->addItem(query.value(2).toString());
    }
    query.clear();
}

void Damage_Goods_Page::addNameGoodsToComboBox(int index)
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

void Damage_Goods_Page::addBatchCodesToComboBox(QString index)
{
    ui->codeBatchComboBox->clear();
    ui->codeBatchComboBox->addItem("Оберіть номер партії");

    QSqlQuery query("SELECT `batch_code` FROM warehouse.`goods` WHERE `name`='"+index+"';");
    while (query.next()) {
        ui->codeBatchComboBox->addItem(query.value(0).toString());
    }
    query.clear();
}

void Damage_Goods_Page::addPestToComboBox()
{
    ui->pestComboBox->clear();
    QStringList pestList={"Працівник","Покупець"};
    ui->pestComboBox->addItems(pestList);
}

QString Damage_Goods_Page::getIdGoods(QString codeBatch)
{
    int pos=codeBatch.indexOf("_");
    return codeBatch.left(pos);
}

bool Damage_Goods_Page::checkCountGoods()
{
    QString sqlQueryGetCount="SELECT count FROM warehouse.goods WHERE `name`='"+ui->nameGoodsComboBox->currentText()+"' AND `batch_code`='"+ui->codeBatchComboBox->currentText()+"'";
    QSqlQuery queryCountGoods(sqlQueryGetCount);

    if (queryCountGoods.size()!=0){
        queryCountGoods.next();
        return (ui->countDamageGoodsSpinBox->value()>queryCountGoods.value(0).toInt()) ? false : true;
    } else return false;
}

bool Damage_Goods_Page::checkInputForm()
{            
    if(ui->categoryGoodsComboBox->currentIndex()!=0 &&
            ui->nameGoodsComboBox->currentIndex()!=0 &&
            ui->nameGoodsComboBox->currentIndex()!=0 &&
            ui->countDamageGoodsSpinBox->text()!="" &&
            ui->causeDamageLineEdit->text()!="" &&
            ui->dateDamageDateEdit->date()<=QDate::currentDate())return true;
    else return false;

}

void Damage_Goods_Page::setUserId(int id)
{
    userId=id;
}

void Damage_Goods_Page::clearAndUpdateForms()
{
    addBatchCodesToComboBox("-1");
    addCategoriesToComboBox();
    addNameGoodsToComboBox(-1);
    ui->causeDamageLineEdit->clear();
    ui->countDamageGoodsSpinBox->setValue(0);
    QDate today=QDate::currentDate();
    ui->dateDamageDateEdit->setDate(today);
    addPestToComboBox();
}

void Damage_Goods_Page::on_categoryGoodsComboBox_currentIndexChanged(int index)
{
    addNameGoodsToComboBox(index);
}

void Damage_Goods_Page::on_nameGoodsComboBox_currentTextChanged(const QString &arg1)
{
    addBatchCodesToComboBox(arg1);
}

void Damage_Goods_Page::on_goToHomePushButton_clicked()
{
    emit goToHomePage();
}
