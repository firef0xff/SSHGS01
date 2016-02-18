#include "users.h"
#include "ui_users.h"


QString CastToCombo( app::UserLevel lvl )
{
    switch (lvl)
    {
        case app::Admin:
            return "Администратор";
        case app::Master:
            return "Мастер";
        case app::User:
            return "Пользователь";
    }
    return "Пользователь";
}

app::UserLevel CastToLevel( QString const& str )
{
    if ( !str.compare( "Администратор", Qt::CaseInsensitive ) )
        return app::Admin;
    else if ( !str.compare( "Мастер", Qt::CaseInsensitive ) )
        return app::Master;
    else if ( !str.compare( "Пользователь", Qt::CaseInsensitive ) )
        return app::User;
    return app::User;
}

Users::Users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Users)
{
    ui->setupUi(this);
    mUsers = app::Settings::Instance().Users();
    ShowData();
}

Users::~Users()
{
    delete ui;
}

void Users::closeEvent(QCloseEvent *e)
{
    app::Settings::Instance().Users( mUsers );
    app::Settings::Instance().Save();
    QWidget::closeEvent( e );
    emit closed();
}

void Users::AutoWidth(QTableWidget *sg )
{
    QFontMetrics metrix( sg->font() );
    for ( int col = 0; col < sg->columnCount(); ++col )
    {
        int max_width = metrix.width(sg->horizontalHeaderItem(col)->text());
        for ( int row = 0; row < sg->rowCount(); ++row )
        {
            auto width = metrix.width( sg->item(row, col)->text() );
            if (width > max_width)
                max_width = width;
        }

        sg->setColumnWidth( col, max_width+20 );
    }
}
void Users::ShowData()
{
    mUpdateGreed = true;
    ui->UsersGreed->clearContents();
    ui->UsersGreed->setRowCount(0);
    std::for_each (mUsers.begin(), mUsers.end(), [this]( app::UserInfo& usr )
    {
        ui->UsersGreed->setRowCount( ui->UsersGreed->rowCount() +1 );

        QTableWidgetItem *item_ptr = new QTableWidgetItem( usr.Login );
        item_ptr->setData(Qt::UserRole, QVariant::fromValue( &usr ));
        ui->UsersGreed->setItem( ui->UsersGreed->rowCount()-1, 0, item_ptr );
    });
    AutoWidth( ui->UsersGreed );
    mUpdateGreed = false;
    ui->Pass->setText("");
    ui->UserName->setText("");
    ui->UserLevel->setCurrentIndex( -1 );
    on_UsersGreed_itemSelectionChanged();
}

void Users::on_UsersGreed_itemSelectionChanged()
{
    auto items = ui->UsersGreed->selectedItems();
    if ( items.isEmpty() )
    {
        mCurrentItem = nullptr;
    }
    else
    {
        mCurrentItem = items.front();
    }

    if ( mCurrentItem )
    {
        app::UserInfo* user = mCurrentItem->data(Qt::UserRole).value< app::UserInfo* >();
        ui->UserName->setText( user->Login );
        ui->Pass->setText("");
        ui->UserLevel->setCurrentText( CastToCombo( user->level ) );
    }
}

void Users::on_Add_clicked()
{
    if (!ui->UserName->text().isEmpty() && !ui->Pass->text().isEmpty())
    {
        app::UserInfo u;
        u.Login = ui->UserName->text();
        u.SetPass( ui->Pass->text() );
        u.level = CastToLevel( ui->UserLevel->currentText() );
        mUsers.push_back( u );
        ShowData();
    }
}

void Users::on_Remove_clicked()
{
    if (!mCurrentItem)
        return;
    app::UserInfo* user = mCurrentItem->data(Qt::UserRole).value< app::UserInfo* >();
    auto it = std::find_if( mUsers.begin(), mUsers.end(),
    [&user]( app::UserInfo const& u)
    {
        return &u == user;
    } );
    if ( it != mUsers.end() )
    {
        mUsers.erase( it );
        ShowData();
    }
}

void Users::on_Edit_clicked()
{
    if (!mCurrentItem)
        return;

    if (!ui->UserName->text().isEmpty() )
    {
        app::UserInfo* user = mCurrentItem->data(Qt::UserRole).value< app::UserInfo* >();
        user->Login = ui->UserName->text();
        if ( !ui->Pass->text().isEmpty() )
        {
            user->SetPass( ui->Pass->text() );
        }
        user->level = CastToLevel( ui->UserLevel->currentText() );
        ShowData();
    }
}
