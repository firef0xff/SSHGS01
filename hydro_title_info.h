#ifndef TITLE_INFO_H
#define TITLE_INFO_H

#include "child_widget.h"
#include <memory>

namespace Ui {
class HydroTitleInfo;
}

class HydroTitleInfo : public ChildWidget
{
    Q_OBJECT

public:
    explicit HydroTitleInfo(bool new_mode, QWidget *parent = 0);
    ~HydroTitleInfo();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_ControlType_activated(int index);

//    void on_GsType_activated(int index);

    void on_PosCount_valueChanged(int arg1);

private:
    Ui::HydroTitleInfo *ui;
    std::unique_ptr< ChildWidget > mChildWindow;
    bool mNewMode;

    bool SaveInputParams();
    void FromParams();
    void CheckRights();
    void OnLogin();

};

#endif // TITLE_INFO_H
