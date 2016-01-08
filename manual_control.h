#ifndef MANUAL_CONTROL_H
#define MANUAL_CONTROL_H

#include <QWidget>

namespace Ui {
class ManualControl;
}

class ManualControl : public QWidget
{
    Q_OBJECT

public:
    explicit ManualControl(QWidget *parent = 0);
    ~ManualControl();

private:
    Ui::ManualControl *ui;

    void closeEvent(QCloseEvent *);

signals:
    void closed();
};

#endif // MANUAL_CONTROL_H
