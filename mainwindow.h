#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QtDebug>
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDateTime>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QObject>

#include <search_window.h>
#include <acceptance_goods.h>
#include <delivery_goods.h>
#include <write_off_goods.h>
#include <reports.h>
#include <admin_page.h>
#include <damage_goods_page.h>
#include <send_mails.h>
#include <connection.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_searchButton_clicked();

    void on_homeButton_clicked();

    void on_acceptanceGoodsButton_clicked();

    void on_deliveryGoodsButton_clicked();

    void on_writeOffGoodsButton_clicked();

    void on_reportsButton_clicked();

    void on_loginButton_clicked();

    void on_exitButton_clicked();

    void moveHomePage();

    void on_adminButton_clicked();

    void createTextWriteOffPushButton();

    void on_damageGoodsButton_clicked();

    void on_sendMailsPushButton_clicked();

    void on_exitAction_triggered();

    void on_homeAction_triggered();

    void on_infoGoodsAction_triggered();

    void on_aboutAction_triggered();

    void on_adminInfoAction_triggered();

private:
    Ui::MainWindow *ui;

    QString getHashPassword(QString pass);

    Search_Window *searchWindowPage;

    Acceptance_Goods *acceptanceGoodsPage;
    Delivery_Goods *deliveryGoodsPage;
    Write_Off_Goods *writeOffGoodsPage;

    Reports *reportsPage;

    Admin_page *adminPage;

    Damage_Goods_Page *damageGoodsPage;

    Send_mails *sendMailsPage;

    void visibleLoginElements(bool s);
    void visibleMainMenuPage(bool s);

    void undisableAllButtons();

    void visibleAdminMainMenuButtons(bool s);
    void visibleUserMainMenuButtons(bool s);
    void visibleEditorMainMenuButtons(bool s);

    void accessToPages(QString role, int userId);

    void updateLoadingBar(int availableValue, int maxValue);

    void createInfoAboutGoodsText();

    int accessDenied=0;

    QString pH="1090e1d59a8772b2701a3b760f1e6842acfa78afdd82e19587e4853ee628d79b6e1c468601dd4b9805c6b8b9b07037610f73757fff30886450beaa25845063ce";
    bool logon=false;

    QString infoAboutGoodsText;

    Connection connection;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
