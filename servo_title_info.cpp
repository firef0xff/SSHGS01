#include "servo_title_info.h"
#include "ui_servo_title_info.h"

ServoTitleInfo::ServoTitleInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServoTitleInfo)
{
    ui->setupUi(this);
}

ServoTitleInfo::~ServoTitleInfo()
{
    delete ui;
}

void ServoTitleInfo::on_buttonBox_accepted()
{
    hide();
    close();
}

void ServoTitleInfo::on_buttonBox_rejected()
{
    close();
}

void ServoTitleInfo::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}


