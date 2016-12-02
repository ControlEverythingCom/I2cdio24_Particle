// This #include statement was automatically added by the Particle IDE.
#include "MCP23017-MCP23008.h"

IO24 inputBoard;

void setup() {
    inputBoard.setAddress(0,0,0,1,0,0);
}

void loop() {
    //Read each input 1 at a time.
    for(int i = 1; i < 25; i++){
        int status = inputBoard.readInput(i);
        if(status == 1){
            Serial.printf("Input %i tripped \n", i);
        }
        if(status == 256){
            Serial.printf("Read Input %i failed \n", i);
        }
        delay(10);
    }
    if(inputBoard.initialized == false){
        Serial.println("Something wrong with board");
    }
    delay(1000);
    
    //read all inputs
    int buffer[3];
    
    inputBoard.readAllInputs(buffer);
    Serial.printf("inputs 1-8: %i \n", buffer[0]);
    Serial.printf("inputs 9-16: %i \n", buffer[1]);
    Serial.printf("inputs 17-24: %i \n", buffer[2]);
    
}