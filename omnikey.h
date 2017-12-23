#pragma once

#include <QObject>
#include <QKeyEvent>
#include <vector>
#include <thread>
#include <memory>

class Omnikey :public QObject
{
   Q_OBJECT

public:
   ~Omnikey();
   static Omnikey& Instance();

signals:

   void card_in( QString id );
   void card_out();

private:
   Omnikey();
   Omnikey( Omnikey const& ) = delete;
   void operator = ( Omnikey const& ) = delete;

   bool eventFilter(QObject *watched, QEvent *event);

   typedef std::pair<QKeyEvent, QObject*> EventData;
   typedef std::vector< EventData > KeyEvents;
   void ParseData( KeyEvents const& in, KeyEvents& out );
   bool ParseText( QString const& text );
   void SendEvents(KeyEvents &in );

   bool IsStartKey( QKeyEvent const& e );
   bool IsEndKey( QKeyEvent const& e );
   QString ToString( QKeyEvent const& e );

   void ScanThread();

   void OnCardIn( QString const& id );
   void OnCardOut();


   KeyEvents mEvents;
   bool mCollect = false;
   QString mLastData;
   bool mIgnore = false;

   std::unique_ptr<std::thread> mScanThread;
   bool mRun = true;
   bool mCardIn = false;
};
