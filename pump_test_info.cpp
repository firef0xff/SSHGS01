#include "pump_test_info.h"
#include "ui_pump_test_info.h"

Pump_test_info::Pump_test_info(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::Pump_test_info)
{
   ui->setupUi(this);
}

Pump_test_info::~Pump_test_info()
{
   delete ui;
}
