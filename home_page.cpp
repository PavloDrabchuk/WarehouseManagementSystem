#include "home_page.h"
#include "ui_home_page.h"


Home_Page::Home_Page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_Page)
{
    ui->setupUi(this);

    qDebug()<<"******HOME*******";

}

Home_Page::~Home_Page()
{
    delete ui;
}


