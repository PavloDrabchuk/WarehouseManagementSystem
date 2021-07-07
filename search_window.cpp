#include "search_window.h"
#include "ui_search_window.h"

Search_Window::Search_Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Search_Window)
{
    ui->setupUi(this);

    qDebug()<<"******SEARCH*******";

    visibleAdvancedSearchElements(false);

    ui->saveButton->setDisabled(true);

    ui->minPriceEdit->setValidator(new QDoubleValidator(0, 8888888,2, this));
    ui->maxPriceEdit->setValidator(new QDoubleValidator(0, 8888888,2, this));

    ui->minCountEdit->setValidator(new QIntValidator(0, 8888888, this));
    ui->maxCountEdit->setValidator(new QIntValidator(0, 8888888, this));

    clearAndUpdateForms();
}

Search_Window::~Search_Window()
{
    delete ui;
}

void Search_Window::on_backToMainMenuWindow_clicked()
{
    emit goToHomePage();
}



void Search_Window::on_advancedSearchButton_clicked()
{
    if (ui->advancedSearchButton->text()=="Приховати"){
        visibleAdvancedSearchElements(false);
        ui->advancedSearchButton->setText("Розширені параметри");
    }else{
        visibleAdvancedSearchElements(true);
        ui->advancedSearchButton->setText("Приховати");
    }
}

void Search_Window::on_searchButton_clicked()
{
    ui->resultTableView->setVisible(true);
    QString nameEdit=ui->nameGoodsEdit->text();
    QString searchQuery="SELECT goods.id, goods.batch_code, goods_categories.name,goods.name,goods.count,goods.price,goods.description,goods.producer,goods.producing_country, "
                        " goods.guarantee,goods.weight,goods.width,goods.height,goods.length,goods.color, IF(goods.expiration_date>=88888, 'Необмежений', goods.expiration_date) AS 'expired_date',goods.production_date, "
                        " provider.name,goods.date_arrival "
                        " FROM warehouse.goods "
                        " LEFT JOIN warehouse.provider ON provider.id=goods.provider_id "
                        " LEFT JOIN warehouse.goods_categories ON goods_categories.id=goods.category_id WHERE ";
    searchQuery+=createQueryOrFilterWithParameters();

    QSqlQuery query(searchQuery);

    if(query.size()==0){
        QSqlQueryModel *model=new QSqlQueryModel;
        ui->resultTableView->setModel(model);
        QMessageBox::about(this,"Інформація про пошук товару","За вказаними параметрами товар відсутній.");
    } else {

        QSqlQueryModel *model=new QSqlQueryModel;
        model->setQuery(searchQuery);

        QVector<std::string> columnNames={"ID","Номер партії","Категорія","Назва","Кількість","Вартість","Опис","Виробник","Країна виробника",
                                          "Гарантія","Вага","Ширина","Висота","Довжина","Колір","Термін придатності","Дата виробництва","Постачальник","Дата прибуття"};

        for(int i=0, length=columnNames.length();i<length;i++){
            model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
        }

        ui->resultTableView->setModel(model);
        ui->resultTableView->hideColumn(0);
    }
}

void Search_Window::visibleAdvancedSearchElements(bool s){
    ui->priceLabel->setVisible(s);
    ui->countLabel->setVisible(s);
    ui->minPriceEdit->setVisible(s);
    ui->maxPriceEdit->setVisible(s);
    ui->minCountEdit->setVisible(s);
    ui->maxCountEdit->setVisible(s);
    ui->categoryLabel->setVisible(s);
    ui->categoryComboBox->setVisible(s);
    ui->producerLabel->setVisible(s);
    ui->producerComboBox->setVisible(s);
    ui->providerLabel->setVisible(s);
    ui->providerComboBox->setVisible(s);
    ui->countryProducerLabel->setVisible(s);
    ui->countryProducerComboBox->setVisible(s);
    ui->descriptionLabel->setVisible(s);
    ui->descriptionComboBox->setVisible(s);
}

void Search_Window::addCategoriesToComboBox()
{
    ui->categoryComboBox->clear();
    ui->categoryComboBox->addItem("Усі");

    QString sqlQuery = "SELECT * FROM `warehouse`.`goods_categories`";

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->categoryComboBox->addItem(query.value(2).toString());
    }
    query.clear();
}

void Search_Window::addProducersToComboBox()
{
    ui->producerComboBox->clear();
    ui->producerComboBox->addItem("Усі");

    QString sqlQuery = "SELECT DISTINCT `producer` FROM `warehouse`.`goods`";

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->producerComboBox->addItem(query.value(0).toString());
    }
    query.clear();

}

void Search_Window::addProvidersToComboBox()
{
    ui->providerComboBox->clear();
    ui->providerComboBox->addItem("Усі");

    QString sqlQuery = "SELECT `name` FROM `warehouse`.`provider`";

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->providerComboBox->addItem(query.value(0).toString());
    }
    query.clear();
}

