#include "hydro_title_info.h"
#include "ui_hydro_title_info.h"
#include "stand_params.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_hydro.h"
#include "settings/settings.h"
#include "test_case/test.h"
#include "functions.h"

HydroTitleInfo::HydroTitleInfo(bool new_mode, QWidget *parent) :
    ChildWidget(parent),
    ui(new Ui::HydroTitleInfo),
    mNewMode( new_mode )
{
    ui->setupUi(this);

//    foreach ( examinee::Device const* device, test::hydro::Parameters::Instance().TestCollection().Devices().Get() )
//    {
//        ui->GsType->addItem( device->Name() + " " + device->Manufacturer(), QVariant::fromValue( device ) );
//    }
//    ui->GsType->setCurrentIndex( -1 );
    ui->ActuationOnTime->setValidator( new QIntValidator( 1, 2000, this ) );
    ui->ActuationOffTime->setValidator( new QIntValidator( 1, 2000, this ) );
    ui->MaxExpenditure->setValidator( new QIntValidator( 10, 660, this ) );
    ui->MaxWorkPressure->setValidator( new QIntValidator( 10, 315, this ) );
    ui->Lost->setValidator( new QDoubleValidator( 0.01, 10.0, 2, this ) );

    on_ControlType_activated( ui->ControlType->currentIndex() );
    on_PosCount_valueChanged( ui->PosCount->value() );

    if ( !mNewMode )
    {
        FromParams();
    }

//убраны из набора тестов
    ui->ActuationOffTime->setVisible( false );
    ui->ActuationOnTime->setVisible( false );

    ui->l_actuation_off_time->setVisible( false );
    ui->l_actuation_on_time->setVisible( false );

    CheckRights();
}

HydroTitleInfo::~HydroTitleInfo()
{
    delete ui;
}

bool HydroTitleInfo::SaveInputParams()
{
    test::hydro::Parameters& params = test::hydro::Parameters::Instance();
    test::CURRENT_PARAMS = &params;

    if ( mNewMode )
        params.Reset();

    bool res = true;

    res *= ParamChecker( ui->l_sn,              params.SerNo( ui->SerNo->text() ) );
    res *= ParamChecker( ui->l_def_expenditure, params.DefaultExpenditure( ui->DefExpenditure->text() ) );
    res *= ParamChecker( ui->l_voltage,         params.Voltage( ui->Voltage->currentText() ) );
    res *= ParamChecker( ui->l_voltage_type,    params.VoltageType( ui->VoltageType->currentText() ) );
    res *= ParamChecker( ui->l_reel_count,      params.PosCount( QString::number( ui->PosCount->value() ) ) );
    res *= ParamChecker( ui->l_control_type,    params.ControlType( ui->ControlType->currentText() ) );
    if ( params.ControlType() == test::CT_HYDRO_ELECTRIC )
    {
        res *= ParamChecker( ui->l_min_control_pressure, params.MinControlPressure( QString::number( ui->MinControlPressure->value() ) ) );
        res *= ParamChecker( ui->l_max_control_pressure, params.MaxControlPressure( QString::number( ui->MaxControlPressure->value() ) ) );
//        res *= ParamChecker( ui->l_test_control_Pressure, params.TestControlPressure( QString::number( ui->TestControlPressure->value() ) ) );
    }
    res *= ParamChecker( ui->l_voltage_range,      params.VoltageRange( QString::number( ui->VoltageRange->value() ) ) );
    res *= ParamChecker( ui->l_lost,  ValidateRange( ui->Lost, params.Lost( ui->Lost->text() ) ) );
    res *= ParamChecker( ui->l_max_expenditure,    ValidateRange( ui->MaxExpenditure, params.MaxExpenditure( ui->MaxExpenditure->text() ) ) );
    res *= ParamChecker( ui->l_max_work_pressure,  ValidateRange( ui->MaxWorkPressure, params.MaxWorkPressure( ui->MaxWorkPressure->text() ) ) );
    res *= ParamChecker( ui->l_min_pressure,       params.MinTestPressure( QString::number( ui->MinPressure->value() ) ) );
    res *= ParamChecker( ui->l_herm_pressure,      params.HermPressure( QString::number( ui->HermPressure->value() ) ) );
    res *= ParamChecker( ui->l_herm_signal,        params.HermSignal( ui->HermSignal->currentText() ) );
    res *= ParamChecker( ui->l_pabt_signal,        params.PABTSignal( ui->PABTSignal->currentText() ) );
    res *= ParamChecker( ui->l_pbat_signal,        params.PBATSignal( ui->PBATSignal->currentText() ) );
//убраны из набора тестов
//    res *= ParamChecker( ui->l_gs_type,         params.GsType( ui->GsType->currentText() ) && ui->GsType->currentIndex() >= 0 );
//    res *= ParamChecker( ui->l_actuation_on_time,  ValidateRange( ui->ActuationOnTime, params.ActuationOnTime( ui->ActuationOnTime->text() ) ) );
//    res *= ParamChecker( ui->l_actuation_off_time, ValidateRange( ui->ActuationOffTime, params.ActuationOffTime( ui->ActuationOffTime->text() ) ) );

    return res;
}

