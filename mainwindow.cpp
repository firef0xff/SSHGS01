#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test_case/tests.h"
#include "title_info.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->TestCase1->setText( test::HydroTests.Name() );
    ui->TestCase1->setDescription( test::HydroTests.Descr() );

    ui->TestCase2->setText( test::ServoTests.Name() );
    ui->TestCase2->setDescription( test::ServoTests.Descr() );

    ui->ManualControl->setText( "Ручное управление" );
    ui->ManualControl->setDescription( "Запуск стенда в режиме ручного управления для администрирования и обслуживания" );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_TestCase1_clicked()
{
    mChildWindow.reset( new TitleInfo() );
    hide();
    mChildWindow->show();
}

void MainWindow::on_TestCase2_clicked()
{
    mChildWindow.reset( new TitleInfo() );
    hide();
    mChildWindow->show();
}

void MainWindow::on_ManualControl_clicked()
{
    mChildWindow.reset( new TitleInfo() );
    hide();
    mChildWindow->show();
}
