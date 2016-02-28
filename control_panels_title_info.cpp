#include "control_panels_title_info.h"
#include "ui_control_panels_title_info.h"
#include "test_form.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_control_panel.h"
#include "settings/settings.h"

ControlPanelsTitleInfo::ControlPanelsTitleInfo(bool new_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlPanelsTitleInfo),
    mNewMode( new_mode )
{
    ui->setupUi(this);

    foreach ( examinee::Device const* device, test::control_board::Parameters::Instance().TestCollection().Devices().Get() )
    {
        ui->GsType->addItem( device->Name() + " " + device->Manufacturer(), QVariant::fromValue( device ) );
    }
    ui->GsType->setCurrentIndex( -1 );

    ui->Voltage->setValidator( new QDoubleValidator( 1, 40, 2, this ) );
    ui->MaxAmperage->setValidator( new QDoubleValidator( 0, 3, 3, this ) );

    if ( !mNewMode )
        FromParams();

    CheckRights();
}

ControlPanelsTitleInfo::~ControlPanelsTitleInfo()
{
    delete ui;
}

void ControlPanelsTitleInfo::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}

bool ControlPanelsTitleInfo::SaveInputParams()
{
    test::control_board::Parameters& params = test::control_board::Parameters::Instance();
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

    res *= ParamChecker( ui->l_gs_type, params.GsType( ui->GsType->currentText() ) );
    res *= ParamChecker( ui->l_ser_no, params.SerNo( ui->SerNo->text() ) );
    res *= ParamChecker( ui->l_signal_type,  params.SignalType( ui->SignalType->currentText() ) );
    res *= ParamChecker( ui->l_voltage,      ValidateRange( ui->Voltage, params.Voltage( ui->Voltage->text() ) ) );
    res *= ParamChecker( ui->l_max_amperage, ValidateRange( ui->MaxAmperage, params.MaxAmperage( ui->MaxAmperage->text() ) ) );
    res *= ParamChecker( ui->l_reel_resist,  params.ReelResist( ui->ReelResist->currentText() ) );

    return res;
}
void ControlPanelsTitleInfo::FromParams()
{
    test::control_board::Parameters& params = test::control_board::Parameters::Instance();

    ui->GsType->setCurrentIndex( ui->GsType->findText( params.GsType() ) );
    ui->SerNo->setText( params.SerNo() );
    ui->SignalType->setCurrentIndex( ui->SignalType->findText( test::ToString( params.SignalType() ) ) );
    ui->Voltage->setText( test::ToString( params.Voltage() ) );
    ui->MaxAmperage->setText( test::ToString( params.MaxAmperage() ) );
    ui->ReelResist->setCurrentIndex( ui->ReelResist->findText( test::ToString( params.ReelResist() ) ) );
}

void ControlPanelsTitleInfo::on_buttonBox_accepted()
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

void ControlPanelsTitleInfo::on_buttonBox_rejected()
{
    close();
}

void ControlPanelsTitleInfo::CheckRights()
{
    if ( app::Settings::Instance().UserAccess() == app::User )
    {
        ui->GsType->setEnabled( false );
        ui->SerNo->setEnabled( false );
        ui->SignalType->setEnabled( false );
        ui->Voltage->setEnabled( false );
        ui->MaxAmperage->setEnabled( false );
        ui->ReelResist->setEnabled( false );
    }
}
