#ifndef REPORTS_H
#define REPORTS_H

#include <QWidget>
#include <QtDebug>
#include <QTextDocument>
#include <QRect>
#include <QPainter>
#include <QTextCursor>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTextDocument>
#include <QMessageBox>
#include <QDesktopServices>

namespace Ui {
class Reports;
}

class Reports : public QWidget
{
    Q_OBJECT

public:
    explicit Reports(QWidget *parent = nullptr);
    ~Reports();

    void setUserId(int id);

    void clearAndUpdateForms();

signals:
    void goToHomePage();

private slots:
    void on_getReportButton_clicked();

    void on_printButton_clicked();

    QString getSqlQueryAcceptanceGoods(QString startDate, QString finishDate);
    QString getSqlQueryDeliveryGoods(QString startDate, QString finishDate);
    QString getSqlQueryWriteOffGoods(QString startDate, QString finishDate);
    QString getSqlQueryDamageGoods(QString startDate, QString finishDate);
    QString getSqlQueryRecycledGoods(QString startDate, QString finishDate);

    void on_goToHomePushButton_clicked();

    void on_openFilePushButton_clicked();

private:
    Ui::Reports *ui;

    QTextCursor m_cursor;
    QTextDocument doc;

    QString createNumberReport(int operation);
    int userId;

    QString filePath;

    QString removeSpaces(QString s);
};

#endif // REPORTS_H
