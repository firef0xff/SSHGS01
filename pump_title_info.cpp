#include "pump_title_info.h"
#include "ui_pump_title_info.h"
#include "pump_test_info.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_pumps.h"
#include "functions.h"
#include "settings/settings.h"

PumpTitleInfo::PumpTitleInfo(bool new_mode, QWidget *parent) :
   ChildWidget(parent),
   ui(new Ui::Pump_title_info),
   mNewMode( new_mode )
{
   ui->setupUi(this);

   foreach ( examinee::Device const* device, test::pump::Parameters::Instance().TestCollection().Devices().Get() )
   {
      ui->Model->addItem( device->Name() + " " + device->Manufacturer(), QVariant::fromValue( device ) );
   }
   ui->Model->setCurrentIndex( -1 );

   auto *val1 = new QIntValidator( 0, 355, this );
   ui->WorkVolume->setValidator( val1 );
   ui->WorkVolume_2->setValidator( val1 );

   auto *val2 = new QIntValidator( 0, 450, this );
   ui->MinPressure->setValidator( val2 );
   ui->NomPressure->setValidator( val2 );
   ui->MaxPressure->setValidator( val2 );

   ui->MinPressure_2->setValidator( val2 );
   ui->NomPressure_2->setValidator( val2 );
   ui->MaxPressure_2->setValidator( val2 );

   auto *val3 = new QIntValidator( 0, 4000, this );
   ui->MinFrequency->setValidator( val3 );
   ui->NomFrequency->setValidator( val3 );
   ui->MaxFrequency->setValidator( val3 );


   auto *val4 = new QDoubleValidator( 0.7, 90, 2, this );
   ui->Drainage->setValidator( val4 );

   if ( !mNewMode )
   {
       FromParams();
   }
   CheckRights();
}

void PumpTitleInfo::CheckRights()
{
   ui->User->setText( app::Settings::Instance().User() );

   if ( app::Settings::Instance().UserAccess() != app::UserLevel::Uncknown )
   {
      ui->AnsverBox->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
      ui->gridLayout->setEnabled(true);
   }
   else
   {
      ui->AnsverBox->setStandardButtons( QDialogButtonBox::Cancel );
      ui->gridLayout->setEnabled(false);
   }
}
void PumpTitleInfo::OnLogin()
{
   if ( isHidden() )
      return;
   CheckRights();
}

PumpTitleInfo::~PumpTitleInfo()
{
   delete ui;
}

