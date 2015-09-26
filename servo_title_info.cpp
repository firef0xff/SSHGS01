#include "servo_title_info.h"
#include "ui_servo_title_info.h"
#include "test_form.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_servo.h"
//#include <climits>

ServoTitleInfo::ServoTitleInfo(bool new_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServoTitleInfo),
    mNewMode( new_mode )
{
    ui->setupUi(this);

    ui->PressureNominal->setValidator( new QIntValidator( 10, 315, this ) );
    ui->MaxExpenditure->setValidator( new QIntValidator( 1, 660, this ) );
    ui->MaxExpenditureA->setValidator( new QIntValidator( 1, 660, this ) );
    ui->MaxExpenditureB->setValidator( new QIntValidator( 1, 660, this ) );
    ui->ControlReelResist->setValidator( new QDoubleValidator( INT32_MIN, INT32_MAX , 2, this ) );
    on_RaspredControl_activated( ui->RaspredControl->currentIndex() );    
    on_ControlType_activated( ui->ControlType->currentIndex() );

    if ( !mNewMode )
        FromParams();
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

    res *= ParamChecker( ui->l_ser_no,          params.SerNo( ui->SerNo->text() ) );

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

    if ( params.ReelControl() == test::RC_REEL )
    {
        res *= ParamChecker( ui->l_end_signal,          ValidateRange( ui->EndSgnal, params.EndSgnal( ui->EndSgnal->text() ) ) );
        res *= ParamChecker( ui->l_control_reel_resist, ValidateRange( ui->ControlReelResist, params.ControlReelResist( ui->ControlReelResist->text() ) ) );
        params.ReelCount( "1" );
    }

    if ( params.ReelControl() == test::RC_CONTROL_BOX )
    {
        res *= ParamChecker( ui->l_max_expenditure_a,    ValidateRange( ui->MaxExpenditureA, params.MaxExpenditureA( ui->MaxExpenditureA->text() ) ) );
        res *= ParamChecker( ui->l_signal_state_a,    ValidateRange( ui->SignalStateA, params.SignalStateA( ui->SignalStateA->text() ) ) );
        res *= ParamChecker( ui->l_signal_state_0,    ValidateRange( ui->SignalState0, params.SignalState0( ui->SignalState0->text() ) ) );

        //определим используется ли вторая катушка
        bool p1 = params.MaxExpenditureB( ui->MaxExpenditureB->text() );
        bool p2 = params.SignalStateB( ui->SignalStateB->text() );
        if ( p1 || p2 )
            params.ReelCount( "2" );
        else
            params.ReelCount( "1" );

        if ( params.ReelCount() > 1  )
        {
            res *= ParamChecker( ui->l_max_expenditure_b,    ValidateRange( ui->MaxExpenditureB, p1 ) );
            res *= ParamChecker( ui->l_signal_state_b,    ValidateRange( ui->SignalStateB, p2 ) );
        }
        else
        {
            ParamChecker( ui->l_max_expenditure_b, true );
        }
    }

    return res;
}
void ServoTitleInfo::FromParams()
{
    test::servo::Parameters& params = test::servo::Parameters::Instance();

    ui->SerNo->setText( params.SerNo() );
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

    if ( params.ReelControl() == test::RC_REEL )
    {
        ui->EndSgnal->setText( test::ToString( params.EndSgnal() ) );
        ui->ControlReelResist->setText( test::ToString( params.ControlReelResist() ) );
    }

    if ( params.ReelControl() == test::RC_CONTROL_BOX )
    {
        ui->MaxExpenditureA->setText( test::ToString( params.MaxExpenditureA() ) );
        if ( params.ReelCount() == 2)
            ui->MaxExpenditureB->setText( test::ToString( params.MaxExpenditureB() ) );

        ui->SignalStateA->setText( test::ToString( params.SignalStateA() ) );
        if ( params.ReelCount() == 2)
            ui->SignalStateB->setText( test::ToString( params.SignalStateB() ) );
        ui->SignalState0->setText( test::ToString( params.SignalState0() ) );
    }
}

