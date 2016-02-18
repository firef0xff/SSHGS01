#include "servo_title_info.h"
#include "ui_servo_title_info.h"
#include "test_form.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_servo.h"
#include "test_case/test.h"
#include "settings/settings.h"
//#include <climits>

ServoTitleInfo::ServoTitleInfo(bool new_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServoTitleInfo),
    mNewMode( new_mode )
{
    ui->setupUi(this);

//    foreach ( examinee::Device const* device, test::servo::Parameters::Instance().TestCollection().Devices().Get() )
//    {
//        ui->GsType->addItem( device->Name() + " " + device->Manufacturer(), QVariant::fromValue( device ) );
//    }

    ui->PressureNominal->setValidator( new QIntValidator( 10, 315, this ) );
    ui->MaxExpenditure->setValidator( new QIntValidator( 1, 660, this ) );
    ui->MaxExpenditureA->setValidator( new QIntValidator( 1, 660, this ) );
    ui->MaxExpenditureB->setValidator( new QIntValidator( 1, 660, this ) );
    ui->ControlReelResist->setValidator( new QDoubleValidator( INT32_MIN, INT32_MAX , 2, this ) );
    on_RaspredControl_activated( ui->RaspredControl->currentIndex() );    
    on_ControlType_activated( ui->ControlType->currentIndex() );
    on_PosCount_valueChanged( ui->PosCount->value() );

    if ( !mNewMode )
        FromParams();
    CheckRights();
}

ServoTitleInfo::~ServoTitleInfo()
{
    delete ui;
}

