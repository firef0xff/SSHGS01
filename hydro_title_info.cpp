#include "hydro_title_info.h"
#include "ui_hydro_title_info.h"
#include "test_case/tests.h"
#include "test_form.h"



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
    ui->ActuationTime->setValidator( new QIntValidator( 1, 2000, this ) );
}

HydroTitleInfo::~HydroTitleInfo()
{
    delete ui;
}

void HydroTitleInfo::on_buttonBox_accepted()
{
    hide();
    if ( mChildWindow.get() )
        QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
    mChildWindow.reset( new TestForm( test::HydroTests ) );
    QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
    mChildWindow->show();
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
