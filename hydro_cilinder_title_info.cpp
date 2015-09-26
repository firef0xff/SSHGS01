#include "hydro_cilinder_title_info.h"
#include "ui_hydro_cilinder_title_info.h"
#include "test_form.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_hydro_cilinder.h"

HydroCilinderTitleInfo::HydroCilinderTitleInfo(bool new_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HydroCilinderTitleInfo),
    mNewMode( new_mode )
{
    ui->setupUi(this);

    ui->MaxPressure->setValidator( new QIntValidator( 10, 315, this ) );
    ui->MoveTime->setValidator( new QDoubleValidator( 0.1, 10, 3, this ) );
    ui->Expenditure->setValidator( new QIntValidator( 1, 300, this ) );

    if ( !mNewMode )
        FromParams();
}

HydroCilinderTitleInfo::~HydroCilinderTitleInfo()
{
    delete ui;
}

void HydroCilinderTitleInfo::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}

bool HydroCilinderTitleInfo::SaveInputParams()
{
    test::hydro_cylinder::Parameters& params = test::hydro_cylinder::Parameters::Instance();
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
    ui->MaxPressure->setText( test::ToString( params.MaxPressure() ) );
    ui->MoveTime->setText( test::ToString( params.MoveTime() ) );
    ui->TestPressure->setValue( params.TestPressure() );
    ui->HermTestTime->setValue( params.HermTestTime() );
    ui->Expenditure->setText( test::ToString( params.Expenditure() ) );
}

void HydroCilinderTitleInfo::on_buttonBox_accepted()
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

void HydroCilinderTitleInfo::on_buttonBox_rejected()
{
    close();
}
