#include "hydro_title_info.h"
#include "ui_hydro_title_info.h"
#include "test_case/tests.h"
//#include "test_form.h"
#include "stand_params.h"
#include <QMessageBox>
#include <test_case/test_params.h>


HydroTitleInfo::HydroTitleInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HydroTitleInfo)
{
    ui->setupUi(this);

    foreach ( examinee::Device const* device, test::HydroTests.Devices().Get())
    {
        ui->GsType->addItem( device->Name() + " " + device->Manufacturer(), QVariant::fromValue( device ) );
    }
    ui->GsType->setCurrentIndex( -1 );
    ui->ActuationOnTime->setValidator( new QIntValidator( 1, 2000, this ) );
    ui->ActuationOffTime->setValidator( new QIntValidator( 1, 2000, this ) );
    ui->MaxExpenditure->setValidator( new QIntValidator( 10, 1100, this ) );
    ui->MaxWorkPressure->setValidator( new QIntValidator( 10, 400, this ) );

    on_ControlType_activated( ui->ControlType->currentIndex() );
    on_ReelCount_valueChanged( ui->ReelCount->value() );
}

HydroTitleInfo::~HydroTitleInfo()
{
    delete ui;
}

bool HydroTitleInfo::SaveInputParams()
{
    test::hydro::Parameters& params = test::hydro::Parameters::Instance();
    test::CURRENT_PARAMS = &params;
    params.Reset();

    bool res = true;

    auto ParamChecker = []( QLabel* control, bool r ) -> bool
    {
        QPalette palette = control->palette();

        if ( !r )
        {
            palette.setColor( control->backgroundRole(), Qt::red );
            control->setAutoFillBackground( true );
        }
        else
        {
            control->setAutoFillBackground( false );
        }
        control->setPalette( palette );
        return r;
    };
    auto ValidateRange = []( QLineEdit* control, bool r ) -> bool
    {
        QString text = control->text();
        int pos = control->cursorPosition();
        return std::min( control->validator()->validate( text, pos ) == QValidator::Acceptable , r );
    };

    res *= ParamChecker( ui->l_sn,              params.SerNo( ui->SerNo->text() ) );
    res *= ParamChecker( ui->l_gs_type,         params.GsType( ui->GsType->currentText() ) );
    res *= ParamChecker( ui->l_voltage,         params.Voltage( ui->Voltage->currentText() ) );
    res *= ParamChecker( ui->l_voltage_type,    params.VoltageType( ui->VoltageType->currentText() ) );
    res *= ParamChecker( ui->l_reel_count,      params.ReelCount( QString::number( ui->ReelCount->value() ) ) );
    res *= ParamChecker( ui->l_control_type,    params.ControlType( ui->ControlType->currentText() ) );
    if ( params.ControlType() == test::CT_HYDRO_ELECTRIC )
    {
        res *= ParamChecker( ui->l_min_control_pressure, params.MinControlPressure( QString::number( ui->MinControlPressure->value() ) ) );
        res *= ParamChecker( ui->l_max_control_pressure, params.MaxControlPressure( QString::number( ui->MaxControlPressure->value() ) ) );
    }
    res *= ParamChecker( ui->l_voltage_range,      params.VoltageRange( QString::number( ui->VoltageRange->value() ) ) );
    res *= ParamChecker( ui->l_max_expenditure,    ValidateRange( ui->MaxExpenditure, params.MaxExpenditure( ui->MaxExpenditure->text() ) ) );
    res *= ParamChecker( ui->l_max_work_pressure,  ValidateRange( ui->MaxWorkPressure, params.MaxWorkPressure( ui->MaxWorkPressure->text() ) ) );
    res *= ParamChecker( ui->l_min_pressure,       params.MinTestPressure( QString::number( ui->MinPressure->value() ) ) );
    res *= ParamChecker( ui->l_herm_pressure,      params.HermPressure( QString::number( ui->HermPressure->value() ) ) );
    res *= ParamChecker( ui->l_herm_signal,        params.HermSignal( ui->HermSignal->currentText() ) );
    res *= ParamChecker( ui->l_pabt_signal,        params.PABTSignal( ui->PABTSignal->currentText() ) );
    res *= ParamChecker( ui->l_pbat_signal,        params.PBATSignal( ui->PBATSignal->currentText() ) );
    res *= ParamChecker( ui->l_actuation_on_time,  ValidateRange( ui->ActuationOnTime, params.ActuationOnTime( ui->ActuationOnTime->text() ) ) );
    res *= ParamChecker( ui->l_actuation_off_time, ValidateRange( ui->ActuationOffTime, params.ActuationOffTime( ui->ActuationOffTime->text() ) ) );

    return res;
}

void HydroTitleInfo::on_buttonBox_accepted()
{
    if ( SaveInputParams() )
    {
        hide();
        if ( mChildWindow.get() )
            QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
        mChildWindow.reset( new StandParams() );
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

void HydroTitleInfo::on_GsType_activated(int index)
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
        qint32 val = it->toInt( &t );
        if ( val )
            max_pressure_control.second = val;
    }

    it = prop.find( "ДавлениеУправления.Минимум" );
    if ( it != prop.end() )
    {
        bool t = false;
        qint32 val = it->toInt( &t );
        if ( val )
            min_pressure_control.first = val;
    }

    //задать диапазоны минимального и максимального давления управления
    ui->MinControlPressure->setRange( min_pressure_control.first, min_pressure_control.second );
    ui->MaxControlPressure->setRange( max_pressure_control.first, max_pressure_control.second );

    //задать установить значения на минимум
    ui->MinControlPressure->setValue( min_pressure_control.first );
    ui->MaxControlPressure->setValue( max_pressure_control.first );

    ui->MinControlPressure->setToolTip( QString::number( min_pressure_control.first ) + "..." + QString::number( min_pressure_control.second ) + " Бар." );
    ui->MaxControlPressure->setToolTip( QString::number( max_pressure_control.first ) + "..." + QString::number( max_pressure_control.second ) + " Бар." );

    it = prop.find( "ТипУправления" );
    if ( it != prop.end() )
    {
        //включить комбу на установленной опции
        auto index = ui->ControlType->findText( it->toString(), Qt::MatchExactly );
        ui->ControlType->setCurrentIndex( index );
        on_ControlType_activated( index );
    }
}


void HydroTitleInfo::on_ReelCount_valueChanged(int arg1)
{
    switch ( arg1 )
    {
    case 1:
        ui->HermSignal->removeItem( 2 );
        ui->PABTSignal->removeItem( 2 );
        ui->PBATSignal->removeItem( 2 );
        break;
    case 2:
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
