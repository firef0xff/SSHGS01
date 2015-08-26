#include "stand_params.h"
#include "ui_stand_params.h"

StandParams::StandParams(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StandParams)
{
    ui->setupUi(this);
}

StandParams::~StandParams()
{
    delete ui;
}
