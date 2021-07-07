#ifndef SEARCH_WINDOW_H
#define SEARCH_WINDOW_H

#include <QWidget>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMessageBox>

namespace Ui {
class Search_Window;
}

class Search_Window : public QWidget
{
    Q_OBJECT

public:
    explicit Search_Window(QWidget *parent = nullptr);
    ~Search_Window();



    void clearAndUpdateForms();

signals:
    void goToHomePage();

private slots:
    void on_backToMainMenuWindow_clicked();

    void on_advancedSearchButton_clicked();

    void on_searchButton_clicked();

    void on_saveButton_clicked();

    void on_editButton_clicked();

private:
    Ui::Search_Window *ui;
    QSqlDatabase db;
    QSqlTableModel *model;

    bool isEdited=false;

    void visibleAdvancedSearchElements(bool s);

    void addCategoriesToComboBox();
    void addProducersToComboBox();
    void addProvidersToComboBox();
    void addCountryProducersToComboBox();
    void addDescriptionStatusToComboBox();

    QString createQueryOrFilterWithParameters();
};

#endif // SEARCH_WINDOW_H
