#include "hydro_title_info.h"
#include "ui_hydro_title_info.h"
#include "test_case/tests.h"
#include "test_form.h"
#include <QMessageBox>


HydroTitleInfo::HydroTitleInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HydroTitleInfo)
{
    ui->setupUi(this);

    foreach ( const examinee::Device* device, test::HydroTests.Devices().Get())
    {
        ui->GsType->addItem( device->Name() + " " + device->Manufacturer(), QVariant::fromValue( device ) );
    }
    ui->GsType->setCurrentIndex( -1 );
    ui->ActuationOnTime->setValidator( new QIntValidator( 1, 2000, this ) );
    ui->ActuationOffTime->setValidator( new QIntValidator( 1, 2000, this ) );
    ui->MaxExpenditure->setValidator( new QIntValidator( 10, 1100, this ) );
    ui->MaxWorkPressure->setValidator( new QIntValidator( 10, 400, this ) );

    on_ControlType_activated( ui->ControlType->currentIndex() );
}

HydroTitleInfo::~HydroTitleInfo()
{
    delete ui;
}

bool HydroTitleInfo::SaveInputParams()
{
    return false;
}

void HydroTitleInfo::on_buttonBox_accepted()
{
    if ( SaveInputParams() )
    {
        hide();
        if ( mChildWindow.get() )
            QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
        mChildWindow.reset( new TestForm( test::HydroTests ) );
        QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
        mChildWindow->show();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle( "Не заполнены обязательные поля" );
        msg.setText( "Заполните все поля и повторите попытку" );
        msg.setStandardButtons( QMessageBox::Ok );
        msg.setModal( true );
        msg.exec();
    }
}

void HydroTitleInfo::on_buttonBox_rejected()
{
    close();
}

void HydroTitleInfo::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}

void HydroTitleInfo::on_ControlType_activated(int index)
{
    auto set_control_pressure_visible = [ this ]( bool visible )
    {
        ui->l_min_control_pressure->setVisible( visible );
        ui->l_max_control_pressure->setVisible( visible );

        ui->MinControlPressure->setVisible( visible );
        ui->MaxControlPressure->setVisible( visible );
    };

    switch (index)
    {
    case 0:
    {
        set_control_pressure_visible( false );
        break;
    }
    case 1:
    {
        set_control_pressure_visible( true );
        break;
    }
    default:
        set_control_pressure_visible( false );
        break;
    }
}
