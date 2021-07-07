#include "reports.h"
#include "ui_reports.h"

Reports::Reports(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reports)
{
    ui->setupUi(this);

    qDebug()<<"******REPORTS*******";

    ui->buttonGroup->setId(ui->acceptRadioButton,0);
    ui->buttonGroup->setId(ui->deliveryRadioButton,1);
    ui->buttonGroup->setId(ui->write_offRadioButton,2);
    ui->buttonGroup->setId(ui->damageGoodsRadioButton,3);
    ui->buttonGroup->setId(ui->recycledGoodsRadioButton,4);

    ui->printButton->setDisabled(true);
    ui->openFilePushButton->setDisabled(true);

    QDate today=QDate::currentDate();
    ui->finishDateEdit->setDate(today);
}

Reports::~Reports()
{
    delete ui;
}

void Reports::setUserId(int id)
{
    userId=id;
}

void Reports::clearAndUpdateForms()
{
    ui->fileNameLineEdit->clear();
    QDate today=QDate::currentDate();
    ui->finishDateEdit->setDate(today);
    QDate prodDate(2000,1,1);
    ui->startDateEdit->setDate(prodDate);
    ui->getReportButton->setText("Отримати");
    ui->openFilePushButton->setText("Відкрити");
    ui->openFilePushButton->setDisabled(true);
    ui->printButton->setDisabled(true);
    ui->printButton->setText("Друкувати");
    ui->messageLabel->setText("");
    QSqlQueryModel *model=new QSqlQueryModel;
    ui->tableView->setModel(model);

    QAbstractButton* checked = ui->buttonGroup->checkedButton();
    if (checked)
    {
        ui->buttonGroup->setExclusive(false);
        checked->setChecked(false);
        ui->buttonGroup->setExclusive(true);
    }
}

