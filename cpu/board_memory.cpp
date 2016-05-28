#include "board_memory.h"

namespace cpu
{

namespace data
{
BoardMem::BoardMem()
{}
void BoardMem::Start()
{
    bool inited = false;
    if (!mBoard)
        mBoard.reset( new CustomBoard() );
    mBoard->SetRUN_STOP(0);
    mBoard->SetA21(0);
    if ( IsShim )
    {
        mBoard->SetMIN_CUR( mMinAmperage );//А03 - Минимальный ток. 0, 5000
        mBoard->SetMAX_CUR( mMaxAmperage );//А02 - Максимальный ток. 0, 5000
        mBoard->SetV_AMP( mVSigAmpl ); //А04 - Амплитуда вибрационного сигнала мА 0, 200
        mBoard->SetV_FREQ( mVSigFreq ); //А05 - Частота вибрационного сигнала, Гц 1, 200

        mBoard->SetPOLARITY_A( 1 );//А06 - Полярность сигнала А -1 1
        mBoard->SetPOLARITY_B( 1 );//А07 - Полярность сигнала B

        if ( mMinAmperage == mBoard->GetMIN_CUR() &&
             mMaxAmperage == mBoard->GetMAX_CUR() &&
             mVSigAmpl == mBoard->GetV_AMP() &&
             mVSigFreq == mBoard->GetV_FREQ() &&
             1 == mBoard->GetPOLARITY_A() &&
             1 == mBoard->GetPOLARITY_B() )
        {
            int sig = mOutputChannels == 1 ? 1 : mOutputChannels == 2 ? 3 : 0;
            mBoard->SetRUN_STOP( sig );//А01 - 0 выкл. Для включения что задавать сюда? 1 2 или 3
            inited = sig == mBoard->GetRUN_STOP();
        }
    }
    else
    {
        mBoard->SetMaxCur2( mMaxAmperage );// А22 - Минимальный ток.
        mBoard->SetMinCur2( mMinAmperage );// А23 - Максимальный ток.
        mBoard->SetA24( mVSigAmpl );// A24 - Амплитуда вибрационного сигнала мА, Гц 0, 20    params
        mBoard->SetA25( mVSigFreq );// A25 - Частота вибрационного сигнала 1, 200            params
        mBoard->SetA26( 1 );// A26 - Полярность сигнала 0 1 2                        params

        if ( mMaxAmperage == mBoard->GetMaxCur2() &&
             mMinAmperage == mBoard->GetMinCur2() &&
             mVSigAmpl == mBoard->GetA24() &&
             mVSigFreq == mBoard->GetA25() &&
             1 == mBoard->GetA26())
        {
            mBoard->SetA21( 1 );// А21 - 1 вкл, 0 выкл
            inited = 1 == mBoard->GetA21();
        }
    }
    if ( !inited )
        throw ::control_board::COMError( "Ошибка запуска платы управления" );
}
void BoardMem::Stop()
{
    if (!mBoard)
        return;
    mBoard->SetRUN_STOP( 0 );
    mBoard->SetA21( 0 );

    if ( 0 != mBoard->GetRUN_STOP() ||
         0 != mBoard->GetA21())
        throw ::control_board::COMError( "Ошибка останова платы управления" );
}

}
}
