#include "pumps_manual_control.h"
#include "ui_pumps_manual_control.h"

pumps_manual_control::pumps_manual_control(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::pumps_manual_control)
{
   ui->setupUi(this);
}

pumps_manual_control::~pumps_manual_control()
{
   delete ui;
}
