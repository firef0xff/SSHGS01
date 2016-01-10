#include "manual_control.h"
#include "ui_manual_control.h"

ManualControlUpdater::ManualControlUpdater():
    mStopSignal(false)
{}
void ManualControlUpdater::run()
{
    mStopSignal = false;
    while ( !mStopSignal )
    {
        cpu::CpuMemory::Instance().DB50.Read();
        cpu::CpuMemory::Instance().I1.Read();
        emit update();
        msleep(500);
    }
}
void ManualControlUpdater::stop()
{
    mStopSignal = true;
    wait();
}


ManualControl::ManualControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManualControl),
    mTaskMode( cpu::CpuMemory::Instance().DB31 ),
    mParams( cpu::CpuMemory::Instance().DB50 ),
    mControlBits( cpu::CpuMemory::Instance().M1 ),
    mErrorBits( cpu::CpuMemory::Instance().I1 )
{
    ui->setupUi(this);

    ui->YB1->setValidator( new QIntValidator( 10, 660, this ) );
    ui->YB2->setValidator( new QIntValidator( 10, 660, this ) );
    ui->YB3->setValidator( new QIntValidator( 10, 315, this ) );
    ui->YB4->setValidator( new QIntValidator( 10, 315, this ) );
    ui->YB5->setValidator( new QIntValidator( 10, 315, this ) );
    ui->YB6->setValidator( new QIntValidator( 10, 315, this ) );
    ui->U_ACDC->setValidator( new QIntValidator( 0, 220, this ) );

    QObject::connect( &Updater, SIGNAL(update()), this, SLOT(onUpdateControls()) );    
}

ManualControl::~ManualControl()
{
    Updater.stop();
    delete ui;
}

void ManualControl::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent( e );
    emit closed();
}
void ManualControl::showEvent( QShowEvent *e )
{
    mTaskMode.N_Operation = 100;
    mTaskMode.Nasos_M2 = false;
    mTaskMode.OP15_25_Continum = false;
    mTaskMode.Q_5_5ma = 80.0;
    mTaskMode.Start_Oper = false;
    mTaskMode.Stop_Oper = false;
    mTaskMode.Write();
    SynkControls();
    Updater.start();
    QWidget::showEvent( e );
}
void ManualControl::hideEvent( QHideEvent *e )
{
    Updater.stop();
    mControlBits.Reset();
    mTaskMode.N_Operation = 0;
    mTaskMode.Nasos_M2 = false;
    mTaskMode.OP15_25_Continum = false;
    mTaskMode.Q_5_5ma = 80.0;
    mTaskMode.Start_Oper = false;
    mTaskMode.Stop_Oper = false;
    mTaskMode.Write();
    QWidget::hideEvent( e );
}

