#include "mainwindow.h"
#include "login.h"
#include "ui_login.h"
#include <settings/settings.h>
#include <QMessageBox>
#include "omnikey.h"

Login::Login(QMainWindow *parent) :
    ui(new Ui::Login),
    mMainWindow( parent )
{
    ui->setupUi(this);
    ui->User->setText( app::Settings::Instance().User() );
    ui->Pass->setEchoMode(QLineEdit::Password);
    ui->Pass->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);

    connect( &Omnikey::Instance(), &Omnikey::card_in, this, &Login::on_card_in, Qt::QueuedConnection );
    connect( &Omnikey::Instance(), &Omnikey::card_out, this, &Login::on_card_out, Qt::QueuedConnection );
}

Login::~Login()
{
   disconnect( &Omnikey::Instance(), &Omnikey::card_in, this, &Login::on_card_in );
   connect( &Omnikey::Instance(), &Omnikey::card_out, this, &Login::on_card_out);
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
        close();
        emit on_login();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle( "Доспут запрещен" );
        msg.setText( "Не верное имя пользователя и/или пароль" );
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
            emit on_login();
        }
    }
}

void Login::on_card_in( QString const& str )
{
   accept = app::Settings::Instance().CheckUser( str );
   if ( accept )
   {
      close();
   }
   emit on_login();
}
void Login::on_card_out()
{
   on_card_in( "" );
}

void Login::closeEvent(QCloseEvent *e)
{
    if (mMainWindow && accept )
        mMainWindow->showMaximized();

    QWidget::closeEvent( e );
}
