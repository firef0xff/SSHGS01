#include "pumps_manual_control.h"
#include "ui_pumps_manual_control.h"
#include "test_case/implementation/test_base.h"
#include "settings/settings.h"
#include <QMessageBox>
#include <QProcess>
#include <QFile>

PumpsManualControlUpdater::PumpsManualControlUpdater():
    mStopSignal(false)
{}
void PumpsManualControlUpdater::run()
{
    mStopSignal = false;
    while ( !mStopSignal )
    {
        cpu::CpuMemory::Instance().DB40.Read();
        cpu::CpuMemory::Instance().DB50.Read();
        cpu::CpuMemory::Instance().DB70.Read();
        cpu::CpuMemory::Instance().DB73.Read();
        cpu::CpuMemory::Instance().DB82R.Read();
        auto &mem = cpu::CpuMemory::Instance().M4;
        std::unique_lock< std::mutex > lock(mem.Mutex);
        mem.Read();
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
   ChildWidget(parent),
   ui(new Ui::pumps_manual_control),
   mTaskMode( cpu::CpuMemory::Instance().DB31 )
{
   ui->setupUi(this);

   auto *val1 = new QDoubleValidator( 0, 999.9, 1, this );
   ui->V1->setValidator( val1 );
   ui->V2->setValidator( val1 );

   auto *val3 = new QIntValidator( 0, 350, this );
   ui->YB7_val->setValidator( val3 );
   ui->YB8_val->setValidator( val3 );
   ui->YB9_val->setValidator( val3 );
   ui->YB10_val->setValidator( val3 );

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
   else
      SetColor( ui->px_ln_4_2, Qt::black ); //16
   if( mem.SP17_warning )
      SetColor( ui->px_ln_7_2, Qt::red ); //17
   else
      SetColor( ui->px_ln_7_2, Qt::black ); //17
   if( mem.SP18_warning )
      SetColor( ui->px_ln_9_2, Qt::red ); //18
   else
      SetColor( ui->px_ln_9_2, Qt::black ); //18
   if( mem.SP25_warning )
      SetColor( ui->px_ln_11_2, Qt::red ); //25
   else
      SetColor( ui->px_ln_11_2, Qt::black ); //25
   if( mem.SP24_warning )
      SetColor( ui->px_ln_12_2, Qt::red ); //24
   else
      SetColor( ui->px_ln_12_2, Qt::black ); //24
   if( mem.SP22_warning )
      SetColor( ui->px_ln_10_4, Qt::red ); //22
   else
      SetColor( ui->px_ln_10_4, Qt::black ); //22
   if( mem.SP19_warning )
      SetColor( ui->px_ln_16_1, Qt::red ); //19
   else
      SetColor( ui->px_ln_16_1, Qt::black ); //19

   mErrors = test::ErrMsg();
   if ( mErrors.isEmpty() )
   {
      mShowErr = false;
      SetColor( ui->Alarm, Qt::black );
   }
   else
   {
      if (!mShowErr)
      {
         mShowErr = true;
         SetColor( ui->Alarm, Qt::red );
         on_Alarm_clicked();
      }
   }
}
void PumpsManualControl::UpdateData()
{
   auto & mem = cpu::CpuMemory::Instance().DB70;
   auto & mem1 = cpu::CpuMemory::Instance().DB50;
   auto &mem2 = cpu::CpuMemory::Instance().DB82R;

   auto Round= []( float val )
   {
      return round( val *10)/10;
   };

   ui->BP7->display( Round( mem.BP7 ) );
   ui->BP8->display( Round( mem.BP8 ) );
   ui->BP9->display( Round( mem.BP9 ) );
   ui->BP10->display( Round( mem.BP10 ) );
   ui->BP12->display( Round( mem.BP12 ) );
   ui->BP13->display( Round( mem.BP13 ) );

   ui->BV5->display( Round( mem.BV5 ) );
   ui->BV6->display( Round( mem.BV6 ) );
   ui->BV7->display( Round( mem.BV7 ) );

   ui->BV8910->display( Round( max( max( mem.BV8, mem.BV9 ), mem.BV10 ) ) );

   ui->DM1->display( Round( mem.DM1 ) );
   ui->DR1->display( Round( mem.DR1 ) );

   ui->BT1->display( Round( mem1.BT1) );
   ui->BT2->display( Round( mem.BT2 ) );
   ui->BT3->display( Round( mem.BT3 ) );

   ui->YB12->setValue(mem.R2);
   ui->YB13->setValue(mem.R3);
   ui->OilLvl->setValue(mem.BL1);

   ui->V1r->display(Round( mem2.Q1_man ));
   ui->V2r->display(Round( mem2.Q2_man ));
   ui->MotoClock->display(Round( mem.MotoClock ));

   QPalette p = ui->OilLvl->palette();
   int percent = ui->OilLvl->value() *100 / ( ui->OilLvl->maximum() - ui->OilLvl->minimum() );
   QString c;
   if ( percent <= 10 )
      c = "background-color: rgb(255, 0, 0);";
   else if ( percent <= 30 )
      c = "background-color: rgb(255, 255, 0);";
   else
      c = "background-color: rgb(255, 255, 255);";

   p.setColor( QPalette::Highlight, Qt::darkGreen );
   p.setColor( QPalette::Base, c );
   ui->OilLvl->setStyleSheet( "border-color: rgb(255,255,255);" + c );

}
void PumpsManualControl::SynkControls()
{
   auto &mem = cpu::CpuMemory::Instance().M4;
   UpdateButton( ui->TestStart, mem.LubMonStart );
}
void PumpsManualControl::UpdateButton( QAbstractButton *btn, bool checked )
{
    btn->setChecked( checked );
}

void PumpsManualControl::Start()
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Reset();
   mem.PumpManual = true;
   mem.Write();
   on_comboBox_activated("");
   on_comboBox_2_activated(0);

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
   cpu::CpuMemory::Instance().DB82.Reset();

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
    ChildWidget::closeEvent( e );
}
void PumpsManualControl::showEvent( QShowEvent *e )
{
    CheckRights();
    QWidget::showEvent( e );
}
void PumpsManualControl::hideEvent( QHideEvent *e )
{
    QWidget::hideEvent( e );
}


void PumpsManualControl::SetColor( QWidget *label, QColor cl )
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

void PumpsManualControl::on_EngineStartStop_clicked()
{
   bool checked = ui->EngineStartStop->isChecked();

   bool error = false;
   auto& e_mem1 = cpu::CpuMemory::Instance().DB40;
   error += e_mem1.sl3;
   error += e_mem1.sq1;
   error += e_mem1.sq3;
   error += e_mem1.sq5;
   error += e_mem1.sq7;
   error += e_mem1.sq13;
   error += e_mem1.sq14;
   error += e_mem1.sq15;
   error += e_mem1.sq9;

   auto& e_mem2 = cpu::CpuMemory::Instance().DB73;
   error += e_mem2.SQ21_warning;
   error += e_mem2.SQ16_warning;
   error += e_mem2.SQ17_warning;
   error += e_mem2.SQ18_warning;
   error += e_mem2.LevelMaslaAlarmPump;
   error += e_mem2.TempMaslaAlarmPump1;
   error += e_mem2.TempMaslaAlarmPump2;

   if ( error )
   {
      on_Alarm_clicked();
      checked = false;
      ui->EngineStartStop->setChecked( checked );
   }

   ui->V1->setEnabled(!checked);
   ui->V2->setEnabled(!checked);
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Start_Stop = checked;
   mem.Write();
}

void PumpsManualControl::CheckDR1t()
{
   bool c = (ui->YB7_val->text().toDouble() +
            ui->YB8_val->text().toDouble() +
            ui->YB9_val->text().toDouble() +
            ui->YB10_val->text().toDouble()) != 0.0;
   ui->DR1t->setEnabled(!c);

   auto power = []( float q, float n, float p )
   {
      return (q*n/1000*p)/540;
   };
   auto& mem = cpu::CpuMemory::Instance().DB82;
   float p7 = ui->YB7_val->text().toFloat();
   float p8 = ui->YB8_val->text().toFloat();
   float p9 = ui->YB9_val->text().toFloat();

   float p1 = 0.0;
   float p2 = 0.0;
   int cnt = 0;
   auto fill_vals = [&p1, &p2, &cnt]( float& val )
   {
      if ( val )
      {
         if (!p1)
         {
            p1 = val;
            ++cnt;
         }
         else if ( !p2 )
         {
            p2 = val;
            ++cnt;
         }
         else
            val = 0.0;
      }
   };

   fill_vals( p7 );
   fill_vals( p8 );
   fill_vals( p9 );

   float n = mem.Speed_man;

   float q1 = mem.Q1_man;
   float q2 = mem.Q2_man;
   if ( cnt == 2 )
   {
      if ( !q1 )
      {
         q1 = q2;
      }
      if ( !q2 )
      {
         q2 = q1;
      }
   }

   float max_power = power( q1, n, p1 );
   if ( cnt == 2 )
      max_power += power( q2, n, p2 );

//   if ( max_power <= 110 )
//   {
      mem.YB7_man = p7;
      mem.YB8_man = p8;
      mem.YB9_man = p9;
      mem.Q1_man = q1;
      mem.Q2_man = q2;
      mem.Write();
//      return;
//   }

//   QMessageBox msg;
//   msg.setWindowTitle( "Ошибки тестирования" );
//   msg.setText( "Превышено ограничение в 110кВт\nНеобходимая мощность " + QString::number(max_power) + "кВт" );
//   msg.addButton( QMessageBox::Ok );
//   msg.setModal( true );
//   msg.exec();
}
void PumpsManualControl::on_YB7_val_returnPressed()
{
   CheckDR1t();
}
void PumpsManualControl::on_YB8_val_returnPressed()
{
   CheckDR1t();
}
void PumpsManualControl::on_YB9_val_returnPressed()
{
   CheckDR1t();
}
void PumpsManualControl::on_YB10_val_returnPressed()
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.YB10_man = ui->YB10_val->text().toFloat();
   mem.Write();
}


