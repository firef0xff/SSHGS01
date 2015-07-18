#include "hydro_title_info.h"
#include "ui_hydro_title_info.h"
#include "test_case/tests.h"


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
    close();
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
