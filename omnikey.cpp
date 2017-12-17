#include "omnikey.h"
#include <QEvent>
#include <QKeyEvent>
#include <memory>
#include <mutex>

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
}

Omnikey::~Omnikey()
{
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
      int key = keyEvent->nativeVirtualKey();
      QString t = keyEvent->text();
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
      emit card_out();
      return true;
   }
   else if ( text.left( TAG_CARD_IN.size() ) == TAG_CARD_IN )
   {
      auto offset = TAG_CARD_IN.size();
      QString msg = text.mid( offset, text.size() - offset );
      emit card_in( msg );
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
