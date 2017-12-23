#include "device_collection_wnd.h"
#include "ui_device_collection_wnd.h"
#include "device.h"

DeviceCollectionWND::DeviceCollectionWND ( examinee::DeviceCollection& collection, QWidget *parent) :
    ChildWidget(parent),
    ui(new Ui::DeviceCollectionWND),
    mCollection( collection ),
    mDevices( mCollection.Get() )
{
    ui->setupUi(this);

    ui->MinPressure->setValidator( new QIntValidator( 10, 30, this ) );
    ui->MaxPressure->setValidator( new QIntValidator( 100, 300, this ) );
    ShowData();
}

DeviceCollectionWND::~DeviceCollectionWND()
{
    delete ui;
}

void DeviceCollectionWND::ShowData()
{
    mUpdateGreed = true;
    ui->Devices->clearContents();
    ui->Devices->setRowCount(0);
    foreach (auto ptr, mDevices)
    {
        ui->Devices->setRowCount( ui->Devices->rowCount() +1 );

        QTableWidgetItem *item_ptr = new QTableWidgetItem( ptr->Name() );
        item_ptr->setData(Qt::UserRole, QVariant::fromValue( ptr ));
        ui->Devices->setItem( ui->Devices->rowCount()-1, 0, item_ptr );

        item_ptr = new QTableWidgetItem( ptr->Manufacturer() );
        item_ptr->setData(Qt::UserRole, QVariant::fromValue( ptr ));
        ui->Devices->setItem(ui->Devices->rowCount()-1, 1, item_ptr );
    }
    AutoWidth( ui->Devices );
    mUpdateGreed = false;
    on_Devices_itemSelectionChanged();
}

void DeviceCollectionWND::AutoWidth(QTableWidget *sg )
{
    QFontMetrics metrix( sg->font() );
    for ( int col = 0; col < sg->columnCount(); ++col )
    {
        int max_width = 0;
        for ( int row = 0; row < sg->rowCount(); ++row )
        {
            auto width = metrix.width( sg->item(row, col)->text() );
            if (width > max_width)
                max_width = width;
        }

        sg->setColumnWidth( col, max_width+20 );
    }

}

void DeviceCollectionWND::on_ControlType_activated(int index)
{
    if ( mCurrentItem )
    {
        auto device = mCurrentItem->data(Qt::UserRole).value< examinee::Device* >();
        auto prop = device->Properties();

        prop["ТипУправления"] = ui->ControlType->itemText( index );

        device->Properties( prop );
    }

    if ( index > 0 )
    {
        if ( ui->MaxPressure->text().isEmpty() )
        {
            ui->MaxPressure->setText( "100" );
            on_MaxPressure_editingFinished();
        }
        if ( ui->MinPressure->text().isEmpty() )
        {
            ui->MinPressure->setText( "10" );
            on_MinPressure_editingFinished();
        }
    }
    else
    {
        if ( mCurrentItem )
        {
            auto device = mCurrentItem->data(Qt::UserRole).value< examinee::Device* >();
            auto prop = device->Properties();

            auto it = prop.find( "ДавлениеУправления.Максимум" );
            if ( it != prop.end() )
                prop.erase( it );

            it = prop.find( "ДавлениеУправления.Минимум" );
                    if ( it != prop.end() )
                        prop.erase( it );
        }
        ui->MaxPressure->setText( "" );
        ui->MinPressure->setText( "" );
    }

    ui->MaxPressure->setEnabled( index > 0 );
    ui->MinPressure->setEnabled( index > 0 );
}

void DeviceCollectionWND::on_MaxPressure_editingFinished()
{
    if ( mCurrentItem )
    {
        auto device = mCurrentItem->data(Qt::UserRole).value< examinee::Device* >();
        auto prop = device->Properties();

        prop["ДавлениеУправления.Максимум"] = ui->MaxPressure->text().toDouble();

        device->Properties( prop );
    }
}

void DeviceCollectionWND::on_MinPressure_editingFinished()
{
    if ( mCurrentItem )
    {
        auto device = mCurrentItem->data(Qt::UserRole).value< examinee::Device* >();
        auto prop = device->Properties();

        prop["ДавлениеУправления.Минимум"] = ui->MinPressure->text().toDouble();

        device->Properties( prop );
    }
}

void DeviceCollectionWND::on_AddBtn_clicked()
{
    auto ptr = new examinee::Device();
    mDevices.push_back( ptr );
    mDevicesNew.push_back( ptr );
    ShowData();
}

