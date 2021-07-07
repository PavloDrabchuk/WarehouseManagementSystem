#include "add_good_category.h"
#include "ui_add_good_category.h"

Add_good_category::Add_good_category(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_good_category)
{
    ui->setupUi(this);

    addGroupGoodsToComboBox();
}

Add_good_category::~Add_good_category()
{
    delete ui;
}

void Add_good_category::clearAndUpdateForms()
{
    ui->goodsCategoryLineEdit->clear();
    ui->groupGoodsComboBox->setCurrentIndex(0);
    ui->errorMessageLabel->setVisible(false);
    addGroupGoodsToComboBox();
}


void Add_good_category::on_addCategoryPushButton_clicked()
{
    if(checkInputForm()){
        qDebug()<<"id: "<<ui->groupGoodsComboBox->currentIndex();
        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Додавання категорії товару","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){

            QSqlQuery addGoodsGroupQuery("INSERT INTO `warehouse`.`goods_categories` (`group_id`, `name`) VALUES ('"+QString::number(ui->groupGoodsComboBox->currentIndex())+"', '"+ui->goodsCategoryLineEdit->text()+"');");

            ui->errorMessageLabel->setVisible(false);
            QMessageBox::about(this,"Додавання категорії товару","Категорію товару успішно додано.");
            this->close();
        }
    } else {
        QMessageBox::warning(this,"Додавання категорії товару","Перевірте правильність введених даних.");
    }
}

void Add_good_category::on_backPushButton_clicked()
{
    this->close();
}

bool Add_good_category::checkInputForm()
{
    bool result=true;

    QSqlQuery checkGroupNameQuery("SELECT * FROM warehouse.goods_categories WHERE `name`='"+ui->goodsCategoryLineEdit->text()+"'");
    checkGroupNameQuery.next();
    if(checkGroupNameQuery.size()>0){
        result=false;
        ui->errorMessageLabel->setText("Така група категорій вже існує");
        ui->errorMessageLabel->setVisible(true);
    }

    if(ui->groupGoodsComboBox->currentIndex()==0){
        ui->errorMessageLabel->setText("Оберіть групу категорії");
        ui->errorMessageLabel->setVisible(true);
        result=false;
    }

    if(ui->goodsCategoryLineEdit->text()=="" || ui->goodsCategoryLineEdit->text()=="-" || ui->goodsCategoryLineEdit->text().length()<=2){
        ui->errorMessageLabel->setText("Введіть назву групи");
        ui->errorMessageLabel->setVisible(true);
        result=false;
    }

    return result;
}

void Add_good_category::addGroupGoodsToComboBox()
{
    ui->groupGoodsComboBox->clear();
    ui->groupGoodsComboBox->addItem("Оберіть групу товару");
    QSqlQuery query("SELECT * FROM warehouse.goods_group;");
    while (query.next()) {
        ui->groupGoodsComboBox->addItem(query.value(1).toString());
    }
    query.clear();
}
