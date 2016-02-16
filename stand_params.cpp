#include "stand_params.h"
#include "ui_stand_params.h"
#include "test_form.h"
#include "test_case/test_params.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_hydro.h"


StandParams::StandParams(bool new_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandParams),
    mNewMode( new_mode )
{
    ui->setupUi(this);

    if ( test::hydro::Parameters::Instance().PosCount() < 2 )
    {
        ui->l_tr->setVisible( false );
        ui->DD1B_ON->setVisible( false );
        ui->DD2B_ON->setVisible( false );
        ui->DD3B_ON->setVisible( false );
        ui->OnDynamic_2->setVisible( false );

        ui->l_br->setVisible( false );
        ui->DD1B_OFF->setVisible( false );
        ui->DD2B_OFF->setVisible( false );
        ui->DD3B_OFF->setVisible( false );
        ui->OffDynamic_2->setVisible( false );
    }

    if (!mNewMode)
        FromParams();
}

StandParams::~StandParams()
{
    delete ui;
}

void StandParams::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
    if (mCustomAction)
        mCustomAction();
}

bool StandParams::SaveInputParams()
{
    test::hydro::Parameters& params = test::hydro::Parameters::Instance();

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


    res *= ParamChecker( ui->l_h1t,             params.OnDD1A ( ui->DD1A_ON->checkState() ) );
    res *= ParamChecker( ui->l_h1t,             params.OnDD2A ( ui->DD2A_ON->checkState() ) );
    res *= ParamChecker( ui->l_h1t,             params.OnDD3A ( ui->DD3A_ON->checkState() ) );
    res *= ParamChecker( ui->l_h2t,             params.OnDynamic_1 ( ui->OnDynamic_1->currentText() ) );

    res *= ParamChecker( ui->l_h1b,             params.OffDD1A ( ui->DD1A_OFF->checkState() ) );
    res *= ParamChecker( ui->l_h1b,             params.OffDD2A ( ui->DD2A_OFF->checkState() ) );
    res *= ParamChecker( ui->l_h1b,             params.OffDD3A ( ui->DD3A_OFF->checkState() ) );
    res *= ParamChecker( ui->l_h2b,             params.OffDynamic_1( ui->OffDynamic_1->currentText() ) );

    if ( params.PosCount() == 2 )
    {        
        res *= ParamChecker( ui->l_h1t,         params.OnDD1B ( ui->DD1B_ON->checkState() ) );
        res *= ParamChecker( ui->l_h1t,         params.OnDD2B ( ui->DD2B_ON->checkState() ) );
        res *= ParamChecker( ui->l_h1t,         params.OnDD3B ( ui->DD3B_ON->checkState() ) );
        res *= ParamChecker( ui->l_h2t,         params.OnDynamic_2 ( ui->OnDynamic_2->currentText() ) );

        res *= ParamChecker( ui->l_h1b,         params.OffDD1B ( ui->DD1B_OFF->checkState() ) );
        res *= ParamChecker( ui->l_h1b,         params.OffDD2B ( ui->DD2B_OFF->checkState() ) );
        res *= ParamChecker( ui->l_h1b,         params.OffDD3B ( ui->DD3B_OFF->checkState() ) );
        res *= ParamChecker( ui->l_h2b,         params.OffDynamic_2( ui->OffDynamic_2->currentText() ) );
    }


    return res;
}

void StandParams::FromParams()
{
    test::hydro::Parameters& params = test::hydro::Parameters::Instance();

    ui->DD1A_ON->setChecked( params.OnDD1A() );
    ui->DD2A_ON->setChecked( params.OnDD2A() );
    ui->DD3A_ON->setChecked( params.OnDD3A() );
    ui->OnDynamic_1->setCurrentIndex( ui->OnDynamic_1->findText( test::ToString( params.OnDynamic_1() ) ) );

    ui->DD1A_OFF->setChecked( params.OffDD1A() );
    ui->DD2A_OFF->setChecked( params.OffDD2A() );
    ui->DD3A_OFF->setChecked( params.OffDD3A() );
    ui->OffDynamic_1->setCurrentIndex( ui->OffDynamic_1->findText( test::ToString( params.OffDynamic_1() ) ) );

    ui->DD1B_ON->setChecked( params.OnDD1B() );
    ui->DD2B_ON->setChecked( params.OnDD2B() );
    ui->DD3B_ON->setChecked( params.OnDD3B() );
    ui->OnDynamic_2->setCurrentIndex( ui->OnDynamic_2->findText( test::ToString( params.OnDynamic_2() ) ) );

    ui->DD1B_OFF->setChecked( params.OffDD1B() );
    ui->DD2B_OFF->setChecked( params.OffDD2B() );
    ui->DD3B_OFF->setChecked( params.OffDD3B() );
    ui->OffDynamic_2->setCurrentIndex( ui->OffDynamic_2->findText( test::ToString( params.OffDynamic_2() ) ) );
}

void StandParams::on_buttonBox_accepted()
{
    if ( SaveInputParams() )
    {        
        hide();
        if ( mCustomAction )
            mCustomAction();
        else
        {
            if ( mChildWindow.get() )
                QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
            mChildWindow.reset( new TestForm( mNewMode ) );
            QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
            mChildWindow->show();
        }
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

void StandParams::on_buttonBox_rejected()
{
    close();
}

void StandParams::SetCallback( std::function< void() > func  )
{
    mCustomAction = func;
}
