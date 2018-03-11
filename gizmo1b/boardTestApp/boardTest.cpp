#include "boardTest.h"

BoardTest::BoardTest()
{
}

BoardTest::~BoardTest()
{
}

int BoardTest::get(uint32 address, uint32& value)
{
    return ERROR_WO;
}

int BoardTest::set(uint32 address, uint32 value)
{
    return ERROR_RO;
}
