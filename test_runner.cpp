#include "test_runner.h"
#include "ui_test_runner.h"
#include "test_case/test.h"
#include "test_case/test_params.h"
#include <QLabel>
#include <QThread>
#include <QFileDialog>
#include "settings/settings.h"
#include "viewer.h"

TestRunner::TestRunner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestRunner)
{
    qRegisterMetaType< Functor >();
    ui->setupUi(this);

    uint32_t num = 0;
    foreach ( auto test_ptr, test::CURRENT_PARAMS->TestCase() )
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
    close();
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
    if ( mWorker.get() )
        StopWorker();

    ui->progressBar->reset();
    ui->progressBar->setRange( 0, test::CURRENT_PARAMS->TestCase().size() );
    ui->progressBar->setValue(0);
    ui->LogBox->clear();

    if ( test::CURRENT_PARAMS )
        ui->LogBox->append( test::CURRENT_PARAMS->ToString() );

    test::ParamsToFile( "last_test_settings.isx", *test::CURRENT_PARAMS );

    mWorker.reset( new Worker());
    QObject::connect( mWorker.get(), &Worker::to_log, ui->LogBox, &QTextBrowser::append );
    QObject::connect( mWorker.get(), &Worker::progress, this, &TestRunner::on_progress );
    QObject::connect( mWorker.get(), &Worker::started, this, &TestRunner::on_test_start );
    QObject::connect( mWorker.get(), &Worker::finished, this, &TestRunner::on_test_stop );
    QObject::connect( mWorker.get(), &Worker::to_exec, this, &TestRunner::exec );
    mWorker->start();
}

void TestRunner::StopWorker()
{
    if ( mWorker.get() )
    {
        mWorker->stop();
        QObject::disconnect( mWorker.get(), &Worker::to_log, ui->LogBox, &QTextBrowser::append );
        QObject::disconnect( mWorker.get(), &Worker::progress, this, &TestRunner::on_progress );
        QObject::disconnect( mWorker.get(), &Worker::started, this, &TestRunner::on_test_start );
        QObject::disconnect( mWorker.get(), &Worker::finished, this, &TestRunner::on_test_stop );
        QObject::disconnect( mWorker.get(), &Worker::to_exec, this, &TestRunner::exec );
        mWorker.reset();
    }
    ui->Start->setEnabled( true );
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

void TestRunner::on_test_start()
{
    ui->Start->setEnabled( false );
}
void TestRunner::on_test_stop()
{
    ui->Start->setEnabled( true );
}

void TestRunner::on_SaveTest_clicked()
{
    QString file_name;
    QFileDialog dlg;
    dlg.setFileMode( QFileDialog::AnyFile );
    dlg.setDirectory( app::Settings::Instance().TestPath() );
    dlg.setNameFilter( "Параметры испытаний (*.isx )" );
    dlg.setAcceptMode( QFileDialog::AcceptSave );
    dlg.setViewMode( QFileDialog::Detail );
    if ( dlg.exec() )
        file_name = dlg.selectedFiles().front();
    if ( !file_name.isEmpty() )
    {
        if ( !file_name.endsWith(".isx", Qt::CaseInsensitive) )
            file_name += ".isx";
        test::ParamsToFile( file_name, *test::CURRENT_PARAMS );
    }
}

void TestRunner::exec( Functor func )
{
    if ( func )
        func();
}

Worker::Worker():
    mStopSignal(false)
{}
void Worker::run()
{
    mStopSignal = false;
    foreach (test::Test* to_run, test::CURRENT_PARAMS->TestCase())
    {
        if (mStopSignal)
            break;
        LogIt( "Запущен тест: " + to_run->Name() );
        bool result = to_run->Run( std::bind( &Worker::LaunchIt, this, std::placeholders::_1 ),
                                   std::bind( &Worker::LogIt, this, std::placeholders::_1 ),
                                   mStopSignal );
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

void Worker::LaunchIt( Functor func )
{
    emit to_exec( func );
}

void TestRunner::on_Results_clicked()
{
    if ( mChildWindow.get() )
        QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(show()) );
    mChildWindow.reset( new Viewer() );
    QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(show()) );
    mChildWindow->show();
}