void Reports::on_getReportButton_clicked()
{
    QSqlQueryModel *model=new QSqlQueryModel;
    ui->tableView->setModel(model);

    bool isChecked=false;
    bool isEmpty=false;

    QSqlQuery getUserNameAndSurname("SELECT `name`,`surname` FROM `warehouse`.`users` WHERE id='"+QString::number(userId)+"'");
    getUserNameAndSurname.next();
    QString name=getUserNameAndSurname.value(0).toString();
    QString surname=getUserNameAndSurname.value(1).toString();

    QString reportTable="";
    QString startTable="<br/><table  border='1' cellspacing='0' cellpadding='5' width='100%'>";
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);

    QString startDate=ui->startDateEdit->date().toString("yyyy-MM-dd");
    QString finishDate=ui->finishDateEdit->date().toString("yyyy-MM-dd");

    QString fileName="files/reports/";
    QString userFileName;

    switch (ui->buttonGroup->checkedId()) {
    case 0:{
        //accept
        QString numberReport=createNumberReport(1);
        userFileName=ui->fileNameLineEdit->text();
        if(removeSpaces(userFileName)==""){
            userFileName=numberReport;
        }else{
            userFileName=numberReport+"_"+removeSpaces(ui->fileNameLineEdit->text());
        }

        QString topReport="<h2 style='text-align: center;'>Звіт №"+numberReport+"</h2> "
                                                                                " <h3 style='text-align: center;'>Про прийнятий товар</h3>  "
                                                                                " <h3 style='text-align: center;'>За період з "+ui->startDateEdit->date().toString("dd.MM.yyyy")+" по "+ui->finishDateEdit->date().toString("dd.MM.yyyy")+"</h3> "  ;

        QString bottomReport="<br/><br/><table width='90%'>"
                             "  <tr>"
                             "  <td width='50%'>&nbsp;&nbsp;&nbsp;&nbsp; "+QDate::currentDate().toString("dd.MM.yyyy")+" р."+"</td>"
                                                                                                                             "  <td width='50%' align='right'>"+surname+" "+name+"&ensp;___________</td> "
                                                                                                                                                                                 " </tr>"
                                                                                                                                                                                 " </table>";

        fileName+="acceptance_goods/"+userFileName+".pdf";
        printer.setOutputFileName(fileName);

        reportTable+=topReport;
        reportTable+=startTable;

        QSqlQuery query(getSqlQueryAcceptanceGoods(startDate,finishDate));
        if(query.size()==0){
            isEmpty=true;
        }else{

            int num=1;
            int totalCount=0;
            double totalCost=0;

            reportTable+="<tr><th >№</th><th>Назва</th><th>Дата</th><th>Номер партії</th><th>Постачальник</th><th>Кількість</th><th>Вартість</th></tr>";

            while (query.next()) {
                QString name = query.value(0).toString();
                QString surname = query.value(1).toString();

                reportTable+="<tr><td align='center'>"+QString::number(num)+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td>"+query.value(2).toString()+"</td><td>"+query.value(3).toString()+"</td><td align='center'>"+query.value(4).toString()+"</td><td align='center'>"+query.value(5).toString()+"</td></tr>";
                totalCost+=query.value(5).toDouble();
                totalCount+=query.value(4).toDouble();
                num++;
            }
            query.clear();
            reportTable+="<tr><th colspan='5'>Загалом</th><th align='center'>"+QString::number(totalCount)+"</th><th align='center'>"+QString::number(totalCost)+"</th></tr>";
            reportTable+="</table>";

            reportTable+=bottomReport;

            QSqlQuery addInfoReports("INSERT INTO `warehouse`.`reports` (`number`, `operation`,`filename`, `date`) VALUES ('"+numberReport+"', '1','"+fileName+"', now())");

            QSqlQueryModel *model=new QSqlQueryModel;
            model->setQuery(getSqlQueryAcceptanceGoods(startDate,finishDate));

            QVector<std::string> columnNames={"Назва","Дата","Номер партії","Постачальник","Кількість","Вартість"};

            for(int i=0, length=columnNames.length();i<length;i++){
                model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
            }

            ui->tableView->setModel(model);
        }
        break;
    }
    case 1:{
        //delivery
        QString numberReport=createNumberReport(2);
        userFileName=ui->fileNameLineEdit->text();
        if(removeSpaces(userFileName)==""){
            userFileName=numberReport;
        }else{
            userFileName=numberReport+"_"+removeSpaces(ui->fileNameLineEdit->text());
        }

        QString topReport="<h2 style='text-align: center;'>Звіт №"+numberReport+"</h2> "
                                                                                " <h3 style='text-align: center;'>Про виданий товар</h3>  "
                                                                                " <h3 style='text-align: center;'>За період з "+ui->startDateEdit->date().toString("dd.MM.yyyy")+" по "+ui->finishDateEdit->date().toString("dd.MM.yyyy")+"</h3> "  ;

        QString bottomReport="<br/><br/><table width='90%'>"
                             "  <tr>"
                             "  <td width='50%'>&nbsp;&nbsp;&nbsp;&nbsp; "+QDate::currentDate().toString("dd.MM.yyyy")+" р."+"</td>"
                                                                                                                             "  <td width='50%' align='right'>"+surname+" "+name+"&ensp;___________</td> "
                                                                                                                                                                                 " </tr>"
                                                                                                                                                                                 " </table>";

        fileName+="delivery_goods/"+userFileName+".pdf";
        printer.setOutputFileName(fileName);

        reportTable+=topReport;
        reportTable+=startTable;

        QSqlQuery query(getSqlQueryDeliveryGoods(startDate,finishDate));
        if(query.size()==0){
            isEmpty=true;
        }else{

            int num=1;
            int totalCount=0;

            reportTable+="<tr><th >№</th><th>Номер партії</th><th>Категорія</th><th>Назва</th><th>Дата</th><th>Кількість</th></tr>";

            while (query.next()) {
                QString name = query.value(0).toString();
                QString surname = query.value(1).toString();

                reportTable+="<tr><td align='center'>"+QString::number(num)+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td>"+query.value(2).toString()+"</td><td>"+query.value(3).toString()+"</td><td align='center'>"+query.value(4).toString()+"</td></tr>";

                totalCount+=query.value(4).toInt();
                num++;
            }
            query.clear();
            reportTable+="<tr><th colspan='5'>Загалом</th><th align='center'>"+QString::number(totalCount)+"</th></tr>";
            reportTable+="</table>";

            reportTable+=bottomReport;

            QSqlQuery addInfoReports("INSERT INTO `warehouse`.`reports` (`number`, `operation`,`filename`, `date`) VALUES ('"+numberReport+"', '2','"+fileName+"', now())");

            QSqlQueryModel *model=new QSqlQueryModel;

            model->setQuery(getSqlQueryDeliveryGoods(startDate,finishDate));

            QVector<std::string> columnNames={"Номер партії","Категорія","Назва","Дата","Кількість"};

            for(int i=0, length=columnNames.length();i<length;i++){
                model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
            }

            ui->tableView->setModel(model);
        }
        break;
    }
    case 2:{
        //write-off
        QString numberReport=createNumberReport(3);
        userFileName=ui->fileNameLineEdit->text();
        if(removeSpaces(userFileName)==""){
            userFileName=numberReport;
        }else{
            userFileName=numberReport+"_"+removeSpaces(ui->fileNameLineEdit->text());
        }

        QString topReport="<h2 style='text-align: center;'>Звіт №"+numberReport+"</h2> "
                                                                                " <h3 style='text-align: center;'>Про списаний протермінований товар</h3>  "
                                                                                " <h3 style='text-align: center;'>За період з "+ui->startDateEdit->date().toString("dd.MM.yyyy")+" по "+ui->finishDateEdit->date().toString("dd.MM.yyyy")+"</h3> "  ;

        QString bottomReport="<br/><br/><table width='90%'>"
                             "  <tr>"
                             "  <td width='50%'>&nbsp;&nbsp;&nbsp;&nbsp; "+QDate::currentDate().toString("dd.MM.yyyy")+" р."+"</td>"
                                                                                                                             "  <td width='50%' align='right'>"+surname+" "+name+"&ensp;___________</td> "
                                                                                                                                                                                 " </tr>"
                                                                                                                                                                                 " </table>";

        fileName+="write-off_goods/"+userFileName+".pdf";
        printer.setOutputFileName(fileName);

        reportTable+=topReport;
        reportTable+=startTable;

        QSqlQuery query(getSqlQueryWriteOffGoods(startDate,finishDate));
        if(query.size()==0){
            isEmpty=true;
        }else{

            int num=1;
            int totalCount=0;

            reportTable+="<tr><th >№</th><th>Номер партії</th><th>Категорія</th><th>Назва</th><th>К-сть</th><th>Дата<br/>виробництва</th><th>Термін пр-сті<br/>(міс.)</th></tr>";

            while (query.next()) {
                QString name = query.value(0).toString();
                QString surname = query.value(1).toString();

                reportTable+="<tr><td align='center'>"+QString::number(num)+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td>"+query.value(2).toString()+"</td><td  align='center'>"+query.value(3).toString()+"</td><td>"+query.value(4).toString()+"</td><td  align='center'>"+query.value(5).toString()+"</td></tr>";

                totalCount+=query.value(3).toInt();
                num++;
            }
            query.clear();
            reportTable+="<tr><th colspan='4'>Загалом</th><th align='center'>"+QString::number(totalCount)+"</th><th colspan='2'>-</th></tr>";
            reportTable+="</table>";

            reportTable+=bottomReport;

            QSqlQuery addInfoReports("INSERT INTO `warehouse`.`reports` (`number`, `operation`,`filename`, `date`) VALUES ('"+numberReport+"', '3','"+fileName+"', now())");

            QSqlQueryModel *model=new QSqlQueryModel;

            model->setQuery(getSqlQueryWriteOffGoods(startDate,finishDate));

            QVector<std::string> columnNames={"Номер партії","Категорія","Назва","Кількість","Дата виробництва","Термін придатності\n(міс.)","Дата списування"};

            for(int i=0, length=columnNames.length();i<length;i++){
                model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
            }
            ui->tableView->setModel(model);
        }
        break;
    }
    case 3:{
        //damage
        QString numberReport=createNumberReport(4);
        userFileName=ui->fileNameLineEdit->text();
        if(removeSpaces(userFileName)==""){
            userFileName=numberReport;
        }else{
            userFileName=numberReport+"_"+removeSpaces(ui->fileNameLineEdit->text());
        }

        QString topReport="<h2 style='text-align: center;'>Звіт №"+numberReport+"</h2> "
                                                                                " <h3 style='text-align: center;'>Про пошкоджений товар</h3>  "
                                                                                " <h3 style='text-align: center;'>За період з "+ui->startDateEdit->date().toString("dd.MM.yyyy")+" по "+ui->finishDateEdit->date().toString("dd.MM.yyyy")+"</h3> "  ;

        QString bottomReport="<br/><br/><table width='90%'>"
                             "  <tr>"
                             "  <td width='50%'>&nbsp;&nbsp;&nbsp;&nbsp; "+QDate::currentDate().toString("dd.MM.yyyy")+" р."+"</td>"
                                                                                                                             "  <td width='50%' align='right'>"+surname+" "+name+"&ensp;___________</td> "
                                                                                                                                                                                 " </tr>"
                                                                                                                                                                                 " </table>";

        fileName+="damaged_goods/"+userFileName+".pdf";
        printer.setOutputFileName(fileName);

        reportTable+=topReport;
        reportTable+=startTable;

        QSqlQuery query(getSqlQueryDamageGoods(startDate,finishDate));
        if(query.size()==0){
            isEmpty=true;
        }else{

            int num=1;
            int totalCount=0;

            reportTable+="<tr><th >№</th><th>Номер партії</th><th>Назва</th><th>К-сть</th><th>Винуватець</th><th>Дата пошкодження</th></tr>";

            while (query.next()) {
                QString name = query.value(0).toString();
                QString surname = query.value(1).toString();

                reportTable+="<tr><td align='center'>"+QString::number(num)+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td align='center'>"+query.value(2).toString()+"</td><td  >"+query.value(3).toString()+"</td><td>"+query.value(4).toString()+"</td></tr>";

                totalCount+=query.value(2).toInt();
                num++;
            }
            query.clear();
            reportTable+="<tr><th colspan='3'>Загалом</th><th align='center'>"+QString::number(totalCount)+"</th><th colspan='2'>-</th></tr>";
            reportTable+="</table>";

            reportTable+=bottomReport;

            QSqlQuery addInfoReports("INSERT INTO `warehouse`.`reports` (`number`, `operation`,`filename`, `date`) VALUES ('"+numberReport+"', '4','"+fileName+"', now())");

            QSqlQueryModel *model=new QSqlQueryModel;

            model->setQuery(getSqlQueryDamageGoods(startDate,finishDate));

            QVector<std::string> columnNames={"Номер партії","Назва","Кількість","Винуватець","Дата пошкодження"};

            for(int i=0, length=columnNames.length();i<length;i++){
                model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
            }

            ui->tableView->setModel(model);
        }
        break;
    }
    case 4:{
        //recycled

        QString numberReport=createNumberReport(5);
        userFileName=ui->fileNameLineEdit->text();
        if(removeSpaces(userFileName)==""){
            userFileName=numberReport;
        }else{
            userFileName=numberReport+"_"+removeSpaces(ui->fileNameLineEdit->text());
        }

        QString topReport="<h2 style='text-align: center;'>Звіт №"+numberReport+"</h2> "
                                                                                " <h3 style='text-align: center;'>Про утилізований товар</h3>  "
                                                                                " <h3 style='text-align: center;'>За період з "+ui->startDateEdit->date().toString("dd.MM.yyyy")+" по "+ui->finishDateEdit->date().toString("dd.MM.yyyy")+"</h3> "  ;

        QString bottomReport="<br/><br/><table width='90%'>"
                             "  <tr>"
                             "  <td width='50%'>&nbsp;&nbsp;&nbsp;&nbsp; "+QDate::currentDate().toString("dd.MM.yyyy")+" р."+"</td>"
                                                                                                                             "  <td width='50%' align='right'>"+surname+" "+name+"&ensp;___________</td> "
                                                                                                                                                                                 " </tr>"
                                                                                                                                                                                 " </table>";

        fileName+="recycled_goods/"+userFileName+".pdf";
        printer.setOutputFileName(fileName);

        reportTable+=topReport;
        reportTable+=startTable;

        QSqlQuery query(getSqlQueryRecycledGoods(startDate,finishDate));

        if(query.size()==0){
            isEmpty=true;
        }else{

            int num=1;
            int totalCount=0;

            reportTable+="<tr><th >№</th><th>Номер партії</th><th>Назва</th><th>К-сть</th><th>Винуватець</th><th>Дата пошкодження</th></tr>";

            while (query.next()) {
                QString name = query.value(0).toString();
                QString surname = query.value(1).toString();

                reportTable+="<tr><td align='center'>"+QString::number(num)+"</td><td>"+query.value(0).toString()+"</td><td>"+query.value(1).toString()+"</td><td align='center'>"+query.value(2).toString()+"</td><td  >"+query.value(3).toString()+"</td><td>"+query.value(4).toString()+"</td></tr>";

                totalCount+=query.value(2).toInt();
                num++;
            }
            query.clear();
            reportTable+="<tr><th colspan='3'>Загалом</th><th align='center'>"+QString::number(totalCount)+"</th><th colspan='2'>-</th></tr>";
            reportTable+="</table>";

            reportTable+=bottomReport;

            QSqlQuery addInfoReports("INSERT INTO `warehouse`.`reports` (`number`, `operation`,`filename`, `date`) VALUES ('"+numberReport+"', '5','"+fileName+"', now())");

            QSqlQueryModel *model=new QSqlQueryModel;

            model->setQuery(getSqlQueryRecycledGoods(startDate,finishDate));

            QVector<std::string> columnNames={"Номер партії","Назва","Кількість","Винуватець","Дата пошкодження"};

            for(int i=0, length=columnNames.length();i<length;i++){
                model->setHeaderData(i, Qt::Horizontal, QObject::tr(columnNames.at(i).c_str()));
            }

            ui->tableView->setModel(model);
        }
        break;
    }default:{
        isChecked=true;
        break;
    }
    }
    if(!isChecked && !isEmpty){
        ui->printButton->setDisabled(false);
        ui->openFilePushButton->setDisabled(false);
        doc.setHtml(reportTable);
        doc.print(&printer);
        filePath=fileName;
        ui->tableView->setVisible(true);
        ui->messageLabel->setText("Звіт №"+userFileName+" сформовано у форматі .pdf та доступний за розташуванням: "+fileName+"");
        ui->printButton->setText("Роздрукувати "+userFileName+".pdf");
        ui->openFilePushButton->setText("Відкрити "+userFileName+".pdf");
        QMessageBox::about(this,"Інформація про створення звіту","Звіт успішно створено.");
    }else if(isEmpty){
        QMessageBox::about(this,"Інформація про створення звіту","Інформація за цей період відсутня.");
    } else if (isChecked){
        QMessageBox::warning(this,"Інформація про створення звіту","Оберіть вид операції.");
    }else{
        QMessageBox::critical(this,"Інформація про створення звіту", "Помилка створення звіту.");
    }
}

