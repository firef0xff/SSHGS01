#ifndef PUMP_TITLE_INFO_H
#define PUMP_TITLE_INFO_H

#include <QWidget>

namespace Ui {
class Pump_title_info;
}

class Pump_title_info : public QWidget
{
   Q_OBJECT

public:
   explicit Pump_title_info(QWidget *parent = 0);
   ~Pump_title_info();

private:
   Ui::Pump_title_info *ui;
};

#endif // PUMP_TITLE_INFO_H