void PumpsManualControl::on_V1_returnPressed()
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Q1_man = ui->V1->text().toFloat();
   mem.Write();
}
void PumpsManualControl::on_V2_returnPressed()
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Q2_man = ui->V2->text().toFloat();
   mem.Write();
}
void PumpsManualControl::on_DR1t_returnPressed()
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Speed_man = ui->DR1t->text().toFloat();
   mem.Write();
}

void PumpsManualControl::on_comboBox_activated(const QString &)
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Voltage_man = ui->comboBox->currentText().toFloat();
   mem.Write();
}

void PumpsManualControl::on_comboBox_2_activated(int)
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.Type_Voltage_man = ui->comboBox_2->currentIndex();
   mem.Write();
}

void PumpsManualControl::on_YA27_clicked()
{
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.YA27 = ui->YA27->isChecked();
   mem.Write();
}

void PumpsManualControl::on_Alarm_clicked()
{
   if (mErrors.isEmpty())
      return;

   QMessageBox msg;
   msg.setWindowTitle( "Ошибки тестирования" );
   msg.setText( mErrors );
   msg.addButton( QMessageBox::Ok );
   msg.setModal( true );
   msg.exec();
   cpu::CpuMemory::Instance().M1.SetKvitir_Osch(true);
}

void PumpsManualControl::CheckYb()
{

   std::vector<QLineEdit *> uis;
   if ( ui->YB7_val->text().toDouble() == 0.0 )
   {
      uis.push_back(ui->YB7_val);
   }
   if ( ui->YB8_val->text().toDouble() == 0.0 )
   {
      uis.push_back(ui->YB8_val);
   }
   if ( ui->YB9_val->text().toDouble() == 0.0 )
   {
      uis.push_back(ui->YB9_val);
   }

   for ( QLineEdit* ptr: uis )
      ptr->setEnabled( uis.size() >= 2 );
}
void PumpsManualControl::on_YB7_val_textChanged(const QString &)
{
   CheckYb();
}
void PumpsManualControl::on_YB8_val_textChanged(const QString &)
{
   CheckYb();
}
void PumpsManualControl::on_YB9_val_textChanged(const QString &)
{
   CheckYb();
}

