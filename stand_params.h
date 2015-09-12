#pragma once
#include <QWidget>
#include <memory>

namespace Ui {
class StandParams;
}

class StandParams : public QWidget
{
    Q_OBJECT

public:
    explicit StandParams( bool new_mode, QWidget *parent = 0);
    ~StandParams();

private:
    Ui::StandParams *ui;
    std::unique_ptr< QWidget > mChildWindow;
    bool mNewMode;

    void closeEvent(QCloseEvent *);
    bool SaveInputParams();
    void FromParams();

signals:
    void closed();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
