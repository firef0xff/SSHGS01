#pragma once

namespace cpu
{
namespace data
{

class In
{
public:    
    virtual ~In(){}
    virtual void Read() = 0;
};

class Out
{
public:   
    virtual ~Out(){}
    virtual void Write() = 0;
};

class InOut : public In, Out
{
public:
    virtual ~InOut(){}
};

}//namespace data
}//namespace cpu
