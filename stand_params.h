#ifndef STAND_PARAMS_H
#define STAND_PARAMS_H

#include <QWidget>

namespace Ui {
class StandParams;
}

class StandParams : public QWidget
{
    Q_OBJECT

public:
    explicit StandParams(QWidget *parent = 0);
    ~StandParams();

private:
    Ui::StandParams *ui;
};

#endif // STAND_PARAMS_H
