#include "test_runner.h"
#include "ui_test_runner.h"
#include "test_case/test.h"
#include "test_case/test_params.h"
#include <QLabel>
#include <QThread>
#include <QFileDialog>
#include "settings/settings.h"
#include "viewer.h"
#include "QDateTime"
#include "test_case/test_params.h"
#include "settings/settings.h"
#include <thread>


TestRunner::TestRunner(QWidget *parent) :
    ChildWidget(parent),
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
    CheckRights();
}

TestRunner::~TestRunner()
{
    close();
    delete ui;
}

void TestRunner::closeEvent(QCloseEvent *e)
{
    StopWorker();
    ChildWidget::closeEvent( e );
}

void TestRunner::CheckRights()
{
   if ( app::Settings::Instance().UserAccess() == app::UserLevel::Uncknown )
   {
      StopWorker();
      ui->Start->setEnabled(false);
   }
   else
      ui->Start->setEnabled(true);
}
void TestRunner::OnLogin()
{
   if (isHidden())
      return;
   CheckRights();
}

void TestRunner::on_Start_clicked()
{    
    std::unique_lock<std::mutex> lock( mWorkerMutex );
    if ( mWorker.get() )
        StopWorker();

    if ( app::Settings::Instance().UserAccess() == app::UserLevel::Uncknown )
       return;

    ui->progressBar->reset();
    ui->progressBar->setRange( 0, test::CURRENT_PARAMS->TestCase().size() );
    ui->progressBar->setValue(0);
    ui->LogBox->clear();

    if ( test::CURRENT_PARAMS )
        ui->LogBox->append( test::CURRENT_PARAMS->ToString() );

    test::CURRENT_PARAMS->Date( QDateTime::currentDateTime() );
    test::CURRENT_PARAMS->User( app::Settings::Instance().User() );
    test::CURRENT_PARAMS->ReportType( ui->ReportType->currentText() );

    mWorker.reset( new Worker(/* ui->ReportType->currentText().compare( "Эталон", Qt::CaseInsensitive ) == 0 */) );
    QObject::connect( mWorker.get(), &Worker::to_log, ui->LogBox, &QTextBrowser::append );
    QObject::connect( mWorker.get(), &Worker::progress, this, &TestRunner::on_progress );
    QObject::connect( mWorker.get(), &Worker::started, this, &TestRunner::on_test_start );
    QObject::connect( mWorker.get(), &Worker::finished, this, &TestRunner::on_test_stop );
    QObject::connect( mWorker.get(), &Worker::to_exec, this, &TestRunner::exec );
    mWorker->start();
    test::ParamsToFile( "last_test_settings.isx", *test::CURRENT_PARAMS );
}

void TestRunner::StopWorker()
{
   std::thread t(
   [this]()
   {
      std::unique_lock<std::mutex> lock( mWorkerMutex );
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
      if ( app::Settings::Instance().UserAccess() != app::UserLevel::Uncknown )
        ui->Start->setEnabled( true );
   } );
   t.detach();
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
   if ( app::Settings::Instance().UserAccess() != app::UserLevel::Uncknown )
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

Worker::Worker(/* bool etalone */):
    mStopSignal(false)/*,
    mEtalone(etalone)*/
{}
void Worker::run()
{
    mStopSignal = false;
    try
    {
        test::CURRENT_PARAMS->StendInit();
    }catch( std::exception const& e )
    {
        LogIt( e.what() );
        return;
    }
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
    try
    {
        test::CURRENT_PARAMS->StendDeInit();
    }catch( std::exception const& e )
    {
        LogIt( e.what() );
        return;
    }

    /*if (mEtalone)
    {
        test::SaveToEtalone( *test::CURRENT_PARAMS );
    }*/
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
    {
        QObject::disconnect( mChildWindow.get(), &ChildWidget::closed, this, &ChildWidget::show );
        QObject::disconnect( this, &ChildWidget::login, mChildWindow.get(), &ChildWidget::on_login );
    }
    mChildWindow.reset( new Viewer() );
    QObject::connect( mChildWindow.get(), &ChildWidget::closed, this, &ChildWidget::show );
    QObject::connect( this, &ChildWidget::login, mChildWindow.get(), &ChildWidget::on_login );

    mChildWindow->show();
}
