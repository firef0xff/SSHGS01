#include "pump_test_info.h"
#include "ui_pump_test_info.h"
#include <QMessageBox>
#include "test_case/implementation/test_params_pumps.h"
#include "test_form.h"
#include "functions.h"
#include "settings/settings.h"

PumpTestInfo::PumpTestInfo(bool new_mode, QWidget *parent) :
   ChildWidget(parent),
   ui(new Ui::Pump_test_info),
   mNewMode(new_mode)
{
   ui->setupUi(this);

   if ( !mNewMode )
   {
       FromParams();
   }
   CheckRights();
}

PumpTestInfo::~PumpTestInfo()
{
   delete ui;
}

void PumpTestInfo::on_buttonBox_rejected()
{
    close();
}
void PumpTestInfo::on_buttonBox_accepted()
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


bool PumpTestInfo::SaveInputParams()
{
   test::pump::Parameters& params = test::pump::Parameters::Instance();
   test::CURRENT_PARAMS = &params;

   bool res = true;

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

   return res;
}
void PumpTestInfo::FromParams()
{
   test::pump::Parameters& params = test::pump::Parameters::Instance();
   test::CURRENT_PARAMS = &params;

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
}

void PumpTestInfo::OnLogin()
{
   if (isHidden())
      return;
   CheckRights();
}

void PumpTestInfo::CheckRights()
{
   if ( app::Settings::Instance().UserAccess() != app::UserLevel::Uncknown )
   {
      ui->buttonBox->setStandardButtons( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
      ui->gridLayout->setEnabled(true);
   }
   else
   {
      ui->buttonBox->setStandardButtons( QDialogButtonBox::Cancel );
      ui->gridLayout->setEnabled(false);
   }
}
