#include "servo_title_info.h"
#include "ui_servo_title_info.h"
#include "test_case/tests.h"
#include "test_form.h"

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
    if ( mChildWindow.get() )
        QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
    mChildWindow.reset( new TestForm( test::ServoTests ) );
    QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
    mChildWindow->show();
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