void DeviceCollectionWND::on_RemoveBtn_clicked()
{
    auto items = ui->Devices->selectedItems();

    QSet< examinee::Device* > to_remove;

    foreach (auto item, items)
    {
        to_remove.insert( item->data(Qt::UserRole).value<examinee::Device*>() );
    }

    for (auto it = mDevices.begin(); it!=mDevices.end(); )
    {
        if ( to_remove.find( *it ) != to_remove.end() )
        {
            it = mDevices.erase( it );
        }
        else
            ++it;
    }

    for (auto it = mDevicesNew.begin(); it!=mDevicesNew.end(); )
    {
        if ( to_remove.find( *it ) != to_remove.end() )
        {
            delete *it;
            it = mDevicesNew.erase( it );
        }
        else
            ++it;
    }

    ShowData();
}

void DeviceCollectionWND::on_Devices_cellChanged(int row, int column)
{
    if ( mUpdateGreed )
        return;

    auto item = ui->Devices->item( row, column );
    auto device = item->data(Qt::UserRole).value< examinee::Device* >();
    switch ( column )
    {
    case 0:
        device->Name( item->text() );
        break;
    case 1:
        device->Manufacturer( item->text() );
        break;
    default:
        break;
    }

}


void DeviceCollectionWND::MoveSelectedRows( bool up )
{
    auto items = ui->Devices->selectedItems();

    QSet< examinee::Device* > to_move;

    foreach (auto item, items)
    {
        to_move.insert( item->data(Qt::UserRole).value<examinee::Device*>() );
    }

    typedef std::list< examinee::Device* > DeviceList;
    DeviceList devices = mDevices.toStdList();

    if ( up )
    {
        for ( auto it = devices.begin(); it!=devices.end(); ++it )
        {
            if ( to_move.find( *it ) != to_move.end() )
            {
                // найден итеротор элемента который нужно подвинуть
                // вычисляем итератор с которым  нужно его поменять
                auto point = devices.end();

                if ( it == devices.begin() )
                    break;
                else
                {
                    point = it;
                    --point;
                }
                std::swap( *it, *point );
            }
        }
    }
    else
    {
        for ( auto it = devices.rbegin(); it!=devices.rend(); ++it )
        {
            if ( to_move.find( *it ) != to_move.end() )
            {
                // найден итеротор элемента который нужно подвинуть
                // вычисляем итератор с которым  нужно его поменять
                auto point = devices.rend();

                if ( it == devices.rbegin() )
                    break;
                else
                {
                    point = it;
                    --point;
                }
                std::swap( *it, *point );
            }
        }
    }

    mDevices = mDevices.fromStdList( devices );
    ShowData();

    //созранить выделение
    for ( int i = 0; i < ui->Devices->rowCount(); ++i )
    {
        auto item = ui->Devices->item( i, 0 );
        auto device = item->data(Qt::UserRole).value<examinee::Device*>();

        if ( to_move.find( device ) != to_move.end() )
        {
            QModelIndex index = ui->Devices->model()->index( i, 0 );

            ui->Devices->selectionModel()->select( index, QItemSelectionModel::Select|QItemSelectionModel::Rows );
        }
    }
}

void DeviceCollectionWND::on_Up_clicked()
{
    MoveSelectedRows( true );
}
void DeviceCollectionWND::on_Down_clicked()
{
    MoveSelectedRows( false );
}

void DeviceCollectionWND::on_Devices_itemSelectionChanged()
{
    auto items = ui->Devices->selectedItems();
    if ( items.isEmpty() )
    {
        mCurrentItem = nullptr;
        ui->ControlType->setCurrentIndex( -1 ) ;
        ui->ControlType->setEnabled( false );
    }
    else
    {
        mCurrentItem = items.front();
        ui->ControlType->setEnabled( true );
    }

    if ( mCurrentItem )
    {
        auto device = mCurrentItem->data(Qt::UserRole).value< examinee::Device* >();

        auto it = device->Properties().find("ТипУправления" );
        if ( it != device->Properties().end())
            ui->ControlType->setCurrentIndex( ui->ControlType->findText( it.value().toString() ) );
        else
            ui->ControlType->setCurrentIndex( -1 );


        it = device->Properties().find("ДавлениеУправления.Максимум" );
        if ( it != device->Properties().end())
            ui->MaxPressure->setText( QString::number( uint32_t( it.value().toDouble() ) ) );
        else
            ui->MaxPressure->setText( "" );


        it = device->Properties().find("ДавлениеУправления.Минимум" );
        if ( it != device->Properties().end())
            ui->MinPressure->setText( QString::number( uint32_t( it.value().toDouble() ) ) );
        else
            ui->MinPressure->setText( "" );
    }
    on_ControlType_activated( ui->ControlType->currentIndex() );
}


void DeviceCollectionWND::on_buttonBox_accepted()
{
    mCollection.Set( mDevices );
    mCollection.Save();
    close();
}

void DeviceCollectionWND::on_buttonBox_rejected()
{
    close();
}
