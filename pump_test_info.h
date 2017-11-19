#ifndef PUMP_TEST_INFO_H
#define PUMP_TEST_INFO_H

#include <QWidget>
#include <memory>

namespace Ui {
class Pump_test_info;
}

class PumpTestInfo : public QWidget
{
   Q_OBJECT

public:
   explicit PumpTestInfo(bool new_mode, QWidget *parent = 0);
   ~PumpTestInfo();

private:
   Ui::Pump_test_info *ui;
   std::unique_ptr< QWidget > mChildWindow;
   bool mNewMode;

   void closeEvent(QCloseEvent *e);
   bool SaveInputParams();
   void FromParams();

signals:
    void closed();
};

#endif // PUMP_TEST_INFO_H
