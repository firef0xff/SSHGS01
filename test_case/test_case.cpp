#include "test_case.h"
#include "test.h"

namespace test
{
TestCase::TestCase(QString const& name, const QString &descr):
    mName( name ), mDescr( descr )
{}

TestCase::~TestCase()
{
    foreach (Test* t, mData)
    {
        t->Free();
    }
}

TestCase::DataType const& TestCase::Tests() const
{
    return mData;
}

QString const& TestCase::Name() const
{
    return mName;
}

QString const& TestCase::Descr() const
{
    return mDescr;
}

void TestCase::Subscribe( Test* t )
{
    mData.insert( t->Number(), t );
}
void TestCase::Unsubscribe( Test* t )
{
    auto it = mData.find( t->Number() );
    if ( it != mData.end() )
        mData.erase( it );
}


}//namespace test
