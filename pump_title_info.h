#ifndef PUMP_TITLE_INFO_H
#define PUMP_TITLE_INFO_H

#include "child_widget.h"
#include <memory>
#include "QCheckBox"

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

   std::unique_ptr< ChildWidget > mChildWindow;
   bool mNewMode;

   bool SaveInputParams();
   void FromParams();

   void OnLogin();
   void CheckRights();
private slots:
    void on_AnsverBox_accepted();
    void on_AnsverBox_rejected();

    void on_ConturA1_clicked();
    void on_ConturB1_clicked();
    void on_ConturC1_clicked();
    void on_ConturA2_clicked();
    void on_ConturB2_clicked();
    void on_ConturC2_clicked();

    void ProcessContur( QCheckBox* activated );
};

#endif // PUMP_TITLE_INFO_H
