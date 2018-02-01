#include "pumps_manual_control.h"
#include "ui_pumps_manual_control.h"

PumpsManualControlUpdater::PumpsManualControlUpdater():
    mStopSignal(false)
{}
void PumpsManualControlUpdater::run()
{
    mStopSignal = false;
    while ( !mStopSignal )
    {
        cpu::CpuMemory::Instance().DB70.Read();
        emit update();
        msleep(500);
    }
}
void PumpsManualControlUpdater::stop()
{
    mStopSignal = true;
    wait();
}


PumpsManualControl::PumpsManualControl(QWidget *parent) :
   QWidget(parent),
   mTaskMode( cpu::CpuMemory::Instance().DB31 ),
   ui(new Ui::pumps_manual_control)
{
   ui->setupUi(this);

   QObject::connect( &mUpdater, &PumpsManualControlUpdater::update, this, &PumpsManualControl::onUpdateControls );
}

PumpsManualControl::~PumpsManualControl()
{
   mUpdater.stop();
   QObject::disconnect( &mUpdater, &PumpsManualControlUpdater::update, this, &PumpsManualControl::onUpdateControls );
   delete ui;
}


void PumpsManualControl::onUpdateControls()
{
   SynkControls();
   UpdateData();
   UpdateMarks();
}

void PumpsManualControl::UpdateMarks()
{
   auto &mem = cpu::CpuMemory::Instance().DB73;
   if( mem.SP16_warning )
      SetColor( ui->px_ln_4_2, Qt::red ); //16
   if( mem.SP17_warning )
      SetColor( ui->px_ln_7_2, Qt::red ); //17
   if( mem.SP18_warning )
      SetColor( ui->px_ln_9_2, Qt::red ); //18
   if( mem.SP25_warning )
      SetColor( ui->px_ln_11_2, Qt::red ); //25
   if( mem.SP24_warning )
      SetColor( ui->px_ln_12_2, Qt::red ); //24
   if( mem.SP22_warning )
      SetColor( ui->px_ln_10_4, Qt::red ); //22
   if( mem.SP19_warning )
      SetColor( ui->px_ln_16_1, Qt::red ); //19
}
void PumpsManualControl::UpdateData()
{
   auto & mem = cpu::CpuMemory::Instance().DB70;
   ui->BP7->display( mem.BP7 );
   ui->BP8->display( mem.BP8 );
   ui->BP9->display( mem.BP9 );
   ui->BP10->display( mem.BP10 );
   ui->BP12->display( mem.BP12 );
   ui->BP13->display( mem.BP13 );

   ui->BV5->display( mem.BV5 );
   ui->BV6->display( mem.BV6 );
   ui->BV7->display( mem.BV7 );

   ui->BV8910->display( max( max( mem.BV8, mem.BV9 ), mem.BV10 ) );

   ui->DM1->display( mem.DM1 );
   ui->DR1->display( mem.DR1 );

   ui->BT2->display( mem.BT2 );
   ui->BT3->display( mem.BT3 );

}
void PumpsManualControl::SynkControls()
{
   on_YA27_clicked();
   on_YB7_clicked();
   on_YB8_clicked();
   on_YB9_clicked();
   on_YB10_clicked();
   on_YA28_clicked();
}
void PumpsManualControl::UpdateButton( QAbstractButton *btn, bool checked )
{
    btn->setChecked( checked );
}

void PumpsManualControl::Start()
{
//   cpu::CpuMemory::Instance().DB71.Reset();

   mTaskMode.N_Operation = 101;
   mTaskMode.Nasos_M2 = false;
   mTaskMode.OP15_25_Continum = false;
   mTaskMode.Q_5_5ma = 80.0;
   mTaskMode.Start_Oper = false;
   mTaskMode.Stop_Oper = false;
   mTaskMode.Write();
   SynkControls();
   mUpdater.start();
}
void PumpsManualControl::Stop()
{
   mUpdater.stop();
//   mControlBits.Reset();
//   cpu::CpuMemory::Instance().DB71.Reset();
   mTaskMode.N_Operation = 0;
   mTaskMode.Nasos_M2 = false;
   mTaskMode.OP15_25_Continum = false;
   mTaskMode.Q_5_5ma = 80.0;
   mTaskMode.Start_Oper = false;
   mTaskMode.Stop_Oper = false;
   mTaskMode.Write();
}

void PumpsManualControl::closeEvent(QCloseEvent *e)
{
    Stop();
    QWidget::closeEvent( e );
}
void PumpsManualControl::showEvent( QShowEvent *e )
{
    Start();
    QWidget::showEvent( e );
}
void PumpsManualControl::hideEvent( QHideEvent *e )
{
    QWidget::hideEvent( e );
}


