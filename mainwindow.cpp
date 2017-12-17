#include "mainwindow.h"
#include "manual_control.h"
#include "ui_mainwindow.h"
#include "hydro_title_info.h"
#include "servo_title_info.h"
#include "control_panels_title_info.h"
#include "hydro_cilinder_title_info.h"
#include "pump_title_info.h"

#include "settings/settings.h"
#include "settings/settings_wnd.h"
#include <QFileDialog>
#include "devices/device_collection_wnd.h"
#include "users/users.h"

#include "test_case/implementation/test_params_hydro.h"
#include "test_case/implementation/test_params_servo.h"
#include "test_case/implementation/test_params_control_panel.h"
#include "test_case/implementation/test_params_hydro_cilinder.h"
#include "test_case/implementation/test_params_pumps.h"
#include "test_case/test.h"
#include "viewer.h"
#include "cpu/cpu_memory.h"
#include "test_case/implementation/test_base.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef DEMO
    setWindowTitle( windowTitle() + " DEMO" );
#endif

    ui->TestCase1->setText( test::hydro::Parameters::Instance().TestCollection().Name() );
    ui->TestCase1->setDescription( test::hydro::Parameters::Instance().TestCollection().Descr() );
    ui->act_test_case1->setToolTip( test::hydro::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case1->setText( test::hydro::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case1->setIconText( test::hydro::Parameters::Instance().TestCollection().Name() );


    ui->TestCase2->setText( test::servo::Parameters::Instance().TestCollection().Name() );
    ui->TestCase2->setDescription( test::servo::Parameters::Instance().TestCollection().Descr() );
    ui->act_test_case2->setToolTip( test::servo::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case2->setText( test::servo::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case2->setIconText( test::servo::Parameters::Instance().TestCollection().Name() );

    ui->TestCase3->setText( test::control_board::Parameters::Instance().TestCollection().Name() );
    ui->TestCase3->setDescription( test::control_board::Parameters::Instance().TestCollection().Descr() );
    ui->act_test_case3->setToolTip( test::control_board::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case3->setText( test::control_board::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case3->setIconText( test::control_board::Parameters::Instance().TestCollection().Name() );

    ui->TestCase4->setText( test::hydro_cylinder::Parameters::Instance().TestCollection().Name() );
    ui->TestCase4->setDescription( test::hydro_cylinder::Parameters::Instance().TestCollection().Descr() );
    ui->act_test_case4->setToolTip( test::hydro_cylinder::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case4->setText( test::hydro_cylinder::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case4->setIconText( test::hydro_cylinder::Parameters::Instance().TestCollection().Name() );

    ui->TestCase5->setText( test::pump::Parameters::Instance().TestCollection().Name() );
    ui->TestCase5->setDescription( test::pump::Parameters::Instance().TestCollection().Descr() );
    ui->act_test_case5->setToolTip( test::pump::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case5->setText( test::pump::Parameters::Instance().TestCollection().Name() );
    ui->act_test_case5->setIconText( test::pump::Parameters::Instance().TestCollection().Name() );


    auto addRoundControl = [&]( ff0x::RoundDial** control, QString const& name, QLayout* place)
    {
        *control = new ff0x::RoundDial(this);
        {
            ff0x::RoundDial* ptr = *control;
            ptr->setObjectName( name );
            ptr->SetAngle( 210 );
            ptr->SetRange( 0, 400 );
            ptr->SetStep( 10 );
            ptr->SetUnits("Бар.");
            QFont w_font = font();
            w_font.setPointSize( 14 );
            ptr->setFont( w_font );
            ff0x::RoundDial::RangeList green_zone;
            green_zone.push_back( QPointF(10, 315) );
            ff0x::RoundDial::RangeList yelow_zone;
            yelow_zone.push_back( QPointF(0, 10) );
            ff0x::RoundDial::RangeList red_zone;
            red_zone.push_back( QPointF(315, 400) );


            ptr->SetGreenRanges( green_zone );
            ptr->SetYellowRanges( yelow_zone );
            ptr->SetRedRanges( red_zone );

            ptr->EnableSaffetyRanges();

            ptr->setMinimumSize( 200, 170 );
            ptr->setMaximumSize( 200, 170 );

            ptr->setSizePolicy( QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding );
            place->addWidget( ptr );
        }
    };

    addRoundControl( &DD1, "DD1", ui->p_dd1);   //A BP3
    addRoundControl( &DD2, "DD2", ui->p_dd2);   //B BP4
    addRoundControl( &DD3, "DD3", ui->p_dd3);   //P BP5

    addRoundControl( &M1M2, "M1M2", ui->p_m1m2);    //M1M2 BP1
    addRoundControl( &T, "T", ui->p_T);             //T BP2
    addRoundControl( &X, "X", ui->p_X);         //x BP6

    auto addRoundControl2 = [&]( ff0x::RoundDial** control, QString const& name, QLayout* place)
    {
        *control = new ff0x::RoundDial(this);
        {
            ff0x::RoundDial* ptr = *control;
            ptr->setObjectName( name );
            ptr->SetAngle( 210 );
            ptr->SetRange( 0, 700 );
            ptr->SetStep( 10 );
            ptr->SetUnits("л/мин");
            QFont w_font = font();
            w_font.setPointSize( 14 );
            ptr->setFont( w_font );

            ptr->EnableSaffetyRanges( false );

            ptr->setMinimumSize( 200, 170 );
            ptr->setMaximumSize( 200, 170 );

            ptr->setSizePolicy( QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding );
            place->addWidget( ptr );
        }
    };
    auto addRoundControl3 = [&]( ff0x::RoundDial** control, QString const& name, QLayout* place)
    {
        *control = new ff0x::RoundDial(this);
        {
            ff0x::RoundDial* ptr = *control;
            ptr->setObjectName( name );
            ptr->SetAngle( 210 );
            ptr->SetRange( 0, 14 );
            ptr->SetStep( 0.2 );
            ptr->SetUnits("л/мин");
            QFont w_font = font();
            w_font.setPointSize( 14 );
            ptr->setFont( w_font );

            ff0x::RoundDial::RangeList green_zone;
            green_zone.push_back( QPointF(0, 10) );
            ff0x::RoundDial::RangeList red_zone;
            red_zone.push_back( QPointF(10, 14) );


            ptr->SetGreenRanges( green_zone );
            ptr->SetRedRanges( red_zone );

            ptr->EnableSaffetyRanges( true );

            ptr->setMinimumSize( 200, 170 );
            ptr->setMaximumSize( 200, 170 );

            ptr->setSizePolicy( QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding );
            place->addWidget( ptr );
        }
    };
    addRoundControl2( &R_P, "R_P", ui->p_p);
    addRoundControl3( &R_Lost, "R_Lost", ui->p_lost);

    QObject::connect( &Updater, SIGNAL(update()), this, SLOT(onUpdateControls()) );
    Updater.start();
}
void MainWindow::CheckRights()
{
   ui->Users->setEnabled( true );
   ui->ManualControl->setEnabled( true );
   ui->TestCase1->setEnabled( true );
   ui->TestCase2->setEnabled( true );
   ui->TestCase3->setEnabled( true );
   ui->TestCase4->setEnabled( true );
   ui->menu_3->setEnabled( true );
   ui->menu_4->setEnabled( true );
   ui->centralWidget->setEnabled( true );

   if ( app::Settings::Instance().UserAccess() != app::Admin )
   {
      ui->Users->setEnabled( false );
      if ( app::Settings::Instance().UserAccess() != app::Master )
      {
         ui->ManualControl->setEnabled( false );
         ui->TestCase1->setEnabled( false );
         ui->TestCase2->setEnabled( false );
         ui->TestCase3->setEnabled( false );
         ui->TestCase4->setEnabled( false );
         ui->menu_3->setEnabled( false );
         ui->menu_4->setEnabled( false );

         if ( app::Settings::Instance().UserAccess() != app::User )
         {
            ui->centralWidget->setEnabled( false );
         }
      }
   }
}

void MainWindow::onLogin()
{
   CheckRights();
   ui->statusBar->showMessage("Текущий пользователь: " + app::Settings::Instance().User() );
}

void MainWindow::showEvent( QShowEvent *e )
{
    QMainWindow::showEvent( e );
}

MainWindow::~MainWindow()
{
    Updater.stop();
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

void MainWindow::ShowChildWindow( std::unique_ptr< QWidget > child, bool maximized )
{
    enable_modes(false);
    if ( mChildWindow.get() )
        QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(enable_modes()) );
    mChildWindow.reset( child.release() );
    QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(enable_modes()) );

    if ( maximized )
        mChildWindow->showMaximized();
    else
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
void MainWindow::StartControlPanelsTest( bool new_test )
{
    ShowChildWindow( ChildPtr( new ControlPanelsTitleInfo( new_test ) ) );
}
void MainWindow::StartHydroCilinderTest( bool new_test )
{
    ShowChildWindow( ChildPtr( new HydroCilinderTitleInfo( new_test ) ) );
}
void MainWindow::StartHydroPumpsTest( bool new_test )
{
   ShowChildWindow( ChildPtr( new PumpTitleInfo( new_test ) ) );
}

void MainWindow::AppSettrings ()
{
    ShowChildWindow( ChildPtr( new settings_wnd() ) );
}
void MainWindow::DeviceLists( examinee::DeviceCollection& devices )
{
    ShowChildWindow( ChildPtr( new DeviceCollectionWND( devices ) ) );
}


void MainWindow::enable_modes(bool enabled)
{
    ui->act_test_case1->setEnabled( enabled );
    ui->act_test_case2->setEnabled( enabled );
    ui->act_test_case3->setEnabled( enabled );
    ui->act_test_case4->setEnabled( enabled );
    ui->ManualControl->setEnabled( enabled );

    ui->TestCase1->setEnabled( enabled );
    ui->TestCase2->setEnabled( enabled );
    ui->TestCase3->setEnabled( enabled );
    ui->TestCase4->setEnabled( enabled );

    ui->app_settings->setEnabled( enabled );
    ui->device_list->setEnabled( enabled );

    ui->hydro_list->setEnabled( enabled );
    ui->servo_list->setEnabled( enabled );

    ui->load_isp_params->setEnabled( enabled );

    CheckRights();
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

void MainWindow::on_act_test_case3_triggered()
{
    on_TestCase3_clicked();
}
void MainWindow::on_TestCase3_clicked()
{
    StartControlPanelsTest( true );
}

void MainWindow::on_act_test_case4_triggered()
{
    on_TestCase4_clicked();
}
void MainWindow::on_TestCase4_clicked()
{
    StartHydroCilinderTest( true );
}

void MainWindow::on_act_test_case5_triggered()
{
   on_TestCase5_clicked();
}
void MainWindow::on_TestCase5_clicked()
{
    StartHydroPumpsTest( true );
}

void MainWindow::on_ManualControl_clicked()
{
    ShowChildWindow( ChildPtr( new ManualControl() ) );
}

//ACTIONS
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

    test::Parameters* ptr = test::ParamsFromFile( file_name );

    if ( ptr == &test::hydro::Parameters::Instance() )
    {
        StartHydroTest( false );
    }
    else if ( ptr == &test::servo::Parameters::Instance() )
    {
        StartServoTest( false );
    }
    else if ( ptr == &test::control_board::Parameters::Instance() )
    {
        StartControlPanelsTest( false );
    }
    else if ( ptr == &test::hydro_cylinder::Parameters::Instance() )
    {
        StartHydroCilinderTest( false );
    }
    else if ( ptr == &test::pump::Parameters::Instance() )
    {
       StartHydroPumpsTest( false );
    }
}

void MainWindow::on_app_settings_triggered()
{
    AppSettrings();
}

void MainWindow::on_hydro_list_triggered()
{
    DeviceLists( test::hydro::Parameters::Instance().TestCollection().Devices() );
}
void MainWindow::on_servo_list_triggered()
{
    DeviceLists( test::servo::Parameters::Instance().TestCollection().Devices() );
}
void MainWindow::on_control_panels_list_triggered()
{
    DeviceLists( test::control_board::Parameters::Instance().TestCollection().Devices() );
}
void MainWindow::on_LastTest_triggered()
{
    ShowChildWindow( ChildPtr( new Viewer() ) );
}

void MainWindow::on_Open_results_triggered()
{
    QString file_name;
    QFileDialog dlg;
    dlg.setFileMode( QFileDialog::ExistingFile );
    dlg.setDirectory( app::Settings::Instance().TestPath() );
    dlg.setNameFilter( "Результаты испытаний (*.res )" );
    dlg.setViewMode( QFileDialog::Detail );
    if ( dlg.exec() )
    {
        file_name = dlg.selectedFiles().front();
        test::DataFromFile( file_name );
        on_LastTest_triggered();
    }
}

void MainWindow::on_Users_triggered()
{
    ShowChildWindow( ChildPtr( new Users() ) );
}

void MainWindow::onUpdateControls()
{
    auto& table = cpu::CpuMemory::Instance().DB50;
    auto& table2 = cpu::CpuMemory::Instance().I1;

    DD1->update_value( round( table.BP3 *100)/100 );
    DD2->update_value( round( table.BP4 *100)/100 );
    DD3->update_value( round( table.BP5 *100)/100 );
    M1M2->update_value( round( table.BP1 *100)/100 );
    T->update_value( round( table.BP2 *100)/100 );
    X->update_value( round( table.BP6 *100)/100 );

    float lost = table.BV4;
    if ( lost < 1.9 )
        lost = table.BV3;

    R_Lost->update_value( round( lost *100)/100 );

    float exp_t = table.BV2;
    if ( exp_t < 60.0 )
        exp_t = table.BV1;
    R_P->update_value( round( exp_t *100)/100 );

    float voltage = table.V0_300B;
//    if ( voltage == 60.0 )
//        voltage = table.V0_300B;
    ui->Voltage->display( round( voltage *100)/100 );

    float amperage_dc = table.A1;  //постоянный ток
    ui->Amperage->display(round( amperage_dc *100)/100);

    float amperage_ac = table.A2;  //переменный ток
//    ui->Amperage->display(amperage_dc);

    //температура масла
    ui->OilTemp->display( round( table.BT1 *100)/100 );

    //уровень масла
    QColor c = Qt::red;
    bool err = false;
    if ( table2.SL1 )
        c = Qt::green;
    else if ( table2.SL2 )
        c = Qt::yellow;
    else if ( table2.SL3 )
        c = Qt::yellow;

    err = !table2.SL1 && !table2.SL2 && !table2.SL3;
    UpdateMark( ui->SL1, table2.SL1, c );     //IX4.0    уровень масла - норма
    UpdateMark( ui->SL2, table2.SL2, c );     //IX4.1    уровень масла -  предупреждение
    UpdateMark( ui->SL3, table2.SL3, c );     //IX4.2    уровень масла - авария

    if (err)
    {
        UpdateMark( ui->SL1, err, c );     //IX4.0    уровень масла - норма
        UpdateMark( ui->SL2, err, c );     //IX4.1    уровень масла -  предупреждение
        UpdateMark( ui->SL3, err, c );     //IX4.2    уровень масла - авария
    }
    //см DB40

    //готовность к работе
    bool ready = true;

    ready *= table.ReadyToWork;
    ready *= table.BT1 > 25.0;     //температура масла должна быть больше 25

    QString st_err = test::ErrMsg();
    if ( table.BT1 <= 25.0 )
        st_err += "холодное масло\n";
    UpdateMark( ui->l_Ready_To_Work, true , ready ? Qt::green : Qt::red );

    if ( LastErrs != st_err )
    {
        LastErrs = st_err;
        ui->l_Ready_To_Work->setToolTip( LastErrs );
    }
}


ControlsUpdater::ControlsUpdater():
    mStopSignal(false)
{}
void ControlsUpdater::run()
{    
    mStopSignal = false;
    while ( !mStopSignal )
    {
        cpu::CpuMemory::Instance().DB50.Read();
        cpu::CpuMemory::Instance().I1.Read();
        emit update();
        msleep(500);
    }
}
void ControlsUpdater::stop()
{
    mStopSignal = true;
    wait();
}
