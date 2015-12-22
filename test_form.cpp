#include "test_form.h"
#include "ui_test_form.h"
#include "test_case/test_case.h"
#include "test_case/test.h"
#include "test_runner.h"
#include "test_case/test_params.h"
#include <QCheckBox>

TestForm::TestForm( bool new_mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestForm)
{
    ui->setupUi( this );

    foreach ( auto test_ptr, test::CURRENT_PARAMS->TestCollection().Tests() )
    {
        ControlPtr ptr( new QCheckBox( ui->scrollAreaWidgetContents ) );
        ptr->setObjectName( QStringLiteral("TestBox") + QString::number( test_ptr->Number() ) );
        ptr->setProperty("object", QVariant::fromValue( test_ptr ) );
        ptr->setText( test_ptr->Name() );

        bool check = true;
        if ( !new_mode )
        {
            test::Parameters::TestsList const& lnk = test::CURRENT_PARAMS->TestCase();
            bool find = false;
            foreach ( test::Test* ptr , lnk )
            {
                if ( ptr->ID() == test_ptr->ID() )
                {
                    find = true;
                    break;
                }
            }
            check *= find;
        }
        ptr->setChecked( check );

        ui->gridLayout->addWidget( ptr.get() , test_ptr->Number() - 1 , 0, 1, 1 );
        mControls.append( ptr );
        mChilds.append( ptr );
    }

    mVertical = new QSpacerItem( 20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding );
    mHorisontal = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );

    ui->gridLayout->addItem( mVertical, ui->gridLayout->rowCount() + 1, 0, 1, 1 );
    ui->gridLayout->addItem( mHorisontal, 0, ui->gridLayout->rowCount() + 1, 1, 1 );        
}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}

void TestForm::on_buttonBox_rejected()
{
    close();
}

void TestForm::on_buttonBox_accepted()
{
    test::Parameters::TestsList selected;
    foreach (ControlPtr check_box, mControls)
    {
        if ( check_box->isChecked() )
        {
            test::Test* test_ptr = check_box->property("object").value< test::Test* >();
            selected.append( test_ptr );
        }
    }
    if ( mChildWindow.get() )
        QObject::disconnect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );
    test::CURRENT_PARAMS->TestCase( selected );
    mChildWindow.reset( new TestRunner() );
    QObject::connect( mChildWindow.get(), SIGNAL(closed()), this, SLOT(close()) );

    hide();
    mChildWindow->show();    
}