bool ServoTitleInfo::SaveInputParams()
{
    test::servo::Parameters& params = test::servo::Parameters::Instance();
    test::CURRENT_PARAMS = &params;

    if (mNewMode)
        params.Reset();

    bool res = true;

    auto ParamChecker = []( QWidget* control, bool r ) -> bool
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

    res *= ParamChecker( ui->l_ser_no,          params.SerNo( ui->SerNo->text() ) );
//    res *= ParamChecker( ui->l_gs_type,         params.GsType( ui->GsType->currentText() ) && ui->GsType->currentIndex() >= 0 );
    res *= ParamChecker( ui->l_def_expenditure, params.DefaultExpenditure( ui->DefExpenditure->text() ) );
    res *= ParamChecker( ui->l_reel_count,      params.PosCount( ui->PosCount->text() ) );

    res *= ParamChecker( ui->l_raspred_control,    params.ControlType( ui->RaspredControl->currentText() ) );
    if ( params.ControlType() == test::CT_HYDRO_ELECTRIC )
    {
        res *= ParamChecker( ui->l_min_control_pressure, params.MinControlPressure( QString::number( ui->MinControlPressure->value() ) ) );
        res *= ParamChecker( ui->l_max_control_pressure, params.MaxControlPressure( QString::number( ui->MaxControlPressure->value() ) ) );
    }
    res *= ParamChecker( ui->l_max_expenditure,    ValidateRange( ui->MaxExpenditure, params.MaxExpenditure( ui->MaxExpenditure->text() ) ) );

    res *= ParamChecker( ui->l_control_type,        params.ReelControl( ui->ControlType->currentText() ) );
    res *= ParamChecker( ui->l_pressure_testing,    params.PressureTesting( QString::number( ui->PressureTesting->value() ) ) );
    res *= ParamChecker( ui->l_frequency_inc,       params.FrequencyInc( QString::number( ui->FrequencyInc->value() ) ) );

    res *= ParamChecker( ui->l_nominal_pressure,     ValidateRange( ui->PressureNominal, params.PressureNominal( ui->PressureNominal->text() ) ) );

    res *= ParamChecker( ui->l_control_signal,    params.ControlSignal( ui->ControlSignal->currentText() ) );
    res *= ParamChecker( ui->l_control_signal_ampl,    params.Amplitudes( ui->ControlSignalAmpl0->text(), ui->ControlSignalAmpl1->text(), ui->ControlSignalAmpl2->text() ) );

    res *= ParamChecker( ui->l_signal_state_a,    ValidateRange( ui->SignalStateA, params.SignalStateA( ui->SignalStateA->text() ) ) );
    res *= ParamChecker( ui->l_signal_state_0,    ValidateRange( ui->SignalState0, params.SignalState0( ui->SignalState0->text() ) ) );
    if ( params.PosCount() == 3 )
        res *= ParamChecker( ui->l_signal_state_b,    ValidateRange( ui->SignalStateB, params.SignalStateB( ui->SignalStateB->text() ) ) );

    if ( params.ReelControl() == test::RC_REEL )
    {
        res *= ParamChecker( ui->l_control_reel_resist, ValidateRange( ui->ControlReelResist, params.ControlReelResist( ui->ControlReelResist->text() ) ) );
    }
    res *= ParamChecker( ui->l_max_expenditure_a,    ValidateRange( ui->MaxExpenditureA, params.MaxExpenditureA( ui->MaxExpenditureA->text() ) ) );
    res *= ParamChecker( ui->l_max_expenditure_b,    ValidateRange( ui->MaxExpenditureB, params.MaxExpenditureB( ui->MaxExpenditureB->text() ) ) );

    if ( params.ReelControl() == test::RC_CONTROL_BOX )
    {
        res *= ParamChecker( ui->l_voltage,         params.Voltage( ui->Voltage->currentText() ) );
    }

    bool channel_a = ui->TestChA->checkState() == Qt::Checked;
    bool channel_b = ui->TestChB->checkState() == Qt::Checked;
    if (!channel_a && !channel_b )
    {
        ParamChecker(ui->TestChA , false);
        ParamChecker(ui->TestChB, false);
        res *= false;
    }
    else
    {
        ParamChecker(ui->TestChA, true);
        ParamChecker(ui->TestChB, true);
        params.TestChannelA( channel_a );
        params.TestChannelB( channel_b );
        params.SignalOnChannelA( ui->ControlReelChA->currentText() );
        params.SignalOnChannelB( ui->ControlReelChB->currentText() );
    }

    res *= ParamChecker( ui->l_Frequency,   params.StartFrequency( QString::number( ui->Frequency->value() ) ) );
    res *= ParamChecker( ui->l_Ampl_Inc,    params.AmplInc( QString::number( ui->AmplInc->value() ) ) );

    return res;
}
void ServoTitleInfo::FromParams()
{
    test::servo::Parameters& params = test::servo::Parameters::Instance();

    ui->SerNo->setText( params.SerNo() );
//    ui->GsType->setCurrentIndex( ui->GsType->findText( params.GsType() ) );
    ui->DefExpenditure->setValue( params.DefaultExpenditure() );
    ui->PosCount->setValue( params.PosCount() );
    on_PosCount_valueChanged( ui->PosCount->value() );
    ui->RaspredControl->setCurrentIndex( ui->RaspredControl->findText( test::ToString( params.ControlType() ) ) );
    on_RaspredControl_activated( ui->RaspredControl->currentIndex() );
    ui->MinControlPressure->setValue( params.MinControlPressure() );
    ui->MaxControlPressure->setValue( params.MaxControlPressure() );

    ui->MaxExpenditure->setText( test::ToString( params.MaxExpenditure() ) );

    ui->ControlType->setCurrentIndex( ui->ControlType->findText( test::ToString( params.ReelControl() ) ) );
    on_ControlType_activated( ui->ControlType->currentIndex() );

    ui->PressureTesting->setValue( params.PressureTesting() );
    ui->FrequencyInc->setValue( params.FrequencyInc() );

    ui->PressureNominal->setText( test::ToString( params.PressureNominal() ) );

    ui->ControlSignal->setCurrentIndex( ui->ControlSignal->findText( test::ToString( params.ControlSignal() ) ) );
    on_ControlSignal_activated( ui->ControlSignal->currentIndex() );

    ui->ControlSignalAmpl0->setText( test::ToString( params.Amplitudes()[0] ) );
    ui->ControlSignalAmpl1->setText( test::ToString( params.Amplitudes()[1] ) );
    ui->ControlSignalAmpl2->setText( test::ToString( params.Amplitudes()[2] ) );

    ui->SignalStateA->setText( test::ToString( params.SignalStateA() ) );
    ui->SignalState0->setText( test::ToString( params.SignalState0() ) );
    if (params.PosCount() == 3)
    {
        ui->SignalStateB->setText( test::ToString( params.SignalStateB() ) );
    }

    if ( params.ReelControl() == test::RC_REEL )
    {
        ui->ControlReelResist->setText( test::ToString( params.ControlReelResist() ) );
    }

    ui->MaxExpenditureA->setText( test::ToString( params.MaxExpenditureA() ) );
    ui->MaxExpenditureB->setText( test::ToString( params.MaxExpenditureB() ) );
    if ( params.ReelControl() == test::RC_CONTROL_BOX )
    {
        ui->Voltage->setCurrentIndex( ui->Voltage->findText( test::ToString( params.Voltage() ) ) );
    }

    bool channel_a = params.TestChannelA();
    if ( channel_a )
    {
        ui->TestChA->setChecked( Qt::Checked );
        ui->ControlReelChA->setCurrentIndex(ui->ControlReelChA->findText( test::ToString( params.SignalOnChannelA() ) ) );
    }
    bool channel_b = params.TestChannelB();
    if ( channel_b )
    {
        ui->TestChB->setChecked( Qt::Checked );
        ui->ControlReelChB->setCurrentIndex(ui->ControlReelChB->findText( test::ToString( params.SignalOnChannelB() ) ) );
    }

    ui->Frequency->setValue(  params.StartFrequency() );
    ui->AmplInc->setValue( params.AmplInc() );
}

