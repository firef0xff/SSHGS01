#include "pump_title_info.h"
#include "ui_pump_title_info.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_pumps.h"
#include "test_form.h"
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

   auto *val2 = new QIntValidator( 0, 350, this );
   ui->MinPressure->setValidator( val2 );
   ui->NomPressure->setValidator( val2 );
   ui->MaxPressure->setValidator( val2 );

   ui->MinPressure_2->setValidator( val2 );
   ui->NomPressure_2->setValidator( val2 );
   ui->MaxPressure_2->setValidator( val2 );

   auto *val4 = new QDoubleValidator( 0.7, 90, 2, this );
   ui->Drainage->setValidator( val4 );

   if ( !mNewMode )
   {
       FromParams();
   }
   CheckRights();
   UpdateFrequency();
}

void PumpTitleInfo::CheckRights()
{
   ui->User->setText( app::Settings::Instance().User() );
   if ( app::Settings::Instance().UserAccess() != app::UserLevel::Uncknown )
   {
      ui->AnsverBox->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
      setEnabled(true);
   }
   else
   {
      ui->AnsverBox->setStandardButtons( QDialogButtonBox::Cancel );
      setEnabled(false);
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
       mChildWindow.reset( new TestForm( mNewMode ) );
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
   if (res)
   {
      bool r = params.PressureMin1() <= params.PressureNom1() &&
            params.PressureNom1() <= params.PressureMax1() ;
      res *= ParamChecker( ui->l_min_pressure, r );
      res *= ParamChecker( ui->l_nom_pressure, r );
      res *= ParamChecker( ui->l_max_pressure, r );
   }

   if ( ui->Section2->isChecked() )
   {
      params.SectionsCount( 2 );
      res *= ParamChecker( ui->l_work_volume_2,   params.WorkVolume2( ui->WorkVolume_2->text() ) );
      res *= ParamChecker( ui->l_min_pressure_2,  params.PressureMin2( ui->MinPressure_2->text() ) );
      res *= ParamChecker( ui->l_nom_pressure_2,  params.PressureNom2( ui->NomPressure_2->text() ) );
      res *= ParamChecker( ui->l_max_pressure_2,  params.PressureMax2( ui->MaxPressure_2->text() ) );
      if (res)
      {
         bool r = params.PressureMin2() <= params.PressureNom2() &&
               params.PressureNom2() <= params.PressureMax2() ;
         res *= ParamChecker( ui->l_min_pressure_2, r );
         res *= ParamChecker( ui->l_nom_pressure_2, r );
         res *= ParamChecker( ui->l_max_pressure_2, r );
      }
   }

   res *= ParamChecker( ui->l_min_freq,  params.FrequencyMin( ui->MinFrequency->text() ) );
   res *= ParamChecker( ui->l_nom_freq,  params.FrequencyNom( ui->NomFrequency->text() ) );
   res *= ParamChecker( ui->l_max_freq,  params.FrequencyMax( ui->MaxFrequency->text() ) );
   if (res)
   {
      bool r = params.FrequencyMin() <= params.FrequencyNom() &&
            params.FrequencyNom() <= params.FrequencyMax() ;
      res *= ParamChecker( ui->l_min_freq, r );
      res *= ParamChecker( ui->l_nom_freq, r );
      res *= ParamChecker( ui->l_max_freq, r );
   }

   res *= ParamChecker( ui->l_volume_kpd,  params.VolumeKPD( ui->VolumeKPD->text() ) );

   if ( !ui->Drainage->text().isEmpty() )
   {
      res *= ParamChecker( ui->l_drainage,  ValidateRange( ui->Drainage, params.Expenditure( ui->Drainage->text() ) ) );
   }

   res *= ParamChecker( ui->l_func_test_time,   params.FuncTestTime( ui->FuncTestTime->text() ) );
   res *= ParamChecker( ui->l_strong_test_time, params.StrongTestTime( ui->StrongTestTime->text() ) );
   res *= ParamChecker( ui->l_herm_test_time,   params.HermTestTime( ui->HermTestTime->text() ) );

   res *= ParamChecker( ui->l_x,    params.X( ui->X->text() ) );
   res *= ParamChecker( ui->l_y,    params.Y( ui->Y->text() ) );
   res *= ParamChecker( ui->l_z,    params.Z( ui->Z->text() ) );
   res *= ParamChecker( ui->l_a,    params.A( ui->A->text() ) );
   res *= ParamChecker( ui->l_a1,   params.A1( ui->A1->text() ) );
   res *= ParamChecker( ui->l_e,    params.E( ui->E->text() ) );
   res *= ParamChecker( ui->l_b,    params.B( ui->B->text() ) );

   res *= ParamChecker( ui->l_cotrol_mode,    params.TypeControl( ui->ControlMode->currentText() ) );
   res *= ParamChecker( ui->l_work_temperature,    params.WorkTemperature( ui->WorkTemperature->text() ) );

   bool a1 = ui->ConturA1->isChecked();
   bool b1 = ui->ConturB1->isChecked();
   bool c1 = ui->ConturC1->isChecked();
   params.ConturA1( a1 );
   params.ConturB1( b1 );
   params.ConturC1( c1 );

   res *= (a1||b1||c1);

   if ( ui->Section2->isChecked() )
   {
      bool a2 = ui->ConturA2->isChecked();
      bool b2 = ui->ConturB2->isChecked();
      bool c2 = ui->ConturC2->isChecked();
      params.ConturA2( a2 );
      params.ConturB2( b2 );
      params.ConturC2( c2 );

      res *= (a2||b2||c2);
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

   ui->FuncTestTime->setValue( params.FuncTestTime() );
   ui->StrongTestTime->setValue( params.StrongTestTime() );
   ui->HermTestTime->setValue( params.HermTestTime() );

   ui->X->setValue( params.X() );
   ui->Y->setValue( params.Y() );
   ui->Z->setValue( params.Z() );
   ui->A->setValue( params.A() );
   ui->A1->setValue( params.A1() );
   ui->E->setValue( params.E() );
   ui->B->setValue( params.B() );

   ui->ControlMode->setCurrentText( test::ToString( params.TypeControl() ) );
   ui->WorkTemperature->setValue( params.WorkTemperature() );

   ui->ConturA1->setChecked( params.ConturA1() );
   ui->ConturB1->setChecked( params.ConturB1() );
   ui->ConturC1->setChecked( params.ConturC1() );

   ui->ConturA2->setChecked( params.ConturA2() );
   ui->ConturB2->setChecked( params.ConturB2() );
   ui->ConturC2->setChecked( params.ConturC2() );
}


void PumpTitleInfo::on_ConturA1_clicked()
{
   ProcessContur( ui->ConturA1 );
}
void PumpTitleInfo::on_ConturB1_clicked()
{
   ProcessContur( ui->ConturB1 );
}
void PumpTitleInfo::on_ConturC1_clicked()
{
   ProcessContur( ui->ConturC1 );
}
void PumpTitleInfo::on_ConturA2_clicked()
{
   ProcessContur( ui->ConturA2 );
}
void PumpTitleInfo::on_ConturB2_clicked()
{
   ProcessContur( ui->ConturB2 );
}
void PumpTitleInfo::on_ConturC2_clicked()
{
   ProcessContur( ui->ConturC2 );
}

void PumpTitleInfo::UpdateFrequency()
{
   int k_a = 600;
   int k_b = 240;
   int k_c = 36;
   int k = k_a;

   auto SetK = [&]( bool r, int kn )
   {
      if ( r && k > kn )
         k = kn;
   };

   SetK( ui->ConturA1->isChecked(), k_a );
   SetK( ui->ConturB1->isChecked(), k_b );
   SetK( ui->ConturC1->isChecked(), k_c );
   bool val = false;
   bool val2 = false;
   double d1 = ui->WorkVolume->text().toDouble( &val );
   double d2 = ui->WorkVolume_2->text().toDouble( &val2 );

   int n_max1 = 0;
   int n_max2 = 0;

   if ( val )
      n_max1 = k * 1000 / d1;

   int freq = n_max1;
   if ( ui->Section2->isChecked() )
   {
      k = k_a;
      SetK( ui->ConturA2->isChecked(), k_a );
      SetK( ui->ConturB2->isChecked(), k_b );
      SetK( ui->ConturC2->isChecked(), k_c );

      if ( val2 )
         n_max2 = k * 1000 / d2;
      freq = std::min( n_max1, n_max2 );
   }


   int max_val = 2400;
   if ( app::Settings::Instance().UserAccess() == app::UserLevel::Admin )
   {
      max_val = 4000;
   }
   freq = std::min( freq, max_val );

   auto *val3 = new QIntValidator( 100, freq, this );
   ui->MinFrequency->setValidator( val3 );
   ui->NomFrequency->setValidator( val3 );
   ui->MaxFrequency->setValidator( val3 );

   QString tt = QString::number( val3->bottom() ) + "..." + QString::number( val3->top() ) + " об/мин";
   ui->MinFrequency->setToolTip( tt );
   ui->NomFrequency->setToolTip( tt );
   ui->MaxFrequency->setToolTip( tt );
}

void PumpTitleInfo::ProcessContur( QCheckBox* activated )
{
   if ( !activated->isChecked() )
      return;

   typedef std::vector< QCheckBox* > ToDisable;
   typedef std::map< QCheckBox*, ToDisable > ConturMap;

   ConturMap m =
   {
      std::pair<QCheckBox*,ToDisable >(ui->ConturA1,{ui->ConturB1, ui->ConturC1, ui->ConturA2} ),
      std::pair<QCheckBox*,ToDisable >(ui->ConturB1,{ui->ConturC1, ui->ConturA1, ui->ConturB2} ),
      std::pair<QCheckBox*,ToDisable >(ui->ConturC1,{ui->ConturB1, ui->ConturA1, ui->ConturC2} ),

      std::pair<QCheckBox*,ToDisable >(ui->ConturA2,{ui->ConturB2, ui->ConturC2, ui->ConturA1} ),
      std::pair<QCheckBox*,ToDisable >(ui->ConturB2,{ui->ConturC2, ui->ConturA2, ui->ConturB1} ),
      std::pair<QCheckBox*,ToDisable >(ui->ConturC2,{ui->ConturB2, ui->ConturA2, ui->ConturC1} )
   };

   ToDisable& to_disable = m[activated];
   for( QCheckBox* item: to_disable )
   {
      item->setChecked( false );
   }
   UpdateFrequency();
}

void PumpTitleInfo::on_WorkVolume_textChanged(const QString &)
{
    UpdateFrequency();
}

void PumpTitleInfo::on_WorkVolume_2_textChanged(const QString &)
{
    UpdateFrequency();
}
