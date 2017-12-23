#ifndef PUMP_TEST_INFO_H
#define PUMP_TEST_INFO_H

#include "child_widget.h"
#include <memory>

namespace Ui {
class Pump_test_info;
}

class PumpTestInfo : public ChildWidget
{
   Q_OBJECT

public:
   explicit PumpTestInfo(bool new_mode, QWidget *parent = 0);
   ~PumpTestInfo();

private:
   Ui::Pump_test_info *ui;
   std::unique_ptr< QWidget > mChildWindow;
   bool mNewMode;

   bool SaveInputParams();
   void FromParams();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // PUMP_TEST_INFO_H
