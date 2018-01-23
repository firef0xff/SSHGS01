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

    void OnYA27YB7Activate();

    void on_YA27_clicked();
    void on_YB7_clicked();
    void on_YB8_clicked();
    void on_YB9_clicked();
    void on_YB10_clicked();
    void on_YA28_clicked();

private:

   void SetColor( QLabel *label, QColor cl );

   void Activate_Gm(bool active);
   void Activate_M13( bool active );

   void Activate_LN_0( bool active );
   void Activate_LN_1( bool active );
   void Activate_LN_2( bool active );
   void Activate_LN_4( bool active );
   void Activate_LN_5( bool active );
   void Activate_LN_6( bool active );
   void Activate_LN_7( bool active );
   void Activate_LN_8( bool active );
   void Activate_LN_9( bool active );
   void Activate_LN_10( bool active );
   void Activate_LN_11( bool active );
   void Activate_LN_12( bool active );
   void Activate_LN_13( bool active );
   void Activate_LN_14( bool active );
   void Activate_LN_15( bool active );
   void Activate_LN_16( bool active );


};

#endif // PUMPS_MANUAL_CONTROL_H
