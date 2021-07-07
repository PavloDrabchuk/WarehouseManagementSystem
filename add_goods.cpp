#include "add_goods.h"
#include "ui_add_goods.h"

Add_Goods::Add_Goods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_Goods)
{
    ui->setupUi(this);

    qDebug()<<"******ADD GOODS*******";
    this->setWindowIcon(QIcon("images/logo_warehouse.png"));

    addProvidersToComboBox();

    addGroupCategoriesToComboBox();
    addCategoriesToComboBox(-1);

    ui->enterDataToFieldLabel->setText("");
}

Add_Goods::~Add_Goods()
{
    delete ui;
}

QString Add_Goods::getName()
{
    return ui->nameEdit->text();
}

QVector<QString> Add_Goods::getAddInfo()
{
    QSqlQuery getCategoryIdByNameQuery("SELECT `id` FROM warehouse.goods_categories WHERE `name`='"+ui->categoryComboBox->currentText()+"'");
    getCategoryIdByNameQuery.next();


    addInfo.clear();
    addInfo.push_back(getCategoryIdByNameQuery.value(0).toString());
    getCategoryIdByNameQuery.clear();
    addInfo.push_back(ui->nameEdit->text());
    addInfo.push_back(ui->countSpinBox->text());
    addInfo.push_back(ui->priceDoubleSpinBox->text());
    addInfo.push_back(ui->descriptionPlainTextEdit->toPlainText());
    addInfo.push_back(ui->producerEdit->text());
    addInfo.push_back(ui->producingCountryEdit->text());
    addInfo.push_back(ui->guaranteeSpinBox->text());
    addInfo.push_back(ui->weightDoubleSpinBox->text());
    addInfo.push_back(ui->widthDoubleSpinBox->text());
    addInfo.push_back(ui->heightDoubleSpinBox->text());
    addInfo.push_back(ui->lengthDoubleSpinBox->text());
    addInfo.push_back(ui->colorEdit->text());
    addInfo.push_back(ui->expirationDateSpinBox->text());

    QDate dateProduction=ui->productionDateDateEdit->date();
    QString dateProductionFormated=dateProduction.toString("yyyy-MM-dd");
    addInfo.push_back(dateProductionFormated);

    addInfo.push_back(QString::number(ui->providerComboBox->currentIndex()));

    return addInfo;
}

void Add_Goods::addProvidersToComboBox()
{
    ui->providerComboBox->clear();
    ui->providerComboBox->addItem("Оберіть постачальника");
    QSqlQuery query("SELECT * FROM warehouse.provider");
    while (query.next()) {
        ui->providerComboBox->addItem(query.value(1).toString());
    }
    query.clear();
}

void Add_Goods::addGroupCategoriesToComboBox()
{
    ui->groupGoodsComboBox->clear();
    ui->groupGoodsComboBox->addItem("Оберіть групу товару");
    QSqlQuery query("SELECT * FROM warehouse.goods_group;");
    while (query.next()) {
        ui->groupGoodsComboBox->addItem(query.value(1).toString());
    }
    query.clear();
}

void Add_Goods::addCategoriesToComboBox(int index)
{
    ui->categoryComboBox->clear();
    ui->categoryComboBox->addItem("Оберіть категорію");

    QString sqlQuery;
    (index==-1) ? sqlQuery="SELECT * FROM warehouse.goods_categories" : sqlQuery ="SELECT * FROM warehouse.goods_categories WHERE group_id="+QString::number(index);

    QSqlQuery query(sqlQuery);
    while (query.next()) {
        ui->categoryComboBox->addItem(query.value(2).toString());
    }
    query.clear();
}

bool Add_Goods::checkGoodsFields()
{
    if(ui->nameEdit->text()!="" &&
            ui->countSpinBox->text() !="" &&
            ui->priceDoubleSpinBox->text() !="0" &&
            ui->groupGoodsComboBox->currentIndex()!=0 &&
            ui->categoryComboBox->currentIndex()!=0 &&
            ui->producerEdit->text() !="" &&
            ui->producingCountryEdit->text() !="" &&
            ui->weightDoubleSpinBox->text() !="0" &&
            ui->widthDoubleSpinBox->text() !="0" &&
            ui->heightDoubleSpinBox->text() !="0" &&
            ui->lengthDoubleSpinBox->text() !="0" &&
            ui->colorEdit->text()!="" &&
            ui->expirationDateSpinBox->text()!="0" &&
            ui->productionDateDateEdit->date().toString("yyyy-MM-dd")!="2000-01-01" &&
            ui->providerComboBox->currentIndex()!=0) {
        ui->enterDataToFieldLabel->setVisible(false);
        return true;
    }
    else {
        ui->enterDataToFieldLabel->setText("Заповніть усі поля.");
        return false;
    }
}

void Add_Goods::on_closeButton_clicked()
{
    close();
}

void Add_Goods::on_saveButton_clicked()
{
    if(checkGoodsFields()) {
        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Збереження","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){
            emit addGoodToTable();
            QMessageBox::about(this,"Інформація про приймання товару","Товар успішно прийнято.");
            this->close();
        }

    }
}

void Add_Goods::on_groupGoodsComboBox_currentIndexChanged(int index)
{
    addCategoriesToComboBox(index);
}

void Add_Goods::clearAndUpdateForms()
{
    ui->nameEdit->clear();
    addProvidersToComboBox();
    ui->expirationDateSpinBox->setValue(0);
    ui->guaranteeSpinBox->setValue(0);
    ui->weightDoubleSpinBox->setValue(0.0);
    ui->priceDoubleSpinBox->setValue(0.0);
    QDate prodDate(2000,1,1);
    ui->productionDateDateEdit->setDate(prodDate);
    ui->colorEdit->clear();
    ui->descriptionPlainTextEdit->clear();
    ui->producingCountryEdit->clear();
    ui->producerEdit->clear();
    ui->countSpinBox->setValue(0);
    addCategoriesToComboBox(-1);
    addGroupCategoriesToComboBox();
    ui->heightDoubleSpinBox->setValue(0.0);
    ui->lengthDoubleSpinBox->setValue(0.0);
    ui->widthDoubleSpinBox->setValue(0.0);
    ui->enterDataToFieldLabel->setText("");
}
