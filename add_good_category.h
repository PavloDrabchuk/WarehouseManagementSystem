#ifndef ADD_GOOD_CATEGORY_H
#define ADD_GOOD_CATEGORY_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Add_good_category;
}

class Add_good_category : public QDialog
{
    Q_OBJECT

public:
    explicit Add_good_category(QWidget *parent = nullptr);
    ~Add_good_category();

    void clearAndUpdateForms();

private slots:


    void on_addCategoryPushButton_clicked();

    void on_backPushButton_clicked();

private:
    Ui::Add_good_category *ui;

    bool checkInputForm();

    void addGroupGoodsToComboBox();
};

#endif // ADD_GOOD_CATEGORY_H
