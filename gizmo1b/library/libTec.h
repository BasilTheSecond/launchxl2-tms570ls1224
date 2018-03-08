#ifndef _LIB_TEC_H_
#define _LIB_TEC_H_

#include <vector>
#include <utility>

class LibTec
{
public:
    typedef std::pair<int, float> TimeCurrent;
    LibTec();
    virtual ~LibTec();
    void enable(bool en);
    float getISense(); // Calculated Current Based on ADC voltage
    float getVSense(); // Calculated Voltage Based on ADC voltage
    void setCurrent(float value); // Calculated Closed Loop Current Output
    // Linear Interpolation over time between set points,
    // Repeat output LOOPCOUNT times, Time (us)
    void setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_TEC_H_ */
