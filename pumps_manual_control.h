#ifndef PUMPS_MANUAL_CONTROL_H
#define PUMPS_MANUAL_CONTROL_H

#include <QWidget>
#include <QThread>
#include <QLabel>
#include <QAbstractButton>
#include "cpu/cpu_memory.h"

namespace Ui {
class pumps_manual_control;
}

class PumpsManualControlUpdater : public QThread
{
    Q_OBJECT
public:
    PumpsManualControlUpdater();
    void run();
    void stop();
private:
    mutable bool mStopSignal;
signals:
    void update();
};

class PumpsManualControl : public QWidget
{
   Q_OBJECT

public:
   explicit PumpsManualControl(QWidget *parent = 0);
   ~PumpsManualControl();

private:
   Ui::pumps_manual_control *ui;

   cpu::data::DB31   &mTaskMode;
   PumpsManualControlUpdater mUpdater;

   void closeEvent( QCloseEvent *e );
   void showEvent ( QShowEvent *e );
   void hideEvent ( QHideEvent *e );

   void UpdateMarks();
   void UpdateData();
   void SynkControls();

   void Start();
   void Stop();

   void UpdateButton( QAbstractButton *btn, bool checked );
private slots:
    void onUpdateControls();

    void on_EngineStartStop_clicked();

    void CheckDR1t();
    void on_YB7_val_returnPressed();
    void on_YB10_val_returnPressed();
    void on_YB9_val_returnPressed();
    void on_YB8_val_returnPressed();

private:

   void SetColor( QLabel *label, QColor cl );

};

#endif // PUMPS_MANUAL_CONTROL_H
