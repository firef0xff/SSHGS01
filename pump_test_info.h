#ifndef PUMP_TEST_INFO_H
#define PUMP_TEST_INFO_H

#include <QWidget>

namespace Ui {
class Pump_test_info;
}

class Pump_test_info : public QWidget
{
   Q_OBJECT

public:
   explicit Pump_test_info(QWidget *parent = 0);
   ~Pump_test_info();

private:
   Ui::Pump_test_info *ui;
};

#endif // PUMP_TEST_INFO_H