void ServoTitleInfo::on_buttonBox_accepted()
{
    auto CheckPower = []() -> bool
    {
        test::servo::Parameters& params = test::servo::Parameters::Instance();

        const int pomp_max_power = 55;
        const int pomp_count = 2;

        double power = pomp_count * pomp_max_power;
        bool combo1 = params.DefaultExpenditure()*params.PressureNominal()/540 > power;
        bool combo2 = params.DefaultExpenditure()*params.PressureTesting()/540 > power;
        bool combo3 = params.MaxExpenditureA()*params.PressureNominal()/540 > power;
        bool combo4 = params.MaxExpenditureB()*params.PressureNominal()/540 > power;

        QString err_msg;
        if ( combo1 )
            err_msg += "Необходимо скорректировать расход по умолчанию и номинальное давление\n";
        if ( combo2 )
            err_msg += "Необходимо скорректировать расход по умолчанию и пробное давление\n";
        if ( combo3 )
            err_msg += "Необходимо скорректировать максимальный расход в канале А и номинальное давление\n";
        if ( combo4 )
            err_msg += "Необходимо скорректировать максимальный расход в канале Б и номинальное давление\n";

        if ( !err_msg.isEmpty() )
        {
            QMessageBox msg;
            msg.setWindowTitle( "Превышена допустимая мощьность насосов" );
            msg.setText( err_msg );
            msg.setStandardButtons( QMessageBox::Ok );
            msg.setModal( true );
            msg.exec();
        }
        return !combo1 && !combo2 && !combo3 && !combo4;
    };
    if ( SaveInputParams() )
    {
        if (!CheckPower())
            return;

        if ( test::servo::Parameters::Instance().PosCount() == 2 )
        {
            foreach (auto test_ptr, test::servo::Parameters::Instance().TestCollection().Tests())
            {
                if ( test_ptr->ID() == 14 || test_ptr->ID() == 15 )
                    test_ptr->Disabled( true );
            }
        }

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

void ServoTitleInfo::on_buttonBox_rejected()
{
    close();
}

void ServoTitleInfo::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}



void ServoTitleInfo::on_RaspredControl_activated(int index)
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

void ServoTitleInfo::on_ControlSignal_activated(int index)
{
    test::SIGNAL_TYPE s_type = test::ST_UNKNOWN;
    auto text = ui->ControlSignal->itemText( index );
    test::ParseValue( s_type, text );
    ui->SignalStateA->setEnabled(true);
    ui->SignalStateB->setEnabled(true);
    ui->SignalState0->setEnabled(true);

    ui->SignalStateA->setText("");
    ui->SignalStateB->setText("");
    ui->SignalState0->setText("");    

    ui->SignalStateA->setToolTip( text );
    ui->SignalStateB->setToolTip( text );
    ui->SignalState0->setToolTip( text );    

    switch (s_type)
    {
        case test::ST_0_20_mA:
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 20, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 20, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 20, 2, this ) );
            break;
        case test::ST_4_20_mA:
            ui->SignalStateA->setValidator( new QDoubleValidator( 4, 20, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 4, 20, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 4, 20, 2, this ) );
            break;
        case test::ST_10_10_mA:
            ui->SignalStateA->setValidator( new QDoubleValidator( -10, 10, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( -10, 10, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( -10, 10, 2, this ) );
            break;
        case test::ST_10_10_V:
            ui->SignalStateA->setValidator( new QDoubleValidator( -10, 10, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( -10, 10, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( -10, 10, 2, this ) );
            break;
        case test::ST_0_10_V:
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 10, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 10, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 10, 2, this ) );
            break;
//        case test::ST_15_15_mA:
//            ui->SignalStateA->setValidator( new QDoubleValidator( -15, 15, 2, this ) );
//            ui->SignalStateB->setValidator( new QDoubleValidator( -15, 15, 2, this ) );
//            ui->SignalState0->setValidator( new QDoubleValidator( -15, 15, 2, this ) );
//            break;
        case test::ST_20_20_mA:
            ui->SignalStateA->setValidator( new QDoubleValidator( -20, 20, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( -20, 20, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( -20, 20, 2, this ) );
            break;
        case test::ST_40_40_mA:
            ui->SignalStateA->setValidator( new QDoubleValidator( -40, 40, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( -40, 40, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( -40, 40, 2, this ) );
            break;
        case test::ST_100_mA:            
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 100, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 100, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 100, 2, this ) );
            break;
        case test::ST_300_mA:            
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 300, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 300, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 300, 2, this ) );
            break;
        case test::ST_600_mA:           
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 600, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 600, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 600, 2, this ) );
            break;
        case test::ST_860_mA:            
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 860, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 860, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 860, 2, this ) );
            break;
        case test::ST_1600_mA:           
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 1600, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 1600, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 1600, 2, this ) );
            break;
        case test::ST_2500_mA:            
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 2500, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 2500, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 2500, 2, this ) );
            break;
        case test::ST_3750_mA:            
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 3750, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 3750, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 3750, 2, this ) );
            break;
        case test::ST_5000_mA:            
            ui->SignalStateA->setValidator( new QDoubleValidator( 0, 5000, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( 0, 5000, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( 0, 5000, 2, this ) );
            break;
        default:
            ui->SignalStateA->setEnabled(false);
            ui->SignalStateB->setEnabled(false);
            ui->SignalState0->setEnabled(false);            
            break;
    }
}

