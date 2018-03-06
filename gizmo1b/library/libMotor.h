#ifndef _LIB_MOTOR_H_
#define _LIB_MOTOR_H_

class LibMotor
{
public:
    enum LibMotorDirection {
        LIB_MOTOR_DIRECTION_POSITIVE,
        LIB_MOTOR_DIRECTION_NEGATIVE,
    };
    enum LibMotorMicrosteps {
        LIB_MOTOR_MICROSTEPS_0,
        LIB_MOTOR_MICROSTEPS_1,
        LIB_MOTOR_MICROSTEPS_2,
        LIB_MOTOR_MICROSTEPS_3,
        LIB_MOTOR_MICROSTEPS_4,
        LIB_MOTOR_MICROSTEPS_5,
        LIB_MOTOR_MICROSTEPS_6,
        LIB_MOTOR_MICROSTEPS_7,
    };
    enum LibMotorError {
        LIB_MOTOR_OKAY,
        LIB_MOTOR_ERROR_INVALID_DIRECTION,
        LIB_MOTOR_ERROR_STEPS_COUNT,
        LIB_MOTOR_ERROR_INVALID_MICROSTEPS,
    };
    LibMotor();
    virtual ~LibMotor();
    void initialize(); // Setup motor to basic operation (Hold Current, Etc)
    int moveRelative(int direction, int stepsCount); // DIR[0,1], STEPS[1-1,000,000]
    int moveAbs(int position);
    int setMicroSteps(int value); // Set Microsteps 1/2^N for N = [0-7]
    int setSpeed(double speed);
    int setAcceleration(double acceleration);
    int setDeceleration(double deceleration);
    int setHoldCurrent(double holdCurrent);
private:
    static bool s_isInitialized;
};

#endif /* _LIB_MOTOR_H_ */
