#include "omnikey.h"
#include <QEvent>
#include <QKeyEvent>
#include <memory>
#include <mutex>

#ifdef WIN32
#include <windows.h>
#include <Winscard.h>
#endif


namespace
{
QString TAG_CARD_IN = "1";
QString TAG_CARD_OUT = "2";

static std::unique_ptr<Omnikey> INST;
static std::mutex MUTEX;
}

Omnikey& Omnikey::Instance()
{
   if ( !INST )
   {
      std::unique_lock< std::mutex > lock(MUTEX);
      if ( !INST )
      {
         INST.reset( new Omnikey );
      }
   }
   return *INST;
}

Omnikey::Omnikey()
{
   mRun = true;
   mScanThread.reset( new std::thread( [this]()
   {
      ScanThread();
   } ) );
}

Omnikey::~Omnikey()
{
   mRun = false;
   if ( mScanThread && mScanThread->joinable() )
      mScanThread->join();
   mScanThread.reset();
}

void Omnikey::ScanThread()
{
#ifdef WIN32

   for(; mRun; )
   {
      std::this_thread::sleep_for(std::chrono::seconds(1));

      DWORD dwReaders;
      LPWSTR szReaders = NULL;
      SCARDCONTEXT hContext;
      std::vector<wchar_t const*> cards;

      LONG status = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hContext);
      if( status != SCARD_S_SUCCESS )
         continue;

      dwReaders = SCARD_AUTOALLOCATE;
      if( SCardListReaders(hContext, NULL, (LPWSTR)&szReaders, &dwReaders) == SCARD_S_SUCCESS )
      {
          LPWSTR reader = szReaders;
          while ( reader != NULL && *reader != '\0')
          {
              //std::wcout << L"Reader name: '" << reader << L"'" << std::endl;
              cards.push_back( reader );
              reader += wcslen(reader)+1;
          }

          std::vector<SCARD_READERSTATEW> lpState( cards.size() );
          for( size_t n = 0; n < cards.size(); ++n )
          {
              memset( &lpState[n], 0, sizeof(SCARD_READERSTATEW) );
              lpState[n].szReader = cards[n];
          }

          do
          {
              status = SCardGetStatusChange( hContext, 500, lpState.data(), cards.size() );
              switch( status )
              {
                 case SCARD_S_SUCCESS:
                 case SCARD_E_TIMEOUT:
                     for( size_t n = 0; n < cards.size(); ++n )
                     {
                         if( !( lpState[n].dwEventState & SCARD_STATE_PRESENT ))
                            OnCardOut();//карты нет
                         else{}
                            //карта вставлена
                     }
                     break;
                 default:
                     break;
              }

              std::this_thread::sleep_for(std::chrono::seconds(1));
          }
          while( mRun );
          // only do this after being done with the strings, or handle the names another way!
          SCardFreeMemory( hContext, szReaders );
      }
      SCardReleaseContext( hContext );
   }
#endif
   mRun = false;
}

bool Omnikey::IsStartKey( QKeyEvent const& e )
{
   return e.text() == "#" || e.text() == "№" ;
}
bool Omnikey::IsEndKey( QKeyEvent const& e )
{
   return e.text() == "@" || e.text() == "\"";
}
QString Omnikey::ToString( QKeyEvent const& e )
{
   return e.text();
}

bool Omnikey::eventFilter(QObject *watched, QEvent *event)
{
   if ( !mIgnore && event->type() == QEvent::KeyPress )
   {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      if (!mCollect && IsStartKey( *keyEvent ) )
      {
         mCollect = true;
         mLastData.clear();
      }

      if ( !mCollect )
         return QObject::eventFilter( watched, event );
      mEvents.push_back( std::make_pair( *keyEvent, watched ) );
      event->accept();

      if ( mCollect && IsEndKey( *keyEvent ) )
      {
         mCollect = false;
         KeyEvents resend;
         ParseData( mEvents, resend );
         mEvents.clear();
         SendEvents( resend );
      }
      return true;
   }
   return QObject::eventFilter( watched, event );
}

void Omnikey::ParseData( KeyEvents const& in, KeyEvents& out )
{
for ( auto it = in.begin(), end = in.end(); it !=end; ++it )
   {
      EventData const& event = *it;
      if ( it == in.begin() )
      {
         if ( !IsStartKey( event.first ) )
         {
            out = in;
            return;
         }
         else
         {
            continue;
         }
      }

      if ( !IsEndKey( event.first ) )
         mLastData += ToString( event.first );

      if ( IsEndKey( event.first )  )
      {
         auto start = in.begin();
         if ( ParseText( mLastData ) )
            start = it + 1;
         if ( start != end )
            out.insert( out.end(), start, end );
         return;
      }
   }
   out = in;
}

bool Omnikey::ParseText( QString const& text )
{
   if ( text == TAG_CARD_OUT )
   {
      emit OnCardOut();
      return true;
   }
   else if ( text.left( TAG_CARD_IN.size() ) == TAG_CARD_IN )
   {
      auto offset = TAG_CARD_IN.size();
      QString msg = text.mid( offset, text.size() - offset );
      OnCardIn(msg);
      return true;
   }
   else
      return false;
}

void Omnikey::SendEvents( KeyEvents& in )
{
   mIgnore = true;
   for ( EventData& event: in )
   {
      event.second->event( &event.first );
   }
   mIgnore = false;
}

void Omnikey::OnCardIn( QString const& id )
{
   mCardIn = true;
   emit card_in( id );
}
void Omnikey::OnCardOut()
{
   if ( !mCardIn )
      return;
   mCardIn = false;
   emit card_out();
}
