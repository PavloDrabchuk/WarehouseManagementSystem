#ifndef DELIVERY_GOODS_H
#define DELIVERY_GOODS_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QDateTime>
#include <QMessageBox>

namespace Ui {
class Delivery_Goods;
}

class Delivery_Goods : public QWidget
{
    Q_OBJECT

public:
    explicit Delivery_Goods(QWidget *parent = nullptr);
    ~Delivery_Goods();

    void clearAndUpdateForms();

signals:
    void goToHomePage();
    void updateWriteOffGoodsPushButton();

private slots:
    void on_sendButton_clicked();

    void on_goToHomeButton_clicked();

    void on_groupGoodsComboBox_currentIndexChanged(int index);

    void on_categoryGoodsComboBox_currentIndexChanged(int index);

    void on_nameGoodsComboBox_currentTextChanged(const QString &arg1);

    void on_batchCodeComboBox_currentTextChanged(const QString &arg1);

private:
    Ui::Delivery_Goods *ui;

    void addGroupCategoriesToComboBox();
    void addCategoriesToComboBox(int index);
    void addGoodsToComboBox(int index);
    void addBatchCodeToComboBox(QString goodsName);

    bool checkCountGoods();
    bool checkGoodsAndForm();
    int getCountGoods(QString name);
};

#endif // DELIVERY_GOODS_H
