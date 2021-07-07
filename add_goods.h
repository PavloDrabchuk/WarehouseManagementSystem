#ifndef ADD_GOODS_H
#define ADD_GOODS_H

#include <QDialog>
#include <QVector>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Add_Goods;
}

class Add_Goods : public QDialog
{
    Q_OBJECT

public:
    explicit Add_Goods(QWidget *parent = nullptr);
    ~Add_Goods();

    QString getName();
    QVector<QString> getAddInfo();

    void clearAndUpdateForms();

signals:
    void addGoodToTable();

private slots:

    void on_closeButton_clicked();

    void on_saveButton_clicked();

    void on_groupGoodsComboBox_currentIndexChanged(int index);

private:
    Ui::Add_Goods *ui;
    QVector<QString> addInfo;

    QSqlDatabase db;

    void addProvidersToComboBox();
    void addGroupCategoriesToComboBox();
    void addCategoriesToComboBox(int index);
    bool checkGoodsFields();
};

#endif // ADD_GOODS_H
