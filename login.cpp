#include "mainwindow.h"
#include "login.h"
#include "ui_login.h"
#include <settings/settings.h>
#include <QMessageBox>

Login::Login(QMainWindow *parent) :
    ui(new Ui::Login),
    mMainWindow( parent )
{
    ui->setupUi(this);
    ui->User->setText( app::Settings::Instance().User() );
    ui->Pass->setEchoMode(QLineEdit::Password);
    ui->Pass->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
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
    accept = app::Settings::Instance().CheckUser( ui->User->text(), ui->Pass->text() );
    if ( accept )
    {
        app::Settings::Instance().User( ui->User->text() );
        app::Settings::Instance().Save();
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle( "Доспут запрещен" );
        msg.setText( "Не вырное имя пользователя и/или пароль" );
        msg.setStandardButtons( QMessageBox::Ok );
        msg.setModal( true );
        msg.exec();
        if ( repats )
        {
            //msg
            --repats;
            show();
        }
        else
        {
            close();
        }
    }
}

void Login::closeEvent(QCloseEvent *e)
{
    if (mMainWindow && accept )
        mMainWindow->showMaximized();

    QWidget::closeEvent( e );
}
