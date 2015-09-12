#pragma once
#include <QMainWindow>
#include <memory>
#include "RoundDial/round_dial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef std::shared_ptr< ff0x::RoundDial > ControlPtr;
    typedef std::unique_ptr< QWidget > ChildPtr;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:

    void closeEvent(QCloseEvent *e);

    void StartHydroTest( bool new_test );
    void StartServoTest( bool new_test );
    void StartManualControl();
    void SettringsFileStorage ();

    void ShowChildWindow( ChildPtr child );


    Ui::MainWindow *ui;
    ChildPtr mChildWindow;

    QList< ControlPtr > mChildControls;

    //барометр
    ff0x::RoundDial* DD1;
    ff0x::RoundDial* DD2;
    ff0x::RoundDial* DD3;

    //барометр
    ff0x::RoundDial* T;
    ff0x::RoundDial* X;
    ff0x::RoundDial* M1M2;

    //расходомеры
    ff0x::RoundDial* R_P;
    ff0x::RoundDial* R_Lost;

    //цилиндр
    ff0x::RoundDial* R_Cilindr;

    //расходомер

private slots:


    void on_TestCase1_clicked();
    void on_TestCase2_clicked();
    void on_ManualControl_clicked();

    void enable_modes( bool enabled = true );
    void on_act_test_case1_triggered();
    void on_act_test_case2_triggered();
    void on_load_isp_params_triggered();
    void on_file_storage_triggered();
};