void Reports::on_printButton_clicked()
{
    QPrinter printer;

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        doc.print(&printer);
    }
}

QString Reports::getSqlQueryAcceptanceGoods(QString startDate, QString finishDate)
{
    return "SELECT acceptance_goods_history.name,acceptance_goods_history.date,acceptance_goods_history.number,provider.name,acceptance_goods_history.count,acceptance_goods_history.cost "
           "FROM `warehouse`.`acceptance_goods_history` "
           "LEFT JOIN warehouse.provider ON acceptance_goods_history.provider_id=provider.id "
           "WHERE `date`>='"+startDate+"' AND `date`<='"+finishDate+" 23:59:59'";
}

QString Reports::getSqlQueryDeliveryGoods(QString startDate, QString finishDate)
{
    return "SELECT delivery_goods_history.batch_code,goods_categories.name,delivery_goods_history.name,delivery_goods_history.date,delivery_goods_history.count "
           " FROM warehouse.delivery_goods_history"
           " LEFT JOIN warehouse.goods_categories ON delivery_goods_history.category_id=goods_categories.id "
           " WHERE  `date`>='"+startDate+"' AND `date`<='"+finishDate+" 23:59:59'";
}

QString Reports::getSqlQueryWriteOffGoods(QString startDate, QString finishDate)
{
    return "SELECT expired_goods.batch_code,goods_categories.name,expired_goods.goods_name,expired_goods.count,expired_goods.production_date,expired_goods.expiration_date,expired_goods.write_off_date "
           "  FROM warehouse.expired_goods "
           "  LEFT JOIN warehouse.goods_categories ON expired_goods.category_id=goods_categories.id "
           " WHERE  `write_off_date`>='"+startDate+"' AND `write_off_date`<='"+finishDate+" 23:59:59'";
}

