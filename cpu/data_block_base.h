#pragma once

namespace cpu
{

class InputBlock
{
public:
    InputBlock();
    virtual ~InputBlock();
    virtual void Read() = 0;
};

class OutputBlock
{
public:
    OutputBlock();
    virtual ~OutputBlock();
    virtual void Write() = 0;
};

class InOutBlock : public InputBlock, OutputBlock
{};

}//namespace cpu
