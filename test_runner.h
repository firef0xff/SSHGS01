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

class TestRunner : public QWidget
{
    Q_OBJECT

public:
    typedef std::shared_ptr< QLabel > ControlPtr;
    typedef QList< test::Test* > TestCase;
    explicit TestRunner( TestCase test_case, QWidget *parent = 0);
    ~TestRunner();

private:
    Ui::TestRunner *ui;
    TestCase mTestCase;
    QList< ControlPtr > mChilds;
    std::unique_ptr< Worker > mWorker;

    void closeEvent(QCloseEvent *e);
    void StopWorker();

signals:
    closed();
private slots:
    void on_Start_clicked();
    void on_Abort_clicked();
    void on_Cancel_clicked();
    void on_progress();
};

class Worker : public QThread
{
    Q_OBJECT
public:
    Worker( TestRunner::TestCase const& test_case );
    void run();
    void stop();
private:
    mutable bool mStopSignal;
    TestRunner::TestCase mTestCase;
    void LogIt( QString const& str );
signals:
    to_log( QString const& );
    progress();

};

#endif // TEST_RUNNER_H
