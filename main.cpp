#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include "settings/settings.h"
#include "login.h"
#include "board/custom_control_board.h"


int main(int argc, char *argv[])
{
    QStringList paths = QApplication::libraryPaths();
    paths.append( QFileInfo(argv[0]).dir().path() + QStringLiteral( "/plugins" ) );
    QApplication::setLibraryPaths( paths );

    QApplication a(argc, argv);

    MainWindow w;
    Login l ( &w );
    l.show();

    return a.exec();
}