void PumpsManualControl::SetColor( QLabel *label, QColor cl )
{
   QString sh = label->styleSheet();
   if ( sh.isEmpty() )
      return;

   static const QString pref_def = "img/";
   static const QString pref_r = "img/r_";
   static const QString pref_b = "img/b_";

   if ( cl == Qt::red )
   {
      if ( sh.contains( pref_r ) )
         return;
      else if ( sh.contains( pref_b ) )
         sh = sh.replace( pref_b, pref_r );
      else
         sh = sh.replace( pref_def, pref_r );
   }
   if ( cl == Qt::blue )
   {
      if ( sh.contains( pref_b ) )
         return;
      else if ( sh.contains( pref_r ) )
         sh = sh.replace( pref_r, pref_b );
      else
         sh = sh.replace( pref_def, pref_b );
   }
   if ( cl == Qt::black)
   {
      if ( sh.contains( pref_b ) )
         sh = sh.replace( pref_b, pref_def );
      else if ( sh.contains( pref_r ) )
         sh = sh.replace( pref_r, pref_def );
      else
         return;
   }
   label->setStyleSheet( sh );

}

void PumpsManualControl::Activate_Gm( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_gm_0, c );
   SetColor( ui->px_gm_1, c );
   SetColor( ui->px_gm_2, c );
}
void PumpsManualControl::Activate_M13( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_m13_0, c );
   SetColor( ui->px_m13_1, c );
   SetColor( ui->px_m13_2, c );
}

void PumpsManualControl::Activate_LN_0( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_0_0, c );
   SetColor( ui->px_ln_0_1, c );
   SetColor( ui->px_ln_0_2, c );
}
void PumpsManualControl::Activate_LN_1( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_1_0, c );
   SetColor( ui->px_ln_1_1, c );
   SetColor( ui->px_ln_1_2, c );
   SetColor( ui->px_ln_1_3, c );
}
void PumpsManualControl::Activate_LN_2( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_2_0, c );
   SetColor( ui->px_ln_2_1, c );
}

void PumpsManualControl::Activate_LN_4( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_4_0, c );
   SetColor( ui->px_ln_4_1, c );
   SetColor( ui->px_ln_4_2, c );
   SetColor( ui->px_ln_4_3, c );
   SetColor( ui->px_ln_4_4, c );
   SetColor( ui->px_ln_4_5, c );
   SetColor( ui->px_ln_4_6, c );
   SetColor( ui->px_ln_4_7, c );
   SetColor( ui->px_ln_4_8, c );
   SetColor( ui->px_ln_4_9, c );
   SetColor( ui->px_ln_4_10, c );
}
void PumpsManualControl::Activate_LN_5( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_5_0, c );
   SetColor( ui->px_ln_5_1, c );
}
void PumpsManualControl::Activate_LN_6( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_6_0, c );
   SetColor( ui->px_ln_6_1, c );
   SetColor( ui->px_ln_6_2, c );
   SetColor( ui->px_ln_6_3, c );
   SetColor( ui->px_ln_6_4, c );
   SetColor( ui->px_ln_6_5, c );
}
void PumpsManualControl::Activate_LN_7( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_7_0, c );
   SetColor( ui->px_ln_7_1, c );
   SetColor( ui->px_ln_7_2, c );
   SetColor( ui->px_ln_7_3, c );
   SetColor( ui->px_ln_7_4, c );
   SetColor( ui->px_ln_7_5, c );
   SetColor( ui->px_ln_7_6, c );
   SetColor( ui->px_ln_7_7, c );
   SetColor( ui->px_ln_7_8, c );
   SetColor( ui->px_ln_7_9, c );
}
void PumpsManualControl::Activate_LN_8( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_8_0, c );
   SetColor( ui->px_ln_8_1, c );
   SetColor( ui->px_ln_8_2, c );
}
void PumpsManualControl::Activate_LN_9( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_9_0, c );
   SetColor( ui->px_ln_9_1, c );
   SetColor( ui->px_ln_9_2, c );
   SetColor( ui->px_ln_9_3, c );
   SetColor( ui->px_ln_9_4, c );
   SetColor( ui->px_ln_9_5, c );
   SetColor( ui->px_ln_9_6, c );
   SetColor( ui->px_ln_9_7, c );
   SetColor( ui->px_ln_9_8, c );
   SetColor( ui->px_ln_9_9, c );
}
void PumpsManualControl::Activate_LN_10( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_10_0, c );
   SetColor( ui->px_ln_10_1, c );
   SetColor( ui->px_ln_10_2, c );
   SetColor( ui->px_ln_10_3, c );
   SetColor( ui->px_ln_10_4, c );
   SetColor( ui->px_ln_10_5, c );
}
void PumpsManualControl::Activate_LN_11( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_11_0, c );
   SetColor( ui->px_ln_11_1, c );
   SetColor( ui->px_ln_11_2, c );
   SetColor( ui->px_ln_11_3, c );
   SetColor( ui->px_ln_11_4, c );
   SetColor( ui->px_ln_11_5, c );
   SetColor( ui->px_ln_11_6, c );
   SetColor( ui->px_ln_11_7, c );
}
void PumpsManualControl::Activate_LN_12( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_12_0, c );
   SetColor( ui->px_ln_12_1, c );
   SetColor( ui->px_ln_12_2, c );
   SetColor( ui->px_ln_12_3, c );
   SetColor( ui->px_ln_12_4, c );
   SetColor( ui->px_ln_12_5, c );
}
void PumpsManualControl::Activate_LN_13( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_13_0, c );
   SetColor( ui->px_ln_13_1, c );
   SetColor( ui->px_ln_13_2, c );
}
void PumpsManualControl::Activate_LN_14( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_14_0, c );
   SetColor( ui->px_ln_14_1, c );
}
void PumpsManualControl::Activate_LN_15( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_15_0, c );
   SetColor( ui->px_ln_15_1, c );
   SetColor( ui->px_ln_15_2, c );
   SetColor( ui->px_ln_15_3, c );
   SetColor( ui->px_ln_15_4, c );
   SetColor( ui->px_ln_15_5, c );
}
void PumpsManualControl::Activate_LN_16( bool active )
{
   QColor c = active? Qt::blue: Qt::black;
   SetColor( ui->px_ln_16_0, c );
   SetColor( ui->px_ln_16_1, c );
   SetColor( ui->px_ln_16_2, c );
   SetColor( ui->px_ln_16_3, c );
   SetColor( ui->px_ln_16_4, c );
}