void ServoTitleInfo::on_buttonBox_accepted()
{
    if ( SaveInputParams() )
    {
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
    ui->EndSgnal->setEnabled(true);

    ui->SignalStateA->setText("");
    ui->SignalStateB->setText("");
    ui->SignalState0->setText("");
    ui->EndSgnal->setText("");

    ui->SignalStateA->setToolTip( text );
    ui->SignalStateB->setToolTip( text );
    ui->SignalState0->setToolTip( text );
    ui->EndSgnal->setText("");

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
        case test::ST_40_40_mA:
            ui->SignalStateA->setValidator( new QDoubleValidator( -40, 40, 2, this ) );
            ui->SignalStateB->setValidator( new QDoubleValidator( -40, 40, 2, this ) );
            ui->SignalState0->setValidator( new QDoubleValidator( -40, 40, 2, this ) );
            break;
        case test::ST_100_mA:
            ui->EndSgnal->setValidator( new QDoubleValidator( 0, 100, 2, this ) );
            break;
        case test::ST_300_mA:
            ui->EndSgnal->setValidator( new QDoubleValidator( 0, 300, 2, this ) );
            break;
        case test::ST_600_mA:
            ui->EndSgnal->setValidator( new QDoubleValidator( 0, 600, 2, this ) );
            break;
        case test::ST_860_mA:
            ui->EndSgnal->setValidator( new QDoubleValidator( 0, 860, 2, this ) );
            break;
        default:
            ui->SignalStateA->setEnabled(false);
            ui->SignalStateB->setEnabled(false);
            ui->SignalState0->setEnabled(false);
            ui->EndSgnal->setEnabled(false);
            break;
    }
#warning неясна сутьба поля с амплитудами

}

void ServoTitleInfo::on_ControlType_activated(int index)
{
    test::RELL_CONTROL control = test::RC_UNKNOWN;
    auto text = ui->ControlType->itemText( index );
    test::ParseValue( control, text );

    ui->SignalStateA->setVisible( control == test::RC_CONTROL_BOX );
    ui->SignalStateB->setVisible( control == test::RC_CONTROL_BOX );
    ui->SignalState0->setVisible( control == test::RC_CONTROL_BOX );
    ui->MaxExpenditureA->setVisible( control == test::RC_CONTROL_BOX );
    ui->MaxExpenditureB->setVisible( control == test::RC_CONTROL_BOX );

    ui->l_signal_state_a->setVisible( control == test::RC_CONTROL_BOX );
    ui->l_signal_state_b->setVisible( control == test::RC_CONTROL_BOX );
    ui->l_signal_state_0->setVisible( control == test::RC_CONTROL_BOX );
    ui->l_max_expenditure_a->setVisible( control == test::RC_CONTROL_BOX );
    ui->l_max_expenditure_b->setVisible( control == test::RC_CONTROL_BOX );

    ui->EndSgnal->setVisible( control == test::RC_REEL );
    ui->l_end_signal->setVisible( control == test::RC_REEL );

    ui->ControlReelResist->setVisible( control == test::RC_REEL );
    ui->l_control_reel_resist->setVisible( control == test::RC_REEL );

    ui->ControlSignal->clear();
    if ( control == test::RC_CONTROL_BOX )
    {
        ui->ControlSignal->addItem( test::ToString( test::ST_0_20_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_4_20_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_10_10_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_40_40_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_10_10_V ) );
    }
    else if ( control == test::RC_REEL )
    {
        ui->ControlSignal->addItem( test::ToString( test::ST_100_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_300_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_600_mA ) );
        ui->ControlSignal->addItem( test::ToString( test::ST_860_mA ) );
    }
    ui->ControlSignal->setCurrentIndex( -1 );
    on_ControlSignal_activated( ui->ControlSignal->currentIndex() );


#warning сделать переключение между режимами интерфейса
///=ControlSignal
/// +sig_x_max
/// +Сопротивление катушки управления(где нет блока управления)
///=PressureTesting
///=PressureNominal
///=MaxExpenditure
///=ControlSignalAmplitude
///=FrequencyInc
///=RaspredControl
///=MinControlPressure
///=MaxControlPressure
///
/// -SignalStateA
/// -SignalStateB
/// -SignalState0
/// -MaxExpenditureA
/// -MaxExpenditureB
}
