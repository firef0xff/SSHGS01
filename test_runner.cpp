#include "test_runner.h"
#include "ui_test_runner.h"
#include "test_case/test.h"
#include <QLabel>
#include <QThread>

TestRunner::TestRunner( TestCase test_case, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestRunner),
    mTestCase( test_case )
{
    ui->setupUi(this);

    uint32_t num = 0;
    foreach ( auto test_ptr, mTestCase )
    {
        ControlPtr ptr( new QLabel( ui->scrollAreaWidgetContents ) );
        ptr->setObjectName( QStringLiteral("l_") + QString::number( test_ptr->Number() ) );
        //ptr->setProperty("object", QVariant::fromValue( test_ptr ) );
        ptr->setText( test_ptr->Name() );
        ui->gridLayout->addWidget( ptr.get() , num++ , 0, 1, 1 );
        mChilds.append( ptr );
    }
}

TestRunner::~TestRunner()
{
    delete ui;
}

void TestRunner::closeEvent(QCloseEvent *e)
{
    StopWorker();
    QWidget::closeEvent( e );
    emit closed();
}


void TestRunner::on_Start_clicked()
{
    ui->progressBar->setRange( 0, mTestCase.size() - 1 );
    ui->progressBar->reset();
    ui->LogBox->clear();
    if ( mWorker.get() )
        StopWorker();

    mWorker.reset( new Worker(
    [&](bool const& stop_signal )->void
    {
        foreach (test::Test* to_run, mTestCase)
        {
            if (stop_signal)
                break;
            ui->LogBox->append( QStringLiteral("Запущен тест: ") + to_run->Name() );
            bool result = to_run->Run( std::bind( &QTextBrowser::append, ui->LogBox, std::placeholders::_1 ), stop_signal );
            ui->progressBar->setValue( ui->progressBar->value() + 1 );
            if (result)
                ui->LogBox->append( QStringLiteral("Тест пройден") );
            else
                ui->LogBox->append( QStringLiteral("Тест не пройден") );
            ui->LogBox->append( QString() );
        }
    }));
    mWorker->start();
}

void TestRunner::StopWorker()
{
    if ( mWorker.get() )
    {
        mWorker->stop();
        mWorker.reset();
    }
}

void TestRunner::on_Abort_clicked()
{
    StopWorker();
}

void TestRunner::on_Cancel_clicked()
{
    close();
}