QString Reports::getSqlQueryDamageGoods(QString startDate, QString finishDate)
{
    return "SELECT damaged_goods.batch_code,damaged_goods.name_goods,damaged_goods.count,damaged_goods.pest,damaged_goods.date_of_damage "
           " FROM warehouse.damaged_goods  "
           " WHERE  `registration_date`>='"+startDate+"' AND `registration_date`<='"+finishDate+" 23:59:59' AND `status`='Не утилізований' ";
}

QString Reports::getSqlQueryRecycledGoods(QString startDate, QString finishDate)
{
    return "SELECT damaged_goods.batch_code,damaged_goods.name_goods,damaged_goods.count,damaged_goods.pest,damaged_goods.date_of_damage "
           " FROM warehouse.damaged_goods  "
           " WHERE  `registration_date`>='"+startDate+"' AND `registration_date`<='"+finishDate+" 23:59:59' AND `status`='Утилізований' ";
}

QString Reports::createNumberReport(int operation)
{
    QString num=QString::number(operation);
    QSqlQuery getCountQuery("SELECT Count(*) FROM `warehouse`.`reports` WHERE `operation`='"+QString::number(operation)+"'");
    getCountQuery.next();
    int n=getCountQuery.value(0).toInt()+1,k=1,nCopy;
    nCopy=n;

    while ((n/=10) > 0) k++;
    for(int i=0;i<5-k;i++){
        num+="0";
    }
    num+=QString::number(nCopy);
    return num;
}

QString Reports::removeSpaces(QString s)
{
    QString result="";
    for(auto s1 :s){
        if(s1!=' ') {
            result+=s1;
        }
    }
    return result;
}

void Reports::on_goToHomePushButton_clicked()
{
    emit goToHomePage();
}

void Reports::on_openFilePushButton_clicked()
{
    QUrl url(QDir::currentPath()+"/"+filePath);
    qDebug()<<"send email"<<QDir::currentPath()+"/"+filePath;
    QDesktopServices::openUrl(url);
}
