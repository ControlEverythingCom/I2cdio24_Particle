#include "MCP23017-MCP23008.h"

void IO24::setAddress(int A01, int A11, int A21, int A02, int A12, int A22){
    //Set MCP23017 and MCP23008 addresses based on user's jumper selection.
    if(A01 == 1){
        address1 = address1 | 1;
    }
    if(A11 == 1){
        address1 = address1 | 2;
    }
    if(A21 == 1){
        address1 = address1 | 4;
    }
    if(A02 == 1){
        address2 = address2 | 1;
    }
    if(A12 == 1){
        address2 = address2 | 2;
    }
    if(A22 == 1){
        address2 = address2 | 4;
    }
    
    //Set all channels on MCP23017 to inputs
    int buff[2] = {255, 255}; 
    if(!wireWrite(address1, 0, buff, 2)){
        initialized = false;
    }
    
    //Pull all inputs up on MCP23017 for reading dry contact closures to ground.
    if(!wireWrite(address1, 0x0C, buff, 2)){
        initialized = false;
    }
    
    //Set all channels on MCP23008 to inputs
    int buff2[1] = {255};
    if(!wireWrite(address2, 0, buff, 1)){
        initialized = false;
    }
    
    //Pull all inputs up on MCP23008 for reading dry contact closures to ground.
    if(!wireWrite(address2, 0x06, buff, 1)){
        initialized = false;
    }
}

bool IO24::readAllInputs(int* buffer){
    int mcp23017[2];
    int mcp23008[1];
    if(!wireRead(address1, 18, mcp23017, 2)){
        return false;
    }
    if(!wireRead(address2, 9, mcp23008, 1)){
        return false;
    }
    byte b1 = mcp23017[0];
    byte b2 = mcp23017[1];
    byte b3 = mcp23008[0];
    b1 = ~b1;
    b2 = ~b2;
    b3 = ~b3;
    buffer[0] = b1;
    buffer[1] = b2;
    buffer[2] = b3;
    return true;
}

int IO24::readInput(int channel){
    if(channel > 24){
        return 256;
    }
    
    if(channel <= 16){
        //read input on MCP23017
        int mcp23017[2];
        if(!wireRead(address1, 18, mcp23017, 2)){
            return 256;
        }
        channel = channel - 1;
        if(channel > 7){
            channel = channel - 8;
            int value = 1; 
            for(int i = 0; i < 8; i++){
                if(i == channel){
                    break;
                }
                value *=2;
            }
            // Serial.printf("Checking against bit: %i \n", value);
            if((value & ~mcp23017[1]) == value){
                return 1;
            }else{
                return 0;
            }
        }
        int value = 1; 
        for(int i = 0; i < 8; i++){
            if(i == channel){
                break;
            }
            value *=2;
        }
        // Serial.printf("Checking against bit: %i \n", value);
        if((value & ~mcp23017[0]) == value){
            return 1;
        }else{
            return 0;
        }
    }else{
        //read input on MCP23008
        int mcp23008[1];
        if(!wireRead(address2, 9, mcp23008, 1)){
            return 256;
        }
        channel = channel - 17;
        int value = 1; 
        for(int i = 0; i < 8; i++){
            if(i == channel){
                break;
            }
            value *=2;
        }
        // Serial.printf("Checking against bit: %i \n", value);
        if((value & ~mcp23008[0]) == value){
            return 1;
        }else{
            return 0;
        }
    }
}

bool IO24::wireWrite(int addr, int reg, int* buff, int len){
    if (!Wire.isEnabled()) {
        Wire.begin();
    }
    Wire.beginTransmission(addr);
    Wire.write(reg);
    for(int i = 0; i < len; i++){
        Wire.write(buff[i]);
    }
    byte status = Wire.endTransmission();
    if(status == 0){
        return true;
    }else{
        return false;
    }
}

bool IO24::wireRead(int addr, int reg, int* readBuff, int returnLen){
    int buf[returnLen];
    if (!Wire.isEnabled()) {
        Wire.begin();
    }
    Wire.beginTransmission(addr);
    Wire.write(reg);
    byte status = Wire.endTransmission();
    if(status != 0){
        return false;
    }
    Wire.requestFrom(addr, returnLen);
    unsigned long startTime = millis();
    while(Wire.available() != returnLen && millis()<startTime+timeout);
    if(Wire.available() != returnLen){
        return false;
    }
    for(int i = 0; i < returnLen; i++){
        readBuff[i] = Wire.read();
        // Serial.printf("read: %i \n", readBuff[i]);
    }
    return true;
}
