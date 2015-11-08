#include "mainwindow.h"
#include "login.h"
#include "ui_login.h"
#include <settings/settings.h>

Login::Login(QMainWindow *parent) :
    ui(new Ui::Login),
    mMainWindow( parent )
{
    ui->setupUi(this);
    ui->User->setText( app::Settings::Instance().User() );
}

Login::~Login()
{
    delete ui;
}

void Login::on_buttonBox_rejected()
{
    close();
}

void Login::on_buttonBox_accepted()
{
    app::Settings::Instance().User( ui->User->text() );
    app::Settings::Instance().Save();
    close();
}

void Login::closeEvent(QCloseEvent *e)
{
    if (mMainWindow)
        mMainWindow->showMaximized();

    QWidget::closeEvent( e );
}
