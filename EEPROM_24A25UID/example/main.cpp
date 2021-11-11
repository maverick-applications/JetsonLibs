#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>

#include "24a25uid.h"


int main(){
    MCP_24A25UID* eeprom = new MCP_24A25UID;
    int err = eeprom->i2c_open();
    if(err <= 0){
        printf("Error: %d", eeprom->error);
    } else {
        printf("EEPROM Device Address: 0x%02X\n", eeprom->kI2CAddress);
        printf("Device Bus: 0x%02X\n", eeprom->kI2CBus);
        eeprom->begin();
    }
}
