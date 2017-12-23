#pragma once
#include "child_widget.h"
#include <memory>

namespace Ui {
class ControlPanelsTitleInfo;
}

class ControlPanelsTitleInfo : public ChildWidget
{
    Q_OBJECT

public:
    explicit ControlPanelsTitleInfo(bool new_mode, QWidget *parent = 0);
    ~ControlPanelsTitleInfo();

private:
    Ui::ControlPanelsTitleInfo *ui;
    std::unique_ptr< ChildWidget > mChildWindow;
    bool mNewMode;

    bool SaveInputParams();
    void FromParams();
    void CheckRights();
    void OnLogin();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
