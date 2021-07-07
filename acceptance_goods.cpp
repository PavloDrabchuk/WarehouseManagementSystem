#include "acceptance_goods.h"
#include "ui_acceptance_goods.h"

Acceptance_Goods::Acceptance_Goods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Acceptance_Goods)
{
    ui->setupUi(this);
    qDebug()<<"******ACEPTANCE*******";
    isLoaded=1;

    addGoodsWindows=new Add_Goods(this);
    addGoodsGroup=new Add_goods_group(this);
    addGoodCategory=new Add_good_category(this);
    addProvider=new Add_Provider(this);

    connect(addGoodsWindows, SIGNAL(addGoodToTable()),SLOT(addGoods()));
}

Acceptance_Goods::~Acceptance_Goods()
{
    delete ui;
}

void Acceptance_Goods::initTable(){
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery("SELECT warehouse.acceptance_goods_history.name, warehouse.acceptance_goods_history.date, warehouse.acceptance_goods_history.number, warehouse.provider.name, warehouse.acceptance_goods_history.count, warehouse.acceptance_goods_history.cost FROM warehouse.acceptance_goods_history LEFT JOIN warehouse.provider on provider.id = warehouse.acceptance_goods_history.provider_id ORDER BY `date` DESC LIMIT 20");

    QVector<std::string> columnNames={"Назва","Дата","Номер","Постачальник","Кількість","Вартість"};

    for(int i=0, length=columnNames.length();i<length;i++){
        model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
    }

    ui->historyAcceptanceTableView->setModel(model);

    QSqlQuery query("SELECT warehouse.acceptance_goods_history.date, warehouse.acceptance_goods_history.number, warehouse.provider.name, warehouse.acceptance_goods_history.count, warehouse.acceptance_goods_history.cost FROM warehouse.acceptance_goods_history LEFT JOIN warehouse.provider on provider.id = warehouse.acceptance_goods_history.provider_id");
}

void Acceptance_Goods::clearAndUpdateForms()
{
    initTable();
    addGoodsWindows->clearAndUpdateForms();
}

int Acceptance_Goods::getIsLoaded()
{
    return isLoaded;
}

void Acceptance_Goods::on_addGoodsButton_clicked()
{    
    addGoodsWindows->clearAndUpdateForms();

    addGoodsWindows->setModal(true);
    addGoodsWindows->setWindowTitle("Приймання товару");

    QIcon appIcon=QIcon("images/logo_warehouse.png");
    addGoodsWindows->setWindowIcon(appIcon);

    addGoodsWindows->exec();
}

void Acceptance_Goods::addGoods()
{   
    QVector<QString> infoFromAddGoodsForm=addGoodsWindows->getAddInfo();

    QDateTime dateTime=QDateTime::currentDateTime();
    QString dateTimeFormated=dateTime.toString("yyyy-MM-dd-hh:mm:ss");

    QSqlQuery idLastGoodQuery("SELECT `id` FROM warehouse.`goods` ORDER BY `id` DESC LIMIT 1");
    idLastGoodQuery.next();
    int idLastGood=idLastGoodQuery.value(0).toInt()+1;
    QString number=QString::number(idLastGood)+"_"+infoFromAddGoodsForm.at(15)+"_"+infoFromAddGoodsForm.at(0)+"_"+dateTimeFormated;

    QString sqlQueryAddGoods="INSERT INTO `warehouse`.`goods` (`batch_code`,`category_id`, `name`, `count`, `price`, `description`, `producer`, `producing_country`, `guarantee`, `weight`, `width`, `height`, `length`, `color`, `expiration_date`,`production_date`, `provider_id`, `date_arrival`) VALUES (";

    sqlQueryAddGoods+="'"+number+"',";
    for(auto goodsValue:infoFromAddGoodsForm){
        sqlQueryAddGoods+="'"+goodsValue+"',";
    }

    sqlQueryAddGoods+="'"+dateTimeFormated+"');";

    QSqlQuery queryInsertGoods(sqlQueryAddGoods);

    QString sqlQueryAddHistoryAccept="INSERT INTO `warehouse`.`acceptance_goods_history` (`name`,`date`, `number`, `provider_id`, `count`, `cost`) VALUES ('"+infoFromAddGoodsForm.at(1)+"', '"+dateTimeFormated+"', '"+number+"', '"+infoFromAddGoodsForm.at(15)+"', '"+infoFromAddGoodsForm.at(2)+"', '"+infoFromAddGoodsForm.at(3)+"');";
    QSqlQuery queryInsertAddGoodsHistory(sqlQueryAddHistoryAccept);

    initTable();
    emit updateWriteOffGoodsPushButton();
}

void Acceptance_Goods::on_goToHomePushButton_clicked()
{
    emit goToHomePage();
}

void Acceptance_Goods::on_addGoodsGroupPushButton_clicked()
{
    addGoodsGroup->setModal(true);
    addGoodsGroup->setWindowTitle("Додавання групи категорій товару");

    addGoodsGroup->clearAndUpdateForms();

    QIcon appIcon=QIcon("images/logo_warehouse.png");
    addGoodsGroup->setWindowIcon(appIcon);

    addGoodsGroup->exec();
}

void Acceptance_Goods::on_addGoodsCategoriesPushButton_clicked()
{
    addGoodCategory->setModal(true);
    addGoodCategory->setWindowTitle("Додавання категорії товару");

    addGoodCategory->clearAndUpdateForms();

    QIcon appIcon=QIcon("images/logo_warehouse.png");
    addGoodCategory->setWindowIcon(appIcon);

    addGoodCategory->exec();
}

void Acceptance_Goods::on_addProviderPushButton_clicked()
{
    addProvider->setModal(true);
    addProvider->setWindowTitle("Додавання постачальника товару");

    addProvider->clearAndUpdateForms();

    /*QIcon appIcon=QIcon("images/logo_warehouse.png");
    addGoodCategory->setWindowIcon(appIcon);*/

    addProvider->exec();
}
