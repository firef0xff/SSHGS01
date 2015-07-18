#include "title_info.h"
#include "ui_title_info.h"
#include "defines/examinee.h"
#include "test_case/tests.h"

Q_DECLARE_METATYPE(const examinee::Device*)

TitleInfo::TitleInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TitleInfo)
{
    ui->setupUi(this);

    foreach (examinee::Device const& device, examinee::DeviceCollection::Devices())
    {
        ui->GsType->addItem( device.Name() + " " + device.Manufacturer(), QVariant::fromValue( &device ) );
    }
    ui->GsType->setCurrentIndex( -1 );
    ui->ExecuteTime->setValidator( new QIntValidator( 1, 2000, this ) );
}

TitleInfo::~TitleInfo()
{
    delete ui;
}
