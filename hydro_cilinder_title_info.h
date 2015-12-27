#pragma once
#include <QWidget>
#include <memory>

namespace Ui {
class HydroCilinderTitleInfo;
}

class HydroCilinderTitleInfo : public QWidget
{
    Q_OBJECT

public:
    explicit HydroCilinderTitleInfo(bool new_mode, QWidget *parent = 0);
    ~HydroCilinderTitleInfo();

private:
    Ui::HydroCilinderTitleInfo *ui;

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