void PumpsManualControl::OnYA27YB7Activate()
{
    auto &mem = cpu::CpuMemory::Instance().M1;
    bool act_ya27 = mem.YA27;
    bool act_yb7 = ui->YB7->isChecked();
    bool main_line = act_ya27 || act_yb7;
    Activate_Gm( main_line );
    Activate_LN_0( main_line );
    Activate_LN_2( main_line );
    Activate_LN_4( main_line );
    Activate_LN_5( act_yb7 );
    Activate_LN_6( act_ya27 );
    Activate_LN_8( main_line );
    Activate_LN_10( main_line );

    Activate_LN_11( main_line );
    Activate_LN_12( main_line );
}

void PumpsManualControl::on_YA27_clicked()
{
   OnYA27YB7Activate();
}
void PumpsManualControl::on_YB7_clicked()
{
   OnYA27YB7Activate();
}
void PumpsManualControl::on_YB8_clicked()
{
   auto &mem = cpu::CpuMemory::Instance().M1;
   bool act_yb8 = ui->YB8->isChecked();
   bool main_line = act_yb8;
   bool glob_line = act_yb8 || ui->YB9->isChecked() || ui->YB7->isChecked() || mem.YA27;
   Activate_Gm( glob_line );
   Activate_LN_0( glob_line );
   Activate_LN_2( glob_line );
   Activate_LN_7( act_yb8 );
   Activate_LN_8( main_line || ui->YB7->isChecked() || mem.YA27 );

   Activate_LN_10( glob_line );
   Activate_LN_11( glob_line );
   Activate_LN_12( glob_line );
}
void PumpsManualControl::on_YB9_clicked()
{
   auto &mem = cpu::CpuMemory::Instance().M1;
   bool act_yb9 = ui->YB9->isChecked();
   bool glob_line = act_yb9 || ui->YB8->isChecked() || ui->YB7->isChecked() || mem.YA27;
   Activate_Gm( glob_line );
   Activate_LN_0( glob_line );
   Activate_LN_2( glob_line );
   Activate_LN_9( act_yb9 );

   Activate_LN_10( glob_line );
   Activate_LN_11( glob_line );
   Activate_LN_12( glob_line );
}
void PumpsManualControl::on_YB10_clicked()
{
   bool act_yb10 = ui->YB10->isChecked();
   bool act_ya28 = ui->YA28->isChecked();
   bool main_line = act_yb10;
   Activate_LN_1( main_line );
   Activate_LN_13( main_line );
   Activate_LN_14( main_line );
   Activate_LN_16( main_line );
   Activate_M13( main_line );

   if ( !act_yb10 )
      Activate_LN_15( false );
   else
      Activate_LN_15( act_ya28 );
}
void PumpsManualControl::on_YA28_clicked()
{
   on_YB10_clicked();
}
