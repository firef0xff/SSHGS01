#ifndef PUMP_TITLE_INFO_H
#define PUMP_TITLE_INFO_H

#include "child_widget.h"
#include <memory>

namespace Ui {
class Pump_title_info;
}

class PumpTitleInfo : public ChildWidget
{
   Q_OBJECT

public:
   explicit PumpTitleInfo(bool new_mode, QWidget *parent = 0);
   ~PumpTitleInfo();

private:
   Ui::Pump_title_info *ui;

   std::unique_ptr< QWidget > mChildWindow;
   bool mNewMode;

   bool SaveInputParams();
   void FromParams();

private slots:
    void on_AnsverBox_accepted();
    void on_AnsverBox_rejected();
};

#endif // PUMP_TITLE_INFO_H
