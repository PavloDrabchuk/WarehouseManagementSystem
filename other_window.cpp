#include "other_window.h"
#include "ui_other_window.h"
#include <QDebug>

Other_window::Other_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Other_window)
{
    ui->setupUi(this);

    qDebug()<<"******OTHER WINDOW*******";
}

Other_window::~Other_window()
{
    delete ui;
}

void Other_window::on_pushButton_clicked()
{
    this->close();
    emit mainWindow();
}
