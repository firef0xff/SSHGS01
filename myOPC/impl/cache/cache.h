#pragma once
#include <map>


namespace memory
{

class CacheBase
{
public:
    CacheBase()
    {}
    virtual ~CacheBase()
    {}
    virtual void Reset() = 0;
};

template < typename KeyType, typename ValueType >
class Cache : public CacheBase
{
public:
    Cache()
    {
    }
    ~Cache()
    {
    }
    void Reset()
    {
        data.clear();
    }
    void Write(const KeyType &addr, const ValueType &data)
    {
        this->data[addr] = data;
    }

    const ValueType& Read(const KeyType &addr)
    {
        return this->data[addr];
    }
protected:
    typedef std::pair<const KeyType, ValueType> DataItem;
    typedef std::map<const KeyType, ValueType> Data;

    Data data;
};

}
