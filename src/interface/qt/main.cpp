#include "MainWindow.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec>
int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    
    app.setOrganizationName("ZLv");
    app.setApplicationName("MassDelivery");
    app.setApplicationVersion("0.9");
    QSettings settings;
    //get locale
    QTranslator translator;
    QString str = QLocale::system().name();
    bool ru;
    //стандартное значение / default value
    if (str == "ru_RU") ru = 1;
    else ru = 0;
    //выбор языка / language selection
    ru = settings.value("/Settings/ru",ru).toBool();
    if (ru)
    {
        if (!translator.load("ru.qm","."))
        {
            translator.load("ru.qm","/usr/local/share/MassDelivery");
        }
    }
    app.installTranslator(&translator);
    qApp->addLibraryPath(qApp->applicationDirPath());
    
    new MainWindow(ru);
    
    return app.exec();
}
