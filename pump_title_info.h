#ifndef PUMP_TITLE_INFO_H
#define PUMP_TITLE_INFO_H

#include <QWidget>
#include <memory>

namespace Ui {
class Pump_title_info;
}

class PumpTitleInfo : public QWidget
{
   Q_OBJECT

public:
   explicit PumpTitleInfo(bool new_mode, QWidget *parent = 0);
   ~PumpTitleInfo();

private:
   Ui::Pump_title_info *ui;

   void closeEvent(QCloseEvent *);
   std::unique_ptr< QWidget > mChildWindow;
   bool mNewMode;

   bool SaveInputParams();
   void FromParams();

signals:
    void closed();

private slots:
    void on_AnsverBox_accepted();
    void on_AnsverBox_rejected();
};

#endif // PUMP_TITLE_INFO_H
