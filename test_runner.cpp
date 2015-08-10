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
    ui->progressBar->setValue(0);
    ui->progressBar->reset();
    ui->LogBox->clear();
    if ( mWorker.get() )
        StopWorker();

    mWorker.reset( new Worker( mTestCase ));
    QObject::connect( mWorker.get(), &Worker::to_log, ui->LogBox, &QTextBrowser::append );
    QObject::connect( mWorker.get(), &Worker::progress, this, &TestRunner::on_progress );
    mWorker->start();
}

void TestRunner::StopWorker()
{
    if ( mWorker.get() )
    {
        mWorker->stop();
        QObject::disconnect( mWorker.get(), &Worker::to_log, ui->LogBox, &QTextBrowser::append );
        QObject::disconnect( mWorker.get(), &Worker::progress, this, &TestRunner::on_progress );
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

void TestRunner::on_progress()
{
    ui->progressBar->setValue( ui->progressBar->value() + 1 );
}



Worker::Worker( TestRunner::TestCase const& test_case ):
    mStopSignal(false),
    mTestCase(test_case)
{}
void Worker::run()
{
    mStopSignal = false;
    foreach (test::Test* to_run, mTestCase)
    {
        if (mStopSignal)
            break;
        LogIt( "Запущен тест: " + to_run->Name() );
        bool result = to_run->Run( std::bind( &Worker::LogIt, this, std::placeholders::_1 ), mStopSignal );
        emit progress();
        if (result)
            LogIt( "Тест пройден" );
        else
            LogIt( "Тест не пройден" );
        LogIt( QString() );
    }
}
void Worker::stop()
{
    mStopSignal = true;
    wait();
}

void Worker::LogIt( QString const& str )
{
    emit to_log( str );
}

