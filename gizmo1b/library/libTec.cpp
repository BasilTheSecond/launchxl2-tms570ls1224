#include "libTec.h"

bool LibTec::s_isInitialized = false;

LibTec::LibTec()
{
    if (!s_isInitialized) {
        s_isInitialized = true;
    }
}

LibTec::~LibTec()
{
}

void LibTec::enable(bool en)
{
}

float LibTec::getISense()
{
    return 0.0;
}

float LibTec::getVSense()
{
    return 0.0;
}

void LibTec::setCurrent(float value)
{
}

void LibTec::setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints)
{
}

