#include "servo_title_info.h"
#include "ui_servo_title_info.h"
#include "test_case/tests.h"
#include "test_form.h"
#include "test_case/test_params.h"
#include <QMessageBox>

ServoTitleInfo::ServoTitleInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServoTitleInfo)
{
    ui->setupUi(this);
    ui->PressureNominal->setValidator( new QIntValidator( 100, 350, this ) );
    ui->MaxExpenditure->setValidator( new QIntValidator( 1, 1000, this ) );
    ui->MaxExpenditureA->setValidator( new QIntValidator( 1, 1000, this ) );
    ui->MaxExpenditureB->setValidator( new QIntValidator( 1, 1000, this ) );
    on_RaspredControl_activated( ui->RaspredControl->currentIndex() );
}

ServoTitleInfo::~ServoTitleInfo()
{
    delete ui;
}

bool ServoTitleInfo::SaveInputParams()
{
    test::servo::Parameters& params = test::servo::Parameters::Instance();
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
    res *= ParamChecker( ui->l_max_expenditure_a,    ValidateRange( ui->MaxExpenditureA, params.MaxExpenditureA( ui->MaxExpenditureA->text() ) ) );


    //определим используется ли вторая катушка
    bool p1 = params.MaxExpenditureB( ui->MaxExpenditureB->text() );
    bool p2 = false;
    if ( p1 || p2 )
        params.ReelCount( "2" );
    else
        params.ReelCount( "1" );

    if ( params.ReelCount() > 1  )
    {
        res *= ParamChecker( ui->l_max_expenditure_b,    ValidateRange( ui->MaxExpenditureB, p1 ) );
    }
    else
    {
        ParamChecker( ui->l_max_expenditure_b, true );
    }

    return res;
}

void ServoTitleInfo::on_buttonBox_accepted()
{
    if ( SaveInputParams() )
    {
        hide();
        if ( mChildWindow.get() )
            QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
        mChildWindow.reset( new TestForm( test::ServoTests ) );
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
