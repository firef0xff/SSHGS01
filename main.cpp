#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include "settings/settings.h"
#include "login.h"
#include "board/custom_control_board.h"
#include "omnikey.h"

int main(int argc, char *argv[])
{
    QStringList paths = QApplication::libraryPaths();
    paths.append( QFileInfo(argv[0]).dir().path() + QStringLiteral( "/plugins" ) );
    QApplication::setLibraryPaths( paths );

    QApplication a(argc, argv);
    a.installEventFilter( &Omnikey::Instance() );
    MainWindow w;
    Login l ( &w );
    w.connect( &l, &Login::on_login, &w, &MainWindow::onLogin );
    l.show();

    auto res = a.exec();
    a.removeEventFilter( &Omnikey::Instance() );
    w.disconnect( &l, &Login::on_login, &w, &MainWindow::onLogin );
    return res;
}
