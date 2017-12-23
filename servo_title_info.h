#ifndef SERVO_TITLE_INFO_H
#define SERVO_TITLE_INFO_H

#include "child_widget.h"
#include <memory>

namespace Ui {
class ServoTitleInfo;
}

class ServoTitleInfo : public ChildWidget
{
    Q_OBJECT

public:
    explicit ServoTitleInfo(bool new_mode, QWidget *parent = 0);
    ~ServoTitleInfo();

private:
    Ui::ServoTitleInfo *ui;
    std::unique_ptr< ChildWidget > mChildWindow;
    bool mNewMode;

    bool SaveInputParams();
    void FromParams();
    void CheckRights();
    void OnLogin();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_RaspredControl_activated(int index);
    void on_ControlSignal_activated(int index);
    void on_ControlType_activated(int index);
    void on_TestChA_clicked();
    void on_TestChB_clicked();
    void on_ControlReelChA_activated(const QString &arg1);
    void on_ControlReelChB_activated(const QString &arg1);
    void on_PosCount_valueChanged(int arg1);
    void on_OutputType_activated(int index);
};

#endif // SERVO_TITLE_INFO_H
