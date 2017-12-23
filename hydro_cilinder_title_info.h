#pragma once
#include "child_widget.h"
#include <memory>

namespace Ui {
class HydroCilinderTitleInfo;
}

class HydroCilinderTitleInfo : public ChildWidget
{
    Q_OBJECT

public:
    explicit HydroCilinderTitleInfo(bool new_mode, QWidget *parent = 0);
    ~HydroCilinderTitleInfo();

private:
    Ui::HydroCilinderTitleInfo *ui;

    std::unique_ptr< QWidget > mChildWindow;
    bool mNewMode;

    bool SaveInputParams();
    void FromParams();
    void CheckRights();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