void ManualControl::UpdateButton( QAbstractButton *btn, bool checked )
{
    btn->setChecked( checked );
}
void ManualControl::UpdateMark  ( QLabel *btn, bool draw, QColor color )
{
    QPalette palette = btn->palette();

    if ( draw )
    {
        palette.setColor( btn->backgroundRole(), color );
        btn->setAutoFillBackground( true );
    }
    else
    {
        btn->setAutoFillBackground( false );
    }
    btn->setPalette( palette );
}
void ManualControl::SynkControls()
{
    mControlBits.Read();
    UpdateButton( ui->YA1, mControlBits.YA1 );          //MX40.0 Разгрузка управл.насоса для М1 м3
    UpdateButton( ui->YA2, mControlBits.YA2 );          //MX40.1 Разгрузка управл.насоса для М2 м3
    UpdateButton( ui->YA3, mControlBits.YA3 );          //MX40.2 Разгрузка насоса управления м4
    UpdateButton( ui->YA4, mControlBits.YA4 );          //MX40.3 Включение гидроаккумулятора
    UpdateButton( ui->YA5, mControlBits.YA5 );          //MX40.4 Разгрузка основных насосов м1м2
    UpdateButton( ui->YA6, mControlBits.YA6 );          //MX40.5 Подача расхода к испытательному модулю
    UpdateButton( ui->YA7, mControlBits.YA7 );          //MX40.6 Включ малый расходомер (до 60л/мин)
    UpdateButton( ui->YA8, !mControlBits.YA8 );          //MX40.7 Закрытие канала А
    UpdateButton( ui->YA9, !mControlBits.YA9 );          //MX41.0 Закрытие канала В
    UpdateButton( ui->YA10, !mControlBits.YA10 );         //MX41.1 Закрытие канала Т
    UpdateButton( ui->YA11, mControlBits.YA11 );         //MX41.2 Подача масла к расходомерам из канала Т
    UpdateButton( ui->YA12, mControlBits.YA12 );         //MX41.3 Подача масла к расходомерам из канала У
    UpdateButton( ui->YA13, mControlBits.YA13 );         //MX41.4 Декомпрессия канала Р
    UpdateButton( ui->YA14, mControlBits.YA14 );         //MX41.5 Декомпрессия канала Т
    UpdateButton( ui->YA15, mControlBits.YA15 );         //MX41.6 Декомпрессия канала А
    UpdateButton( ui->YA16, mControlBits.YA16 );         //MX41.7 Декомпрессия канала В
    UpdateButton( ui->YA17, mControlBits.YA17 );         //MX42.0 Декомпрессия канала Х
    UpdateButton( ui->YA18, mControlBits.YA18 );         //MX42.1 Включение г/цилиндра 1 (1-10 л/мин) испытания частоты канала А
    UpdateButton( ui->YA19, mControlBits.YA19 );         //MX42.2 Включение г/цилиндра 1 (1-10 л/мин) испытания частоты канала В
    UpdateButton( ui->YA20, mControlBits.YA20 );         //MX42.3 Включение г/цилиндра 2 (0-1 л/мин) испытания канала А
    UpdateButton( ui->YA21, mControlBits.YA21 );         //MX42.4 Включение г/цилиндра 2 (0-1 л/мин) испытания канала В
    UpdateButton( ui->YA22, mControlBits.YA22 );         //MX42.5 Открытие канала Х
    UpdateButton( ui->YA23, mControlBits.YA23 );         //MX42.6 Включить малый расходомер утечек
    UpdateButton( ui->YA24, mControlBits.YA24 );         //MX42.7 Подвод г/цилиндра
    UpdateButton( ui->YA25, mControlBits.YA25 );         //MX43.0 Отвод г/цилиндра
    UpdateButton( ui->KM1, mControlBits.KM1 );          //MX43.1 Насос М1
    UpdateButton( ui->KM2, mControlBits.KM2 );          //MX43.2 Насос М2
    UpdateButton( ui->KM3, mControlBits.KM3 );          //MX43.3 Насос М3
    UpdateButton( ui->KM4, mControlBits.KM4 );          //MX43.4 Насос М4
    UpdateButton( ui->KM5, mControlBits.KM5 );          //MX43.5 Насос контрура охлаждения М5
    UpdateButton( ui->KM9, mControlBits.KM9 );          //MX43.6 Насос М12
    UpdateButton( ui->KM6, mControlBits.KM6 );          //MX43.7 Вентилятор М6,М7
    UpdateButton( ui->KM7, mControlBits.KM7 );          //MX44.0 Вентилятор М8,М9
    UpdateButton( ui->KM8, mControlBits.KM8 );          //MX44.1 Вентилятор М10,М11
    UpdateButton( ui->KM10, mControlBits.KM10 );         //MX44.2 ТЭНы
}

void ManualControl::onUpdateControls()
{
    UpdateMarks();
    UpdateData();
}

