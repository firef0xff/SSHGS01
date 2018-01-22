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
    UpdateMarks();
    UpdateData();
}

void PumpsManualControl::UpdateMarks()
{}
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
{}
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
