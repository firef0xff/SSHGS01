#include "mainwindow.h"
#include "hydro_title_info.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include "settings/settings.h"


int main(int argc, char *argv[])
{
    QStringList paths = QApplication::libraryPaths();
    paths.append( QFileInfo(argv[0]).dir().path() + QStringLiteral( "/plugins" ) );
    QApplication::setLibraryPaths( paths );

    QApplication a(argc, argv);

    app::Settings::Instance().Load();

    MainWindow w;
    w.show();

    return a.exec();
}
