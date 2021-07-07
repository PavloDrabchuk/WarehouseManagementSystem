#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>


int main(int argc, char *argv[])
{
    qDebug()<<"******MAIN*******";

    QApplication a(argc, argv);
    QIcon appIcon=QIcon("images/logo_warehouse.png");

    QTranslator qtTranslator;
        if (qtTranslator.load(QLocale::system(),
                    "qt", "_",
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            a.installTranslator(&qtTranslator);
        }

        QTranslator qtBaseTranslator;
        if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            a.installTranslator(&qtBaseTranslator);
        }

    MainWindow w;
    w.setWindowIcon(appIcon);
    w.setWindowTitle("Облік складу гіпермаркету");
    w.show();
    return a.exec();
}