void ManualControl::UpdateMarks()
{
    //краны 0 - красный 1 - зеленый
    UpdateMark( ui->SQ1,true , !mErrorBits.SQ1 ? Qt::red : Qt::green );     //IX0.0    кран насоса М1
    UpdateMark( ui->SQ3,true , !mErrorBits.SQ3 ? Qt::red : Qt::green );     //IX0.2    кран насоса М2
    UpdateMark( ui->SQ5,true , !mErrorBits.SQ5 ? Qt::red : Qt::green );     //IX0.4    кран насоса М3.2
    UpdateMark( ui->SQ7,true , !mErrorBits.SQ7 ? Qt::red : Qt::green );     //IX0.6    кран насоса М3.1
    UpdateMark( ui->SQ9,true , !mErrorBits.SQ9 ? Qt::red : Qt::green );     //IX1.0    кран насоса М4
    UpdateMark( ui->SQ11,true , !mErrorBits.SQ11 ? Qt::red : Qt::green );   //IX1.2    кран насоса М5
    UpdateMark( ui->SQ13,true , !mErrorBits.SQ13 ? Qt::red : Qt::green );   //IX1.4    кран дренажа М1
    UpdateMark( ui->SQ14,true , !mErrorBits.SQ14 ? Qt::red : Qt::green );   //IX1.5    кран дренажа М2
    UpdateMark( ui->SQ15,true , !mErrorBits.SQ15 ? Qt::red : Qt::green );   //IX1.6    кран дренажа М4

    //фильтры 0 - красный 1 - зеленый
    UpdateMark( ui->SP1,true , !mErrorBits.SP1 ? Qt::red : Qt::green );     //IX2.0    фильтр всасывающий М1
    UpdateMark( ui->SP2,true , !mErrorBits.SP2 ? Qt::red : Qt::green );     //IX2.1    фильтр всасывающий М2
    UpdateMark( ui->SP3,true , !mErrorBits.SP3 ? Qt::red : Qt::green );     //IX2.2    фильтр всасывающий М3.2
    UpdateMark( ui->SP4,true , !mErrorBits.SP4 ? Qt::red : Qt::green );     //IX2.3    фильтр всасывающий М3.1
    UpdateMark( ui->SP5,true , !mErrorBits.SP5 ? Qt::red : Qt::green );     //IX2.4    фильтр всасывающий М4
    UpdateMark( ui->SP6,true , !mErrorBits.SP6 ? Qt::red : Qt::green );     //IX2.5    фильтр всасывающий М5
    UpdateMark( ui->SP7,true , !mErrorBits.SP7 ? Qt::red : Qt::green );     //IX2.6    фильтр тонкой очистки М1
    UpdateMark( ui->SP8,true , !mErrorBits.SP8 ? Qt::red : Qt::green );     //IX2.7    фильтр грубой очистки М1
    UpdateMark( ui->SP9,true , !mErrorBits.SP9 ? Qt::red : Qt::green );     //IX3.0    фильтр тонкой очистки М2
    UpdateMark( ui->SP10,true , !mErrorBits.SP10 ? Qt::red : Qt::green );   //IX3.1    фильтр грубой очистки М2
    UpdateMark( ui->SP11,true , !mErrorBits.SP11 ? Qt::red : Qt::green );   //IX3.2    фильтр тонкой очистки М4
    UpdateMark( ui->SP12,true , !mErrorBits.SP12 ? Qt::red : Qt::green );   //IX3.3    фильтр тонкой очистки М3.2
    UpdateMark( ui->SP13,true , !mErrorBits.SP13 ? Qt::red : Qt::green );   //IX3.4    фильтр тонкой очистки М3.1
    UpdateMark( ui->SP14,true , !mErrorBits.SP14 ? Qt::red : Qt::green );   //IX3.5    фильтр контура охлаждения
    UpdateMark( ui->SP15,true , !mErrorBits.SP15 ? Qt::red : Qt::green );   //IX3.6    фильтр тонкой очистки М12

    QColor c;
    if ( mErrorBits.SL1 )
        c = Qt::green;
    else if ( mErrorBits.SL2 )
        c = Qt::yellow;
    else if ( mErrorBits.SL3 )
        c = Qt::red;
    UpdateMark( ui->SL1, mErrorBits.SL1, c );     //IX4.0    уровень масла - норма
    UpdateMark( ui->SL2, mErrorBits.SL2, c );     //IX4.1    уровень масла -  предупреждение
    UpdateMark( ui->SL3, mErrorBits.SL3, c );     //IX4.2    уровень масла - авария

    UpdateMark( ui->SL4, mErrorBits.SL4, Qt::red    );     //IX4.3    уровень масла верхний (поддон)
    UpdateMark( ui->SL5, mErrorBits.SL5, Qt::green  );     //IX4.4    уровень масла нижний (поддон)

    ui->ErrorMark->setVisible( mErrorBits.Error );
    UpdateMark( ui->ErrorMark, mErrorBits.Error, Qt::red    );   //MX44.6 не правильная комбинация вкл клапанов
}

void ManualControl::UpdateValue  ( QLCDNumber *ctrl, float const& value )
{
    ctrl->display( value );
}

void ManualControl::UpdateData()
{

    UpdateValue( ui->BP1, mParams.BP1 );
    UpdateValue( ui->BP2, mParams.BP2 );
    UpdateValue( ui->BP3, mParams.BP3 );
    UpdateValue( ui->BP4, mParams.BP4 );
    UpdateValue( ui->BP5, mParams.BP5 );
    UpdateValue( ui->BP6, mParams.BP6 );
    UpdateValue( ui->BT1, mParams.BT1 );


    float exp_t = mParams.BV2;
    if ( exp_t < 60.0 )
        exp_t = mParams.BV1;
    UpdateValue( ui->BV1BV2, exp_t );

    float lost = mParams.BV4;
    if ( lost < 1.9 )
        lost = mParams.BV3;
    UpdateValue( ui->BV3BV4, lost );

    UpdateValue( ui->U, mParams.V0_300B );
    UpdateValue( ui->I_DC, mParams.A1 );
    UpdateValue( ui->I_AC, mParams.A2 );
    UpdateValue( ui->POS1, mParams.POS_1_REAL );
    UpdateValue( ui->POS2, mParams.POS_2_REAL );
}

