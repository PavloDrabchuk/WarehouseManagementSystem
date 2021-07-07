#ifndef WRITE_OFF_GOODS_H
#define WRITE_OFF_GOODS_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Write_Off_Goods;
}

class Write_Off_Goods : public QWidget
{
    Q_OBJECT

public:
    explicit Write_Off_Goods(QWidget *parent = nullptr);
    ~Write_Off_Goods();

    void clearAndUpdateForms();

signals:
    void updateWriteOffGoodsPushButton();

    void goToHomePage();

private slots:
    void on_getExpiredGoodsButton_clicked();

    void on_getDamagedGoodsButton_clicked();

    void on_goToHomePageButton_clicked();

    void on_write_offGoodsButton_clicked();

    void on_deleteDamageGoodsPushButton_clicked();

private:
    Ui::Write_Off_Goods *ui;

    void visibleExpiredElements(bool status);
    void visibleDamageElements(bool status);
};

#endif // WRITE_OFF_GOODS_H