void HydroTitleInfo::FromParams()
{
    test::hydro::Parameters& params = test::hydro::Parameters::Instance();
    ui->SerNo->setText( params.SerNo() );
    ui->DefExpenditure->setValue( params.DefaultExpenditure() );
    ui->Voltage->setCurrentIndex( ui->Voltage->findText( test::ToString( params.Voltage()  ) ) );
    ui->VoltageType->setCurrentIndex( ui->VoltageType->findText( test::ToString( params.VoltageType() ) ) );
    ui->PosCount->setValue( params.PosCount() );
    on_PosCount_valueChanged( ui->PosCount->value() );
    ui->ControlType->setCurrentIndex( ui->ControlType->findText( test::ToString( params.ControlType() ) ) );
    on_ControlType_activated( ui->ControlType->currentIndex() );
    ui->MinControlPressure->setValue( params.MinControlPressure() );
    ui->MaxControlPressure->setValue( params.MaxControlPressure() );
//    ui->TestControlPressure->setValue( params.TestControlPressure() );

    ui->VoltageRange->setValue( params.VoltageRange() );
    ui->Lost->setText( test::ToString( params.Lost() ) );
    ui->MaxExpenditure->setText( test::ToString( params.MaxExpenditure() ) );
    ui->MaxWorkPressure->setText( test::ToString( params.MaxWorkPressure() ) );

    ui->MinPressure->setValue( params.MinTestPressure() );
    ui->HermPressure->setValue( params.HermPressure() );

    ui->HermSignal->setCurrentIndex( ui->HermSignal->findText( test::ToString( params.HermSignal() ) ) );
    ui->PABTSignal->setCurrentIndex( ui->PABTSignal->findText( test::ToString( params.PABTSignal() ) ) );
    ui->PBATSignal->setCurrentIndex( ui->PBATSignal->findText( test::ToString( params.PBATSignal() ) ) );

    //убраны из набора тестов
//    ui->GsType->setCurrentIndex( ui->GsType->findText( params.GsType() ) );
//    on_GsType_activated( ui->GsType->currentIndex() );
//    ui->ActuationOnTime->setText( test::ToString( params.ActuationOnTime() ) );
//    ui->ActuationOffTime->setText( test::ToString( params.ActuationOffTime() ) );
}

