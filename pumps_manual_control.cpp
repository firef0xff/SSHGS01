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

   ui->YB12->setValue(mem.R2);
   ui->YB13->setValue(mem.R3);
   ui->OilLvl->setValue(mem.BL1);
   QPalette p = ui->OilLvl->palette();
   int percent = ui->OilLvl->value() *100 / ( ui->OilLvl->maximum() - ui->OilLvl->minimum() );
   QColor c;
   if ( percent <= 10 )
      c = Qt::red;
   else if ( percent <= 30 )
      c = Qt::yellow;
   else
      c = Qt::white;

   p.setColor( QPalette::Highlight, Qt::darkGreen );
   p.setColor( QPalette::Base, c );
   ui->OilLvl->setPalette(p);

}
void PumpsManualControl::SynkControls()
{

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

void PumpsManualControl::on_EngineStartStop_clicked()
{
   bool checked = ui->EngineStartStop->isChecked();
   ui->V1->setEnabled(!checked);
   ui->V2->setEnabled(!checked);

   if ( checked )
   {
      on_comboBox_activated("");
      on_comboBox_2_activated(0);
   }
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

}
void PumpsManualControl::on_YB7_val_returnPressed()
{
   CheckDR1t();
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.YB7_man = ui->YB7_val->text().toFloat();
   mem.Write();
}
void PumpsManualControl::on_YB8_val_returnPressed()
{
   CheckDR1t();
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.YB8_man = ui->YB8_val->text().toFloat();
   mem.Write();
}
void PumpsManualControl::on_YB9_val_returnPressed()
{
   CheckDR1t();
   auto& mem = cpu::CpuMemory::Instance().DB82;
   mem.YB9_man = ui->YB9_val->text().toFloat();
   mem.Write();
}
void PumpsManualControl::on_YB10_val_returnPressed()
{
   CheckDR1t();
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
