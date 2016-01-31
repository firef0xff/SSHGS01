#pragma once
#include <QWidget>
#include <QAbstractButton>
#include "cpu/cpu_memory.h"
#include <QThread>
#include <QLabel>
#include <QLCDNumber>

namespace Ui {
class ManualControl;
}

class ManualControlUpdater : public QThread
{
    Q_OBJECT
public:
    ManualControlUpdater();
    void run();
    void stop();
private:
    mutable bool mStopSignal;
signals:
    void update();
};

void UpdateMark  ( QLabel *btn, bool draw, QColor color );

class ManualControl : public QWidget
{
    Q_OBJECT

public:
    explicit ManualControl(QWidget *parent = 0);
    ~ManualControl();

private:
    Ui::ManualControl *ui;
    cpu::data::DB31   &mTaskMode;
    cpu::data::DB50   &mParams;
    cpu::data::M1     &mControlBits;
    cpu::data::I1     &mErrorBits;
    ManualControlUpdater Updater;

    void closeEvent( QCloseEvent *e );
    void showEvent ( QShowEvent *e );
    void hideEvent ( QHideEvent *e );

    void SynkControls();

    void UpdateButton( QAbstractButton *btn, bool checked );    
    void UpdateValue  ( QLCDNumber *ctrl, float const& value );

    void UpdateMarks(); //обновление индикации
    void UpdateData();  //обновление данных на сетке

    void act_CB_clicked();
    void act_CR_clicked();
signals:
    void closed();
private slots:
    void onUpdateControls();
    void on_KM1_clicked();
    void on_KM2_clicked();
    void on_YA5_clicked();
    void on_KM3_clicked();
    void on_YA1_clicked();
    void on_YA2_clicked();
    void on_KM4_clicked();
    void on_YA3_clicked();
    void on_KM9_clicked();
    void on_YA6_clicked();
    void on_YA13_clicked();
    void on_YA10_clicked();
    void on_YA14_clicked();
    void on_YA8_clicked();
    void on_YA15_clicked();
    void on_YA9_clicked();
    void on_YA16_clicked();
    void on_YA22_clicked();
    void on_YA17_clicked();
    void on_KM6_clicked();
    void on_KM7_clicked();
    void on_KM8_clicked();
    void on_KM10_clicked();
    void on_KM5_clicked();
    void on_YA12_clicked();
    void on_YA23_clicked();
    void on_YA11_clicked();
    void on_YA7_clicked();
    void on_YA18_clicked();
    void on_YA19_clicked();
    void on_YA20_clicked();
    void on_YA21_clicked();
    void on_YA24_clicked();
    void on_YA25_clicked();
//    void on_YA4_clicked();
    void on_Accept_clicked(); // передача задания в контроллер
    void on_CB_clicked();
    void on_CR_clicked();
    void on_RC1_clicked();
    void on_RC2_clicked();
    void on_ONRA_clicked();
    void on_ONRB_clicked();
    void on_SigLevel_currentIndexChanged(const QString &arg1);
};
