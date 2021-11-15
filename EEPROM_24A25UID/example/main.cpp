#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <thread>

#include "24a25uid.h"

void printBuffer(uint8_t* data, uint8_t length);

int main(){
    int buffer_size = 66;
    MCP_24A25UID* eeprom = new MCP_24A25UID(0x50);
    int err = eeprom->i2c_open();
    if(err <= 0){
        printf("Error: %d", eeprom->error);
    } else {
        printf("EEPROM Device Address: 0x%02X\n", eeprom->kI2CAddress);
        printf("Device Bus: 0x%02X\n", eeprom->kI2CBus);
        eeprom->begin();
    }
    printf("%02X\n", eeprom->setAddress(0xFA));
    uint8_t eeprom_uid[6];
    eeprom->i2cread(eeprom_uid, 6);
    printBuffer(eeprom_uid, 6);

    uint8_t buffer[buffer_size] = {0};
    printBuffer(buffer, buffer_size);
    eeprom->readFromAddress(0, buffer, buffer_size);
    printBuffer(buffer, buffer_size);

    // // Test single byte write
    // std::fill(buffer, buffer+8, 0);
    // eeprom->byteWrite(0, 0xCC);
    // while (eeprom->ackPoll() != 1){
    //     //std::this_thread::sleep_for(std::chrono::milliseconds(2));
    //     continue;
    // }
    // uint8_t read_buffer[16] = {0};
    // eeprom->readFromAddress(0, read_buffer, 1);
    // printBuffer(read_buffer, 1);

    // Test Page Write
    //std::fill(buffer, buffer+24, 0);
    for(int i = 0; i < buffer_size; ++i){
        buffer[i] = (uint8_t)i;
    }
    eeprom->pageWrite(1, buffer, buffer_size);
    eeprom->readFromAddress(0, buffer, buffer_size);
    printBuffer(buffer, buffer_size);

}

void printBuffer(uint8_t* data, uint8_t length){
    for (int i = 0; i < length; ++i){
        printf("%02X ", data[i]);
    }
    printf("\n");
}