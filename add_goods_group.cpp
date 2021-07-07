#include "add_goods_group.h"
#include "ui_add_goods_group.h"

Add_goods_group::Add_goods_group(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_goods_group)
{
    ui->setupUi(this);

    ui->errorMessageLabel->setVisible(false);
}

Add_goods_group::~Add_goods_group()
{
    delete ui;
}

void Add_goods_group::clearAndUpdateForms()
{
    ui->errorMessageLabel->setVisible(false);
    ui->groupCategoryLineEdit->clear();
}

void Add_goods_group::on_addGroupCategoryPushButton_clicked()
{
    if(checkInputForm()){
        QMessageBox::StandardButton answerExit = QMessageBox::question(this, "Додавання групи категорій товару","Все добре введено?",QMessageBox::Yes | QMessageBox::No);

        if(answerExit==QMessageBox::Yes){
            QSqlQuery addGoodsGroupQuery("INSERT INTO `warehouse`.`goods_group` (`group_name`) VALUES ('"+ui->groupCategoryLineEdit->text()+"')");

            ui->errorMessageLabel->setVisible(false);
            QMessageBox::about(this,"Додавання групи категорій товару","Групу категорій товару успішно додано.");
            this->close();
        }
    } else {
        QMessageBox::warning(this,"Додавання групи категорій товару","Перевірте правильність введених даних.");
    }

}

bool Add_goods_group::checkInputForm()
{
    bool result=true;

    QSqlQuery checkGroupNameQuery("SELECT * FROM warehouse.goods_group WHERE `group_name`='"+ui->groupCategoryLineEdit->text()+"'");
    checkGroupNameQuery.next();
    if(checkGroupNameQuery.size()>0){
        result=false;
        ui->errorMessageLabel->setText("Така група категорій вже існує");
        ui->errorMessageLabel->setVisible(true);
    }

    if(ui->groupCategoryLineEdit->text()=="" || ui->groupCategoryLineEdit->text()=="-" || ui->groupCategoryLineEdit->text().length()<=2){
        ui->errorMessageLabel->setText("Введіть назву групи");
        ui->errorMessageLabel->setVisible(true);
        result=false;
    }

    return result;
}



void Add_goods_group::on_backPushButton_clicked()
{
    this->close();
}
