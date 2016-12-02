#include "spark_wiring_i2c.h"
#include "spark_wiring_constants.h"
#include "spark_wiring.h"
#include "spark_wiring_usbserial.h"

class IO24{
public: 
    void setAddress(int A01, int A11, int A21, int A02, int A12, int A22);
    bool readAllInputs(int* buffer);
    int readInput(int channel);
    
    bool initialized = true;
    
private:
    int address1 = 0x20;
    int address2 = 0x20;
    unsigned long timeout = 100;
    bool inFailureMode = false;
    bool wireWrite(int addr, int reg, int* buff, int len);
    bool wireRead(int addr, int reg, int* readBuff, int returnLen);
};