void HydroTitleInfo::on_buttonBox_accepted()
{    
    auto CheckPower = []() -> bool
    {
        test::hydro::Parameters& params = test::hydro::Parameters::Instance();

        const int pomp_max_power = 55;
        const int pomp_count = 2;

        double power = pomp_count * pomp_max_power;
        bool combo1 = params.DefaultExpenditure()*params.MaxWorkPressure()/540 > power;
        bool combo2 = params.DefaultExpenditure()*params.HermPressure()/540 > power;
        bool combo3 = params.MaxExpenditure()*params.MaxWorkPressure()/540 > power;


        QString err_msg;
        if ( combo1 )
            err_msg += "Необходимо скорректировать расход по умолчанию и максимальное рабочее давление\n";
        if ( combo2 )
            err_msg += "Необходимо скорректировать расход по умолчанию и давление для испытания герметичности\n";
        if ( combo3 )
            err_msg += "Необходимо скорректировать максимальный расход и максимальное рабочее давление\n";

        if ( !err_msg.isEmpty() )
        {
            QMessageBox msg;
            msg.setWindowTitle( "Превышена допустимая мощьность насосов" );
            msg.setText( err_msg );
            msg.setStandardButtons( QMessageBox::Ok );
            msg.setModal( true );
            msg.exec();
        }
        return !combo1 && !combo2 && !combo3;
    };

    if ( SaveInputParams() )
    {
        if (!CheckPower())
            return;

        if ( test::hydro::Parameters::Instance().ControlType() == test::CT_ELECTRIC )
        {
            foreach (auto test_ptr, test::hydro::Parameters::Instance().TestCollection().Tests())
            {
                if ( test_ptr->ID() == 6 )
                    test_ptr->Disabled( true );
            }
        }

        hide();
        if ( mChildWindow.get() )
            QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
        mChildWindow.reset( new StandParams( mNewMode ) );
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

void HydroTitleInfo::on_ControlType_activated(int index)
{
    auto set_control_pressure_visible = [ this ]( bool visible )
    {
        ui->l_min_control_pressure->setVisible( visible );
        ui->l_max_control_pressure->setVisible( visible );
        ui->l_test_control_Pressure->setVisible( false );

        ui->MinControlPressure->setVisible( visible );
        ui->MaxControlPressure->setVisible( visible );
        ui->TestControlPressure->setVisible( false );
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

/*void HydroTitleInfo::on_GsType_activated(int index)
{
    /// по индексу извлечь девайс прочитать его свойства выставить пределы на контролах

    examinee::Device const* ptr = ui->GsType->itemData( index ).value< examinee::Device const* >();
    examinee::Device::PropList const& prop = ptr->Properties();

    typedef std::pair< qint32, qint32 > Range;
    Range min_pressure_control = { 10, 30 };
    Range max_pressure_control = { 100, 300 };

    auto it = prop.end();

    it = prop.find( "ДавлениеУправления.Максимум" );
    if ( it != prop.end() )
    {
        bool t = false;
        double val = it->toDouble( &t );
        if ( val )
            max_pressure_control.second = val;
    }

    it = prop.find( "ДавлениеУправления.Минимум" );
    if ( it != prop.end() )
    {
        bool t = false;
        double val = it->toDouble( &t );
        if ( val )
            min_pressure_control.first = val;
    }

    //задать диапазоны минимального и максимального давления управления
    ui->MinControlPressure->setRange( min_pressure_control.first, min_pressure_control.second );
    ui->MaxControlPressure->setRange( max_pressure_control.first, max_pressure_control.second );
    ui->TestControlPressure->setRange( min_pressure_control.first, max_pressure_control.second );

    //задать установить значения на минимум
    ui->MinControlPressure->setValue( min_pressure_control.first );
    ui->MaxControlPressure->setValue( max_pressure_control.first );
    ui->TestControlPressure->setValue( max_pressure_control.first );

    ui->MinControlPressure->setToolTip( QString::number( min_pressure_control.first ) + "..." + QString::number( min_pressure_control.second ) + " Бар." );
    ui->MaxControlPressure->setToolTip( QString::number( max_pressure_control.first ) + "..." + QString::number( max_pressure_control.second ) + " Бар." );
    ui->TestControlPressure->setToolTip( QString::number( min_pressure_control.first ) + "..." + QString::number( max_pressure_control.second ) + " Бар." );

    it = prop.find( "ТипУправления" );
    if ( it != prop.end() )
    {
        //включить комбу на установленной опции
        auto index = ui->ControlType->findText( it->toString(), Qt::MatchExactly );
        ui->ControlType->setCurrentIndex( index );
        on_ControlType_activated( index );
    }
}
*/

void HydroTitleInfo::on_PosCount_valueChanged(int arg1)
{
    switch ( arg1 )
    {
    case 2:
        ui->HermSignal->removeItem( 2 );
        ui->PABTSignal->removeItem( 2 );
        ui->PBATSignal->removeItem( 2 );
        break;
    case 3:
        if ( ui->HermSignal->findText( "Катушка Б" ) == -1 )
        {
            ui->HermSignal->insertItem( 2, "Катушка Б" );
            ui->PABTSignal->insertItem( 2, "Катушка Б" );
            ui->PBATSignal->insertItem( 2, "Катушка Б" );
        }
        break;
    default:
        break;
    }
}

void HydroTitleInfo::CheckRights()
{
    if ( app::Settings::Instance().UserAccess() == app::User )
    {
        ui->SerNo->setEnabled( false );
        ui->DefExpenditure->setEnabled( false );
        ui->Voltage->setEnabled( false );
        ui->VoltageType->setEnabled( false );
        ui->PosCount->setEnabled( false );
        ui->ControlType->setEnabled( false );
        ui->MinControlPressure->setEnabled( false );
        ui->MaxControlPressure->setEnabled( false );
//        ui->TestControlPressure->setEnabled( false );
        ui->VoltageRange->setEnabled( false );
        ui->Lost->setEnabled( false );
        ui->MaxExpenditure->setEnabled( false );
        ui->MaxWorkPressure->setEnabled( false );
        ui->MinPressure->setEnabled( false );
        ui->HermPressure->setEnabled( false );
        ui->HermSignal->setEnabled( false );
        ui->PABTSignal->setEnabled( false );
        ui->PBATSignal->setEnabled( false );
        ui->ActuationOffTime->setEnabled( false );
        ui->ActuationOnTime->setEnabled( false );
    }
}
