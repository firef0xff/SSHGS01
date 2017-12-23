#pragma once
#include <child_widget.h>
#include <memory>
#include <functional>

namespace Ui {
class StandParams;
}

class StandParams : public ChildWidget
{
    Q_OBJECT

public:
    explicit StandParams( bool new_mode, QWidget *parent = 0);
    ~StandParams();

    void SetCallback( std::function< void() > func );

private:
    Ui::StandParams *ui;
    std::unique_ptr< ChildWidget > mChildWindow;
    bool mNewMode;
    std::function< void() > mCustomAction;

    void closeEvent(QCloseEvent *);
    bool SaveInputParams();
    void FromParams();
    void CheckRights();
    void OnLogin();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
