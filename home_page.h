#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <connection.h>

namespace Ui {
class Home_Page;
}

class Home_Page : public QWidget
{
    Q_OBJECT

public:
    explicit Home_Page(QWidget *parent = nullptr);
    ~Home_Page();

private slots:
    void on_loginButton_clicked();

private:
    Ui::Home_Page *ui;
    Connection connection;
    QSqlDatabase db;
};

#endif // HOME_PAGE_H
