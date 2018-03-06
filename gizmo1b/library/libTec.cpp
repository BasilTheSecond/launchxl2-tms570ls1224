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

void enable(bool en)
{
}

double getISense()
{
    return 0.0;
}

double getVSense()
{
    return 0.0;
}

void setCurrent(double value)
{
}

void LibTec::setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints)
{
}

