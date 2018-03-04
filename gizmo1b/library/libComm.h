#ifndef _LIB_COMM_H_
#define _LIB_COMM_H_

class LibComm {
public:
    LibComm();
    virtual ~LibComm();
private:
    static bool s_isInitialized;
};

#endif /* _LIB_COMM_H_ */
