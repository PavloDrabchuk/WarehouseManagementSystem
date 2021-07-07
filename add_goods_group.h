#ifndef ADD_GOODS_GROUP_H
#define ADD_GOODS_GROUP_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class Add_goods_group;
}

class Add_goods_group : public QDialog
{
    Q_OBJECT

public:
    explicit Add_goods_group(QWidget *parent = nullptr);
    ~Add_goods_group();

    void clearAndUpdateForms();

private slots:
    void on_addGroupCategoryPushButton_clicked();

    void on_backPushButton_clicked();

private:
    Ui::Add_goods_group *ui;

    bool checkInputForm();



};

#endif // ADD_GOODS_GROUP_H
