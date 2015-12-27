#pragma once
#include <QWidget>
#include "device_collection.h"


namespace Ui {
class DeviceCollectionWND;
}

class QTableWidgetItem;
class QTableWidget;

class DeviceCollectionWND : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceCollectionWND ( examinee::DeviceCollection& collection, QWidget *parent = 0 );
    ~DeviceCollectionWND();

private:
    void closeEvent(QCloseEvent *e);
    void ShowData();
    void AutoWidth( QTableWidget *sg );
    void MoveSelectedRows( bool up );

    Ui::DeviceCollectionWND *ui;
    examinee::DeviceCollection &mCollection;
    examinee::DeviceCollection::DataType mDevices;
    examinee::DeviceCollection::DataType mDevicesNew;

    bool mUpdateGreed;
    QTableWidgetItem* mCurrentItem;

signals:
    void closed();
private slots:
    void on_ControlType_activated(int index);
    void on_AddBtn_clicked();
    void on_RemoveBtn_clicked();
    void on_Devices_cellChanged(int row, int column);
    void on_Up_clicked();
    void on_Down_clicked();
    void on_Devices_itemSelectionChanged();
    void on_MaxPressure_editingFinished();
    void on_MinPressure_editingFinished();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};