void ManualControl::on_KM1_clicked()
{
    mControlBits.SetKM1( ui->KM1->isChecked() );
}
void ManualControl::on_KM2_clicked()
{
    mControlBits.SetKM2( ui->KM2->isChecked() );
}
void ManualControl::on_YA5_clicked()
{
    mControlBits.SetYA5( ui->YA5->isChecked() );
}
void ManualControl::on_KM3_clicked()
{
    mControlBits.SetKM3( ui->KM3->isChecked() );
}
void ManualControl::on_YA1_clicked()
{
    mControlBits.SetYA1( ui->YA1->isChecked() );
}
void ManualControl::on_YA2_clicked()
{
    mControlBits.SetYA2( ui->YA2->isChecked() );
}
void ManualControl::on_KM4_clicked()
{
    mControlBits.SetKM4( ui->KM4->isChecked() );
}
void ManualControl::on_YA3_clicked()
{
    mControlBits.SetYA3( ui->YA3->isChecked() );
}
void ManualControl::on_KM9_clicked()
{
    mControlBits.SetKM9( ui->KM9->isChecked() );
}

void ManualControl::on_YA6_clicked()
{
    mControlBits.SetYA6( ui->YA6->isChecked() );
}
void ManualControl::on_YA13_clicked()
{
    mControlBits.SetYA13( ui->YA13->isChecked() );
}
void ManualControl::on_YA10_clicked()
{
    mControlBits.SetYA10( !ui->YA10->isChecked() );
}
void ManualControl::on_YA14_clicked()
{
    mControlBits.SetYA14( ui->YA14->isChecked() );
}
void ManualControl::on_YA8_clicked()
{
    mControlBits.SetYA8( !ui->YA8->isChecked() );
}
void ManualControl::on_YA15_clicked()
{
    mControlBits.SetYA15( ui->YA15->isChecked() );
}
void ManualControl::on_YA9_clicked()
{
    mControlBits.SetYA9( !ui->YA9->isChecked() );
}
void ManualControl::on_YA16_clicked()
{
    mControlBits.SetYA16( ui->YA16->isChecked() );
}
void ManualControl::on_YA22_clicked()
{
    mControlBits.SetYA22( ui->YA22->isChecked() );
}
void ManualControl::on_YA17_clicked()
{
    mControlBits.SetYA17( ui->YA17->isChecked() );
}

void ManualControl::on_KM6_clicked()
{
    mControlBits.SetKM6( ui->KM6->isChecked() );
}
void ManualControl::on_KM7_clicked()
{
    mControlBits.SetKM7( ui->KM7->isChecked() );
}
void ManualControl::on_KM8_clicked()
{
    mControlBits.SetKM8( ui->KM8->isChecked() );
}
void ManualControl::on_KM10_clicked()
{
    mControlBits.SetKM10( ui->KM10->isChecked() );
}
void ManualControl::on_KM5_clicked()
{
    mControlBits.SetKM5( ui->KM5->isChecked() );
}

void ManualControl::on_YA12_clicked()
{
    mControlBits.SetYA12( ui->YA12->isChecked() );
}
void ManualControl::on_YA23_clicked()
{
    mControlBits.SetYA23( ui->YA23->isChecked() );
}
void ManualControl::on_YA11_clicked()
{
    mControlBits.SetYA11( ui->YA11->isChecked() );
}
void ManualControl::on_YA7_clicked()
{
    mControlBits.SetYA7( ui->YA7->isChecked() );
}


void ManualControl::on_YA18_clicked()
{
    mControlBits.SetYA18( ui->YA18->isChecked() );
}
void ManualControl::on_YA19_clicked()
{
    mControlBits.SetYA19( ui->YA19->isChecked() );
}
void ManualControl::on_YA20_clicked()
{
    mControlBits.SetYA20( ui->YA20->isChecked() );
}
void ManualControl::on_YA21_clicked()
{
    mControlBits.SetYA21( ui->YA21->isChecked() );
}
void ManualControl::on_YA24_clicked()
{
    mControlBits.SetYA24( ui->YA24->isChecked() );
}
void ManualControl::on_YA25_clicked()
{
    mControlBits.SetYA25( ui->YA25->isChecked() );
}

void ManualControl::on_YA4_clicked()
{
    mControlBits.SetYA4( ui->YA4->isChecked() );
}

void ManualControl::on_Accept_clicked()
{
    bool s = false;

    mParams.YB1 = ui->YB1->text().toDouble( &s );
    mParams.YB2 = ui->YB2->text().toDouble( &s );
    mParams.YB3 = ui->YB3->text().toDouble( &s );
    mParams.YB4 = ui->YB4->text().toDouble( &s );
    mParams.YB5 = ui->YB5->text().toDouble( &s );
    mParams.YB6 = ui->YB6->text().toDouble( &s );

    double voltage = ui->U_ACDC->text().toDouble( &s );
    if ( ui->rb_AC->isChecked() )
    {
        mParams.Z_U_Post = 0.0;
        mParams.Z_U_Per = voltage;
    }
    else
    {
        mParams.Z_U_Per = 0.0;
        mParams.Z_U_Post = voltage;
    }

    mParams.WriteTask();
}
