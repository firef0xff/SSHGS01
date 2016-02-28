#include "settings_wnd.h"
#include "ui_settings_wnd.h"
#include "settings.h"
#include <QFileDialog>

settings_wnd::settings_wnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_wnd)
{
    ui->setupUi(this);

    auto index = ui->LogLevel->findText( app::Settings::Instance().LogLevel() );
    if ( index > 0 )
        ui->LogLevel->setCurrentIndex( index );
    else
        ui->LogLevel->setCurrentIndex( 0 );

    ui->TestingParams->setText( app::Settings::Instance().TestPath() );
    ui->TestingResults->setText( app::Settings::Instance().ResultPath() );
    ui->MainPump->setCurrentIndex( ui->MainPump->findText( app::Settings::Instance().MainPupm() ) );
    ui->ComAddr->setText( app::Settings::Instance().ComAddr() );
}

settings_wnd::~settings_wnd()
{
    delete ui;
}

void settings_wnd::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}

QString settings_wnd::ChooseDir( const QString &place )
{
    QString file_name;
    QFileDialog dlg;
    dlg.setFileMode( QFileDialog::DirectoryOnly );
    dlg.setViewMode( QFileDialog::Detail );
    if ( !place.isEmpty() )
        dlg.setDirectory( place );
    if ( dlg.exec() )
        file_name = dlg.selectedFiles().front();
    return file_name;
}

void settings_wnd::on_TestParamsButton_clicked()
{
    QString dir = ChooseDir( ui->TestingParams->text() );
    if ( !dir.isEmpty() )
        ui->TestingParams->setText( dir );
}

void settings_wnd::on_toolButton_2_clicked()
{
    QString dir = ChooseDir( ui->TestingParams->text() );
    if ( !dir.isEmpty() )
        ui->TestingResults->setText( dir );
}

void settings_wnd::on_buttonBox_rejected()
{
    close();
}

void settings_wnd::on_buttonBox_accepted()
{
    app::Settings::Instance().LogLevel( ui->LogLevel->currentText() );
    app::Settings::Instance().TestPath( ui->TestingParams->text() );
    app::Settings::Instance().ResultPath( ui->TestingResults->text() );
    app::Settings::Instance().MainPupm( ui->MainPump->currentText() );
    app::Settings::Instance().ComAddr( ui->ComAddr->text() );
    app::Settings::Instance().Save();
    close();
}