void ServoTitleInfo::on_ControlType_activated(int index)
{
    test::RELL_CONTROL control = test::RC_UNKNOWN;
    auto text = ui->ControlType->itemText( index );
    test::ParseValue( control, text );

    ui->SignalStateA->setVisible( ui->PosCount->value() >= 2);
    ui->SignalStateB->setVisible( ui->PosCount->value() == 3);
    ui->SignalState0->setVisible( ui->PosCount->value() >= 2);
    ui->Voltage->setVisible( control == test::RC_CONTROL_BOX );

    ui->l_signal_state_a->setVisible( ui->PosCount->value() >= 2);
    ui->l_signal_state_b->setVisible( ui->PosCount->value() == 3);
    ui->l_signal_state_0->setVisible( ui->PosCount->value() >= 2);
    ui->l_voltage->setVisible( control == test::RC_CONTROL_BOX );

    ui->ControlReelResist->setVisible( control == test::RC_REEL );
    ui->l_control_reel_resist->setVisible( control == test::RC_REEL );

    ui->ControlSignal->clear();
    if ( control == test::RC_CONTROL_BOX )
    {
        ui->ControlSignal->addItem( test::ToString( test::ST_0_20_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_4_20_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_10_10_mA ) );
//        ui->ControlSignal->addItem( test::ToString( test::ST_15_15_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_20_20_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_40_40_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_10_10_V ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_0_10_V ) );
    }
    else if ( control == test::RC_REEL )
    {
        ui->ControlSignal->addItem( test::ToString( test::ST_100_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_300_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_600_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_860_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_1600_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_2500_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_3750_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_5000_mA ) );
    }
    ui->ControlSignal->setCurrentIndex( -1 );
    on_ControlSignal_activated( ui->ControlSignal->currentIndex() );
}

