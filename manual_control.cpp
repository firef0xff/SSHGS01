#include "manual_control.h"
#include "ui_manual_control.h"

ManualControl::ManualControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManualControl)
{
    ui->setupUi(this);
}

ManualControl::~ManualControl()
{
    delete ui;
}

void ManualControl::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}