void PumpTitleInfo::on_AnsverBox_rejected()
{
    close();
}
void PumpTitleInfo::on_AnsverBox_accepted()
{
   if ( SaveInputParams() )
   {
       hide();
       if ( mChildWindow.get() )
       {
           QObject::disconnect( mChildWindow.get(), &ChildWidget::closed, this, &ChildWidget::close );
           QObject::disconnect( this, &ChildWidget::login, mChildWindow.get(), &ChildWidget::on_login );
       }
       mChildWindow.reset( new PumpTestInfo( mNewMode ) );
       QObject::connect( mChildWindow.get(), &ChildWidget::closed, this, &ChildWidget::close );
       QObject::connect( this, &ChildWidget::login, mChildWindow.get(), &ChildWidget::on_login );


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

bool PumpTitleInfo::SaveInputParams()
{
   test::pump::Parameters& params = test::pump::Parameters::Instance();
   test::CURRENT_PARAMS = &params;

   if ( mNewMode )
       params.Reset();

   bool res = true;

   res *= ParamChecker( ui->l_ser_no,  params.SerianNo( ui->SerNo->text() ) );
   res *= ParamChecker( ui->l_model,  params.Model( ui->Model->currentText() ) );
   res *= ParamChecker( ui->l_spin,  params.Spin( ui->Spin->currentText() ) );

   params.ElektrikControl( ui->ElectricControl->isChecked() );
   if ( params.ElektrikControl() )
   {
      res *= ParamChecker( ui->l_voltage, params.Voltage( ui->Voltage->currentText() ) );
      res *= ParamChecker( ui->l_ac_dc, params.VoltageType( ui->AcDc->currentText() ) );
   }

   params.HydroControl( ui->HydroControl->isChecked() );
   if ( params.HydroControl() )
   {
      res *= ParamChecker( ui->l_max_control_pressure, params.MaxControlPressure( ui->MaxControlPressure->text() ) );
   }

   params.SectionsCount( 1 );
   res *= ParamChecker( ui->l_work_volume,   params.WorkVolume1( ui->WorkVolume->text() ) );
   res *= ParamChecker( ui->l_min_pressure,  params.PressureMin1( ui->MinPressure->text() ) );
   res *= ParamChecker( ui->l_nom_pressure,  params.PressureNom1( ui->NomPressure->text() ) );
   res *= ParamChecker( ui->l_max_pressure,  params.PressureMax1( ui->MaxPressure->text() ) );

   if ( ui->Section2->isChecked() )
   {
      params.SectionsCount( 2 );
      res *= ParamChecker( ui->l_work_volume_2,   params.WorkVolume2( ui->WorkVolume_2->text() ) );
      res *= ParamChecker( ui->l_min_pressure_2,  params.PressureMin2( ui->MinPressure_2->text() ) );
      res *= ParamChecker( ui->l_nom_pressure_2,  params.PressureNom2( ui->NomPressure_2->text() ) );
      res *= ParamChecker( ui->l_max_pressure_2,  params.PressureMax2( ui->MaxPressure_2->text() ) );
   }

   res *= ParamChecker( ui->l_min_freq,  params.FrequencyMin( ui->MinFrequency->text() ) );
   res *= ParamChecker( ui->l_nom_freq,  params.FrequencyNom( ui->NomFrequency->text() ) );
   res *= ParamChecker( ui->l_max_freq,  params.FrequencyMax( ui->MaxFrequency->text() ) );

   res *= ParamChecker( ui->l_volume_kpd,  params.VolumeKPD( ui->VolumeKPD->text() ) );

   if ( !ui->Drainage->text().isEmpty() )
   {
      res *= ParamChecker( ui->l_drainage,  ValidateRange( ui->Drainage, params.Expenditure( ui->Drainage->text() ) ) );
   }
   return res;
}

void PumpTitleInfo::FromParams()
{
   test::pump::Parameters& params = test::pump::Parameters::Instance();
   test::CURRENT_PARAMS = &params;

   ui->SerNo->setText( params.SerianNo() );
   ui->Model->setCurrentText( params.Model() );

   ui->Spin->setCurrentText( test::ToString( params.Spin() ) );
   ui->ElectricControl->setChecked( params.ElektrikControl() );
   if ( params.ElektrikControl() )
   {
      ui->Voltage->setCurrentText( test::ToString( params.Voltage() ) );
      ui->AcDc->setCurrentText( test::ToString( params.VoltageType() ) );
   }

   ui->HydroControl->setChecked( params.HydroControl() );
   if ( params.HydroControl() )
   {
      ui->MaxControlPressure->setValue( params.MaxControlPressure() );
   }


   ui->WorkVolume->setText ( test::ToString( params.WorkVolume1() ) );
   ui->MinPressure->setText( test::ToString( params.PressureMin1() ) );
   ui->NomPressure->setText( test::ToString( params.PressureNom1() ) );
   ui->MaxPressure->setText( test::ToString( params.PressureMax1() ) );

   ui->Section2->setChecked( false );
   if ( params.SectionsCount() == 2 )
   {
      ui->Section2->setChecked( true );
      ui->WorkVolume_2->setText ( test::ToString( params.WorkVolume2() ) );
      ui->MinPressure_2->setText( test::ToString( params.PressureMin2() ) );
      ui->NomPressure_2->setText( test::ToString( params.PressureNom2() ) );
      ui->MaxPressure_2->setText( test::ToString( params.PressureMax2() ) );
   }

   ui->MinFrequency->setText( test::ToString( params.FrequencyMin() ) );
   ui->NomFrequency->setText( test::ToString( params.FrequencyNom() ) );
   ui->MaxFrequency->setText( test::ToString( params.FrequencyMax() ) );

   ui->VolumeKPD->setValue( params.VolumeKPD() );

   if ( params.Expenditure() )
   {
      ui->Drainage->setText( test::ToString( params.Expenditure() ) );
   }
}