void ServoTitleInfo::on_TestChA_clicked()
{
    if ( ui->TestChA->checkState() == Qt::Checked )
    {
        if ( ui->TestChB->checkState() == Qt::Checked )
        {
            //если канал Б кликнут, смотрим что выставленно там и выставляем другой если возможно
            test::CONTROL_SIGNAL sig = test::CS_UNKNOWN;
            test::CONTROL_SIGNAL sig_toSet = test::CS_UNKNOWN;
            test::ParseValue( sig, ui->ControlReelChB->currentText() );
            if ( sig == test::CS_REEL_A )
                sig_toSet = test::CS_REEL_B;
            if ( sig == test::CS_REEL_B )
                sig_toSet = test::CS_REEL_A;
            int index_to_set = ui->ControlReelChA->findText( test::ToString( sig_toSet ) );
            if ( index_to_set == -1 )
            {
                //если не возможно то сбрасываем checked
                ui->TestChA->setChecked( Qt::Unchecked );
                return;
            }
            ui->ControlReelChA->setCurrentIndex( index_to_set );
        }
        else
        {
            //если канал Б не кликнут то выставляем канал А по умолчанию
            ui->ControlReelChA->setCurrentIndex( ui->ControlReelChA->findText( test::ToString( test::CS_REEL_A ) ) );
        }
    }
}

void ServoTitleInfo::on_TestChB_clicked()
{
    if ( ui->TestChB->checkState() == Qt::Checked )
    {
        if ( ui->TestChA->checkState() == Qt::Checked )
        {
            //если канал Б кликнут, смотрим что выставленно там и выставляем другой если возможно
            test::CONTROL_SIGNAL sig = test::CS_UNKNOWN;
            test::CONTROL_SIGNAL sig_toSet = test::CS_UNKNOWN;
            test::ParseValue( sig, ui->ControlReelChA->currentText() );
            if ( sig == test::CS_REEL_A )
                sig_toSet = test::CS_REEL_B;
            if ( sig == test::CS_REEL_B )
                sig_toSet = test::CS_REEL_A;
            int index_to_set = ui->ControlReelChB->findText( test::ToString( sig_toSet ) );
            if ( index_to_set == -1 )
            {
                //если не возможно то сбрасываем checked
                ui->TestChB->setChecked( Qt::Unchecked );
                return;
            }
            ui->ControlReelChB->setCurrentIndex( index_to_set );
        }
        else
        {
            //если канал Б не кликнут то выставляем канал А по умолчанию
            int index_to_set = ui->ControlReelChA->findText( test::ToString( test::CS_REEL_B ) );
            if ( index_to_set == -1 )
            {
                //если не возможно то выставляем Канал А
                index_to_set = ui->ControlReelChA->findText( test::ToString( test::CS_REEL_A ) );
            }
            ui->ControlReelChB->setCurrentIndex( index_to_set );
        }
    }
}

void ServoTitleInfo::on_ControlReelChA_activated(const QString &arg1)
{
    if ( ui->TestChA->checkState() == Qt::Checked )
    {
        if ( ui->TestChB->checkState() == Qt::Checked )
        {
            //если канал Б кликнут, смотрим что выставленно там и выставляем другой если возможно
            test::CONTROL_SIGNAL sig = test::CS_UNKNOWN;
            test::CONTROL_SIGNAL sig_toSet = test::CS_UNKNOWN;
            test::ParseValue( sig, arg1 );
            if ( sig == test::CS_REEL_A )
                sig_toSet = test::CS_REEL_B;
            if ( sig == test::CS_REEL_B )
                sig_toSet = test::CS_REEL_A;
            int index_to_set = ui->ControlReelChB->findText( test::ToString( sig_toSet ) );
            if ( index_to_set == -1 )
            {
                //если не возможно то сбрасываем checked
                ui->TestChB->setChecked( Qt::Unchecked );
                return;
            }
            ui->ControlReelChB->setCurrentIndex( index_to_set );
        }
        //если канал Б не кликнут то ничего не делаем
    }
}

