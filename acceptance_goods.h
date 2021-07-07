#ifndef ACCEPTANCE_GOODS_H
#define ACCEPTANCE_GOODS_H

#include <QWidget>
#include <QtDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDateTime>

#include <add_goods.h>
#include <add_goods_group.h>
#include <add_good_category.h>
#include <add_provider.h>

namespace Ui {
class Acceptance_Goods;
}

class Acceptance_Goods : public QWidget
{
    Q_OBJECT

public:
    explicit Acceptance_Goods(QWidget *parent = nullptr);
    ~Acceptance_Goods();

    void initTable();

    void clearAndUpdateForms();

    int getIsLoaded();

signals:
    void goToHomePage();
    void updateWriteOffGoodsPushButton();

private slots:
    void on_addGoodsButton_clicked();
    void addGoods();
    void on_goToHomePushButton_clicked();

    void on_addGoodsGroupPushButton_clicked();

    void on_addGoodsCategoriesPushButton_clicked();

    void on_addProviderPushButton_clicked();

private:
    Ui::Acceptance_Goods *ui;

    QSqlDatabase db;

    Add_Goods *addGoodsWindows;
    Add_goods_group *addGoodsGroup;
    Add_good_category *addGoodCategory;
    Add_Provider *addProvider;

    int isLoaded=0;
};

#endif // ACCEPTANCE_GOODS_H
