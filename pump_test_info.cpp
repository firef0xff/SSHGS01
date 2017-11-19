#include "pump_test_info.h"
#include "ui_pump_test_info.h"

PumpTestInfo::PumpTestInfo(bool new_mode, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::Pump_test_info),
   mNewMode(new_mode)
{
   ui->setupUi(this);
}

PumpTestInfo::~PumpTestInfo()
{
   delete ui;
}

void PumpTestInfo::closeEvent(QCloseEvent *e)
{
   QWidget::closeEvent( e );
   emit closed();
}
