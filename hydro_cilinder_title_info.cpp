#include "hydro_cilinder_title_info.h"
#include "ui_hydro_cilinder_title_info.h"
#include "test_form.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_hydro_cilinder.h"
#include "settings/settings.h"
#include "functions.h"

HydroCilinderTitleInfo::HydroCilinderTitleInfo(bool new_mode, QWidget *parent) :
    ChildWidget(parent),
    ui(new Ui::HydroCilinderTitleInfo),
    mNewMode( new_mode )
{
    ui->setupUi(this);

    ui->MaxPressure->setValidator( new QIntValidator( 10, 315, this ) );
    ui->MoveTime->setValidator( new QDoubleValidator( 0.1, 10, 3, this ) );
    ui->Expenditure->setValidator( new QIntValidator( 1, 300, this ) );

    if ( !mNewMode )
        FromParams();

    CheckRights();
}

HydroCilinderTitleInfo::~HydroCilinderTitleInfo()
{
    delete ui;
}

bool HydroCilinderTitleInfo::SaveInputParams()
{
    test::hydro_cylinder::Parameters& params = test::hydro_cylinder::Parameters::Instance();
    test::CURRENT_PARAMS = &params;

    if (mNewMode)
        params.Reset();

    bool res = true;

    res *= ParamChecker( ui->l_ser_no,          params.SerNo( ui->SerNo->text() ) );
    res *= ParamChecker( ui->l_def_expenditure, params.DefaultExpenditure( ui->DefExpenditure->text() ) );
    res *= ParamChecker( ui->l_gs_type,         params.GsType( ui->GsType->text() ) );
    res *= ParamChecker( ui->l_max_pressure,    ValidateRange( ui->MaxPressure, params.MaxPressure( ui->MaxPressure->text() ) ) );
    res *= ParamChecker( ui->l_move_time,       ValidateRange( ui->MoveTime, params.MoveTime( ui->MoveTime->text() ) ) );
    res *= ParamChecker( ui->l_test_pressure,   params.TestPressure( QString::number( ui->TestPressure->value() ) ) );
    res *= ParamChecker( ui->l_herm_test_time,  params.HermTestTime( QString::number( ui->HermTestTime->value() ) ) );
    res *= ParamChecker( ui->l_expenditure,     ValidateRange( ui->Expenditure, params.Expenditure( ui->Expenditure->text() ) ) );

    return res;
}
void HydroCilinderTitleInfo::FromParams()
{
    test::hydro_cylinder::Parameters& params = test::hydro_cylinder::Parameters::Instance();

    ui->SerNo->setText( params.SerNo() );
    ui->DefExpenditure->setValue( params.DefaultExpenditure() );
    ui->GsType->setText( params.GsType() );
    ui->MaxPressure->setText( test::ToString( params.MaxPressure() ) );
    ui->MoveTime->setText( test::ToString( params.MoveTime() ) );
    ui->TestPressure->setValue( params.TestPressure() );
    ui->HermTestTime->setValue( params.HermTestTime() );
    ui->Expenditure->setText( test::ToString( params.Expenditure() ) );
}

void HydroCilinderTitleInfo::on_buttonBox_accepted()
{
    auto CheckPower = []() -> bool
    {
        test::hydro_cylinder::Parameters& params = test::hydro_cylinder::Parameters::Instance();

        const int pomp_max_power = 55;
        const int pomp_count = 2;

        double max_expenditure = std::max( params.DefaultExpenditure(), params.Expenditure() );
        double max_pressure = std::max( params.MaxPressure(), params.TestPressure() );


        bool res = max_expenditure*max_pressure/540 <= pomp_count * pomp_max_power;
        if (!res)
        {
            QMessageBox msg;
            msg.setWindowTitle( "Превышена допустимая мощьность насосов" );
            msg.setText( "Необходимо скорректировать параметры расходов и давлений" );
            msg.setStandardButtons( QMessageBox::Ok );
            msg.setModal( true );
            msg.exec();
        }
        return res;
    };
    if ( SaveInputParams() )
    {
        if (!CheckPower())
            return;
        hide();
        if ( mChildWindow.get() )
            QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
        mChildWindow.reset( new TestForm( mNewMode ) );
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

void HydroCilinderTitleInfo::on_buttonBox_rejected()
{
    close();
}

void HydroCilinderTitleInfo::CheckRights()
{
    if ( app::Settings::Instance().UserAccess() == app::User )
    {
        ui->GsType->setEnabled( false );
        ui->SerNo->setEnabled( false );
        ui->DefExpenditure->setEnabled( false );
        ui->GsType->setEnabled( false );
        ui->MaxPressure->setEnabled( false );
        ui->MoveTime->setEnabled( false );
        ui->TestPressure->setEnabled( false );
        ui->HermTestTime->setEnabled( false );
        ui->Expenditure->setEnabled( false );
    }
}