void ServoTitleInfo::on_ControlReelChB_activated(const QString &arg1)
{
    if ( ui->TestChB->checkState() == Qt::Checked )
    {
        if ( ui->TestChA->checkState() == Qt::Checked )
        {
            //если канал Б кликнут, смотрим что выставленно там и выставляем другой если возможно
            test::CONTROL_SIGNAL sig = test::CS_UNKNOWN;
            test::CONTROL_SIGNAL sig_toSet = test::CS_UNKNOWN;
            test::ParseValue( sig, arg1 );
            if ( sig == test::CS_REEL_A )
                sig_toSet = test::CS_REEL_B;
            if ( sig == test::CS_REEL_B )
                sig_toSet = test::CS_REEL_A;
            int index_to_set = ui->ControlReelChA->findText( test::ToString( sig_toSet ) );
            if ( index_to_set == -1 )
            {
                //если не возможно то сбрасываем checked
                ui->TestChA->setChecked( Qt::Unchecked );
                return;
            }
            ui->ControlReelChA->setCurrentIndex( index_to_set );
        }
        //если канал A не кликнут то ничего не делаем
    }
}

void ServoTitleInfo::on_PosCount_valueChanged(int arg1)
{
    // когда меняется количество катушек происходит обновление комбобоксов назначения катушек на каналы испытания
    ui->ControlReelChA->clear();
    ui->ControlReelChA->addItem( test::ToString( test::CS_REEL_A ) );
    ui->ControlReelChB->clear();
    ui->ControlReelChB->addItem( test::ToString( test::CS_REEL_A ) );
    if ( arg1 == 2 )
    {
        ui->ControlReelChA->addItem( test::ToString( test::CS_REEL_B ) );
        ui->ControlReelChB->addItem( test::ToString( test::CS_REEL_B ) );
    }
    if ( arg1 == 1 )
    {
        ui->TestChA->setChecked( Qt::Unchecked );
        ui->TestChB->setChecked( Qt::Unchecked );
    }
    on_ControlType_activated( ui->ControlType->currentIndex() );
}

void ServoTitleInfo::CheckRights()
{
    if ( app::Settings::Instance().UserAccess() == app::User )
    {
        ui->SerNo->setEnabled( false );
        ui->DefExpenditure->setEnabled( false );
        ui->PosCount->setEnabled( false );
        ui->RaspredControl->setEnabled( false );
        ui->MinControlPressure->setEnabled( false );
        ui->MaxControlPressure->setEnabled( false );
        ui->MaxExpenditure->setEnabled( false );
        ui->ControlType->setEnabled( false );
        ui->PressureTesting->setEnabled( false );
        ui->FrequencyInc->setEnabled( false );
        ui->PressureNominal->setEnabled( false );
        ui->ControlSignal->setEnabled( false );
        ui->ControlSignalAmpl0->setEnabled( false );
        ui->ControlSignalAmpl1->setEnabled( false );
        ui->ControlSignalAmpl2->setEnabled( false );
        ui->SignalStateA->setEnabled( false );
        ui->SignalState0->setEnabled( false );
        ui->SignalStateB->setEnabled( false );
        ui->ControlReelResist->setEnabled( false );
        ui->MaxExpenditureA->setEnabled( false );
        ui->MaxExpenditureB->setEnabled( false );
        ui->Voltage->setEnabled( false );
        ui->TestChA->setEnabled( false );
        ui->ControlReelChA->setEnabled( false );
        ui->TestChB->setEnabled( false );
        ui->ControlReelChB->setEnabled( false );
        ui->Frequency->setEnabled( false );
        ui->AmplInc->setEnabled( false );
    }
}
