#pragma once
#include <QWidget>
#include <memory>
#include <functional>

namespace Ui {
class StandParams;
}

class StandParams : public QWidget
{
    Q_OBJECT

public:
    explicit StandParams( bool new_mode, QWidget *parent = 0);
    ~StandParams();

    void SetCallback( std::function< void() > func );

private:
    Ui::StandParams *ui;
    std::unique_ptr< QWidget > mChildWindow;
    bool mNewMode;
    std::function< void() > mCustomAction;

    void closeEvent(QCloseEvent *);
    bool SaveInputParams();
    void FromParams();
    void CheckRights();

signals:
    void closed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
