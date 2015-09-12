#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test_case/tests.h"
#include "hydro_title_info.h"
#include "servo_title_info.h"
#include "test_case/test_params.h"
#include "settings/settings_wnd.h"
#include <QFileDialog>
#include "settings/settings.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->TestCase1->setText( test::HydroTests.Name() );
    ui->TestCase1->setDescription( test::HydroTests.Descr() );
    ui->act_test_case1->setToolTip( test::HydroTests.Name() );
    ui->act_test_case1->setText( test::HydroTests.Name() );
    ui->act_test_case1->setIconText( test::HydroTests.Name() );


    ui->TestCase2->setText( test::ServoTests.Name() );
    ui->TestCase2->setDescription( test::ServoTests.Descr() );
    ui->act_test_case2->setToolTip( test::ServoTests.Name() );
    ui->act_test_case2->setText( test::ServoTests.Name() );
    ui->act_test_case2->setIconText( test::ServoTests.Name() );



    ui->ManualControl->setText( "Ручное управление" );
    ui->ManualControl->setDescription( "Запуск стенда в режиме ручного управления для администрирования и обслуживания" );

    auto addRoundControl = [&]( ff0x::RoundDial** control, QString const& name, QLayout* place)
    {
        *control = new ff0x::RoundDial(this);
        {
            ff0x::RoundDial* ptr = *control;
            ptr->setObjectName( name );
            ptr->SetAngle( 210 );
            ptr->SetRange( 0, 500 );
            ptr->SetStep( 10 );
            ptr->SetUnits("Бар.");
            QFont w_font = font();
            w_font.setPointSize( w_font.pointSize() - 2 );
            ptr->setFont( w_font );
            ff0x::RoundDial::RangeList green_zone;
            green_zone.push_back( QPointF(100, 400) );
            ff0x::RoundDial::RangeList yelow_zone;
            yelow_zone.push_back( QPointF(50, 100) );
            yelow_zone.push_back( QPointF( 400, 450) );
            ff0x::RoundDial::RangeList red_zone;
            red_zone.push_back( QPointF(0, 50) );
            red_zone.push_back( QPointF( 450, 500) );

            ptr->SetGreenRanges( green_zone );
            ptr->SetYellowRanges( yelow_zone );
            ptr->SetRedRanges( red_zone );

            ptr->EnableSaffetyRanges();

            ptr->setMinimumSize( 200, 170 );
            ptr->setMaximumSize( 200, 170 );

            ptr->setSizePolicy( QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding );
            place->addWidget( ptr );

            ptr->update_value( 101 );
        }
    };

    addRoundControl( &DD1, "DD1", ui->p_dd1);
    addRoundControl( &DD2, "DD2", ui->p_dd2);
    addRoundControl( &DD3, "DD3", ui->p_dd3);

    addRoundControl( &M1M2, "M1M2", ui->p_m1m2);
    addRoundControl( &T, "T", ui->p_T);
    addRoundControl( &X, "X", ui->p_X);

    auto addRoundControl2 = [&]( ff0x::RoundDial** control, QString const& name, QLayout* place)
    {
        *control = new ff0x::RoundDial(this);
        {
            ff0x::RoundDial* ptr = *control;
            ptr->setObjectName( name );
            ptr->SetAngle( 210 );
            ptr->SetRange( -100, 100 );
            ptr->SetStep( 5 );
            ptr->SetUnits("л/мин");
            QFont w_font = font();
            w_font.setPointSize( w_font.pointSize() - 2 );
            ptr->setFont( w_font );

            ptr->EnableSaffetyRanges( false );

            ptr->setMinimumSize( 200, 170 );
            ptr->setMaximumSize( 200, 170 );

            ptr->setSizePolicy( QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding );
            place->addWidget( ptr );

            ptr->update_value( 10 );
        }
    };
    addRoundControl2( &R_P, "R_P", ui->p_p);
    addRoundControl2( &R_Lost, "R_Lost", ui->p_lost);
    addRoundControl2( &R_Cilindr, "R_Cilindr", ui->p_c);
}

MainWindow::~MainWindow()
{
    DD1->deleteLater();
    DD2->deleteLater();
    DD3->deleteLater();

    M1M2->deleteLater();
    T->deleteLater();
    X->deleteLater();

    R_P->deleteLater();
    R_Lost->deleteLater();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    mChildWindow.reset();
    QWidget::closeEvent( e );
}

void MainWindow::ShowChildWindow( std::unique_ptr< QWidget > child )
{
    enable_modes(false);
    if ( mChildWindow.get() )
        QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
    mChildWindow.reset( child.release() );
    QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(enable_modes()) );
    mChildWindow->show();
}

void MainWindow::StartHydroTest( bool new_test )
{
    ShowChildWindow( ChildPtr( new HydroTitleInfo( new_test ) ) );
}
void MainWindow::StartServoTest( bool new_test )
{
    ShowChildWindow( ChildPtr( new ServoTitleInfo( new_test ) ) );
}
void MainWindow::StartManualControl()
{

}
void MainWindow::SettringsFileStorage ()
{
    ShowChildWindow( ChildPtr( new settings_wnd() ) );
}

void MainWindow::enable_modes(bool enabled)
{
    ui->act_test_case1->setEnabled( enabled );
    ui->act_test_case2->setEnabled( enabled );
    ui->TestCase1->setEnabled( enabled );
    ui->TestCase2->setEnabled( enabled );
    ui->ManualControl->setEnabled( enabled );
    ui->file_storage->setEnabled( enabled );
}

void MainWindow::on_act_test_case1_triggered()
{
    on_TestCase1_clicked();
}
void MainWindow::on_TestCase1_clicked()
{
    StartHydroTest( true );
}

void MainWindow::on_act_test_case2_triggered()
{
    on_TestCase2_clicked();
}
void MainWindow::on_TestCase2_clicked()
{
    StartServoTest( true );
}

void MainWindow::on_ManualControl_clicked()
{
    StartManualControl();
}

void MainWindow::on_load_isp_params_triggered()
{
    QString file_name;
    QFileDialog dlg;
    dlg.setFileMode( QFileDialog::ExistingFile );
    dlg.setDirectory( app::Settings::Instance().TestPath() );
    dlg.setNameFilter( "Параметры испытаний (*.isx )" );
    dlg.setViewMode( QFileDialog::Detail );
    if ( dlg.exec() )
        file_name = dlg.selectedFiles().front();

    test::Parameters* ptr = test::FromFile( file_name );

    if ( ptr == &test::hydro::Parameters::Instance() )
    {
        StartHydroTest( false );
    }
    else if ( ptr == &test::servo::Parameters::Instance() )
    {
        StartServoTest( false );
    }
}

void MainWindow::on_file_storage_triggered()
{
    SettringsFileStorage();
}
