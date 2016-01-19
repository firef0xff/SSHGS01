#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <QWidget>
#include <QThread>
#include <memory>
#include <functional>

namespace Ui {
class TestRunner;
}

namespace test
{
class Test;
}

class QLabel;
class Worker;

typedef std::function< void() > Functor;
class TestRunner : public QWidget
{
    Q_OBJECT

public:
    typedef std::shared_ptr< QLabel > ControlPtr;    
    explicit TestRunner( QWidget *parent = 0);
    ~TestRunner();

private:
    Ui::TestRunner *ui;
    QList< ControlPtr > mChilds;
    std::unique_ptr< Worker > mWorker;
    std::unique_ptr< QWidget > mChildWindow;

    void closeEvent(QCloseEvent *e);
    void StopWorker();

signals:
    void closed();
private slots:
    void on_Start_clicked();
    void on_Abort_clicked();
    void on_Cancel_clicked();
    void on_progress();
    void on_test_start();
    void on_test_stop();
    void on_SaveTest_clicked();
    void on_Results_clicked();

    void exec( Functor );
};

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker( /*bool etalone*/ );
    void run();
    void stop();
private:
    mutable bool mStopSignal;
//    bool mEtalone;
    void LogIt( QString const& str );
    void LaunchIt( Functor  func );
signals:
    void to_log( QString const& );
    void to_exec( Functor );
    void progress();

};

Q_DECLARE_METATYPE(Functor)
#endif // TEST_RUNNER_H
