#ifndef DAMAGE_GOODS_PAGE_H
#define DAMAGE_GOODS_PAGE_H

#include <QWidget>
#include <QtDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>

namespace Ui {
class Damage_Goods_Page;
}

class Damage_Goods_Page : public QWidget
{
    Q_OBJECT

public:
    explicit Damage_Goods_Page(QWidget *parent = nullptr);
    ~Damage_Goods_Page();

    void setUserId(int id);

    void clearAndUpdateForms();

signals:
    void goToHomePage();
    void updateWriteOffPage();

private slots:
    void on_addInfoAboutDamageGoodsPushButton_clicked();

    void on_categoryGoodsComboBox_currentIndexChanged(int index);

    void on_nameGoodsComboBox_currentTextChanged(const QString &arg1);

    void on_goToHomePushButton_clicked();

private:
    Ui::Damage_Goods_Page *ui;

    void addCategoriesToComboBox();
    void addNameGoodsToComboBox(int index);
    void addBatchCodesToComboBox(QString index);
    void addPestToComboBox();

    QString getIdGoods(QString codeBatch);

    int userId;

    bool checkCountGoods();
    bool checkInputForm();
};

#endif // DAMAGE_GOODS_PAGE_H
