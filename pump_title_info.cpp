#include "pump_title_info.h"
#include "ui_pump_title_info.h"

Pump_title_info::Pump_title_info(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::Pump_title_info)
{
   ui->setupUi(this);
}

Pump_title_info::~Pump_title_info()
{
   delete ui;
}
