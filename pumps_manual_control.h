#ifndef PUMPS_MANUAL_CONTROL_H
#define PUMPS_MANUAL_CONTROL_H

#include <QWidget>

namespace Ui {
class pumps_manual_control;
}

class pumps_manual_control : public QWidget
{
   Q_OBJECT

public:
   explicit pumps_manual_control(QWidget *parent = 0);
   ~pumps_manual_control();

private:
   Ui::pumps_manual_control *ui;
};

#endif // PUMPS_MANUAL_CONTROL_H