void PumpsManualControl::OnLogin()
{
   if ( isHidden() )
      return;
   CheckRights();
}
void  PumpsManualControl::CheckRights()
{
   if ( app::Settings::Instance().UserAccess() != app::UserLevel::Uncknown )
   {
      setEnabled(true);
      Start();
      int max_val = 2400;
      if (app::Settings::Instance().UserAccess() == app::UserLevel::Admin )
         max_val = 4000;

      auto *val2 = new QIntValidator( 0, max_val, this );
      ui->DR1t->setValidator( val2 );
      QString tt = QString::number( val2->bottom() ) + "..." + QString::number( val2->top() ) + " об/мин";
      ui->DR1t->setToolTip( tt );
   }
   else
   {
      setEnabled(false);
      Stop();
   }
}

void PumpsManualControl::on_TestStart_clicked()
{
   auto &mem = cpu::CpuMemory::Instance().M4;
   std::unique_lock< std::mutex > lock(mem.Mutex);
   mem.LubMonStart = true;
   mem.LubMonStop = false;
   mem.Write();

   QString exe = "\"" + app::Settings::Instance().LubMon() +"\"";

   if ( !QProcess::startDetached( exe ) )
   {
      mErrors += "Не удалось запустить " + exe + "\n";
      on_Alarm_clicked();
   }


}

void PumpsManualControl::on_TestStop_clicked()
{
   auto &mem = cpu::CpuMemory::Instance().M4;
   std::unique_lock< std::mutex > lock(mem.Mutex);
   mem.LubMonStop = true;
   mem.Write();
}