void Search_Window::addCountryProducersToComboBox()
{
    ui->countryProducerComboBox->clear();
    ui->countryProducerComboBox->addItem("Усі");

    QString sqlQuery = "SELECT DISTINCT `producing_country` FROM `warehouse`.`goods`";

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->countryProducerComboBox->addItem(query.value(0).toString());
    }
    query.clear();
}

void Search_Window::addDescriptionStatusToComboBox()
{
    ui->descriptionComboBox->clear();
    ui->descriptionComboBox->addItem("Неважливо");
    ui->descriptionComboBox->addItem("Є");
    ui->descriptionComboBox->addItem("Немає");
}

QString Search_Window::createQueryOrFilterWithParameters()
{
    QString result="goods.name LIKE '%"+ui->nameGoodsEdit->text()+"%'";

    if(ui->minPriceEdit->text()!=""){
        result+=" AND `price`>='"+ui->minPriceEdit->text()+"'";
    }

    if(ui->maxPriceEdit->text()!=""){
        result+=" AND `price`<='"+ui->maxPriceEdit->text()+"'";
    }

    if(ui->minCountEdit->text()!=""){
        result+=" AND `count`>='"+ui->minCountEdit->text()+"'";
    }

    if(ui->maxCountEdit->text()!=""){
        result+=" AND `count`<='"+ui->maxCountEdit->text()+"'";
    }

    if(ui->categoryComboBox->currentText()!="Усі"){
        result+=" AND `category_id`='"+QString::number(ui->categoryComboBox->currentIndex())+"'";
    }

    if(ui->producerComboBox->currentText()!="Усі"){
        result+=" AND `producer`='"+ui->producerComboBox->currentText()+"'";
    }

    if(ui->providerComboBox->currentText()!="Усі"){
        result+=" AND `provider_id`='"+QString::number(ui->providerComboBox->currentIndex())+"'";
    }
    if(ui->countryProducerComboBox->currentText()!="Усі"){
        result+=" AND `producing_country`='"+ui->countryProducerComboBox->currentText()+"'";
    }
    if(ui->descriptionComboBox->currentText()=="Є"){
        result+=" AND `description`!=''";
    }
    if(ui->descriptionComboBox->currentText()=="Немає"){
        result+=" AND `description` is null or `description`=''";
    }
    return result;
}

void Search_Window::clearAndUpdateForms()
{
    ui->nameGoodsEdit->clear();
    addCategoriesToComboBox();
    ui->minPriceEdit->clear();
    ui->maxPriceEdit->clear();
    ui->minCountEdit->clear();
    ui->maxCountEdit->clear();
    addProducersToComboBox();
    addProvidersToComboBox();
    addCountryProducersToComboBox();
    addDescriptionStatusToComboBox();

    QSqlQueryModel *model=new QSqlQueryModel;
    ui->resultTableView->setModel(model);

    visibleAdvancedSearchElements(false);
    ui->saveButton->setDisabled(true);
}

void Search_Window::on_saveButton_clicked()
{
    if(isEdited){
        model->submitAll();
        QMessageBox::about(this,"Інформація про редагування","Інформацію про товар успішно змінено.");
    }else{
        QMessageBox::about(this,"Інформація про редагування","Змін не знайдено.");
    }
}

void Search_Window::on_editButton_clicked()
{
    ui->saveButton->setDisabled(false);
    ui->resultTableView->setVisible(true);
    isEdited=true;
    QString nameEdit=ui->nameGoodsEdit->text();

    QString searchQuery="SELECT goods.id, goods.batch_code, goods_categories.name,goods.name,goods.count,goods.price,goods.description,goods.producer,goods.producing_country, "
                        " goods.guarantee,goods.weight,goods.width,goods.height,goods.length,goods.color,IF(goods.expiration_date>=88888, 'Необмежений',goods.production_date, "
                        " provider.name,goods.date_arrival "
                        " FROM warehouse.goods "
                        " LEFT JOIN warehouse.provider ON provider.id=goods.provider_id "
                        " LEFT JOIN warehouse.goods_categories ON goods_categories.id=goods.category_id WHERE ";
    searchQuery+=createQueryOrFilterWithParameters();

    QSqlQuery query(searchQuery);

    if(query.size()==0){
        QSqlQueryModel *model=new QSqlQueryModel;
        ui->resultTableView->setModel(model);
        QMessageBox::about(this,"Інформація про редагування товару","За вказаними параметрами товар відсутній.");
    } else {

        QString filter=createQueryOrFilterWithParameters();

        model=new QSqlTableModel;
        model->setTable("goods");
        model->setFilter(filter);

        model->setEditStrategy(QSqlTableModel::OnRowChange);
        model->select();

        QVector<std::string> columnNames={"ID","Номер партії","ID категорії","Назва","Кількість","Вартість","Опис","Виробник","Країна виробника","Гарантія","Вага",
                                          "Ширина","Висота","Довжина","Колір","Термін придатності","Дата виробництва","ID постачальника","Дата прибуття"};

        for(int i=0, length=columnNames.length();i<length;i++){
            model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
        }

        ui->resultTableView->setModel(model);
        ui->resultTableView->hideColumn(0);
    }
}
