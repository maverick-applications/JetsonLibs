#include "24a25uid.h"


MCP_24A25UID::MCP_24A25UID(int address){
    kI2CBus = 1;
    kI2CAddress = address;
    error = 0;
};

MCP_24A25UID::~MCP_24A25UID(){
    i2c_close();
};

bool MCP_24A25UID::i2c_open(){
    char fileNameBuffer[32];
    sprintf(fileNameBuffer, "/dev/i2c-%d", kI2CBus);
    kI2CFileDescriptor = open(fileNameBuffer, O_RDWR);

    if(kI2CFileDescriptor < 0){
        error = errno;
        return false;
    }

    if(ioctl(kI2CFileDescriptor, I2C_SLAVE, kI2CAddress) < 0){
        error = errno;
        return false;
    }
    return true;
};

void MCP_24A25UID::i2c_close(){
    if (kI2CFileDescriptor > 0){
        close(kI2CFileDescriptor);
        // WARNING: This is not quite right, need to check for error first
        kI2CFileDescriptor = -1;
    }
};

int MCP_24A25UID::getError(){
    return error
}

int MCP_24A25UID::setAddress(uint8_t address){
    return i2c_smbus_write_byte(kI2CFileDescriptor, writeValue);
}

int MCP_24A25UID::byteWrite(uint8_t address, uint8_t writeValue){
    uint8_t buffer[2] = {address, writeValue};
    return write(kI2CFileDescriptor, buffer, 2);
};

int MCP_24A25UID::pageWrite(uint8_t start_address, uint8_t* data, uint8_t length){
    // This should track page location and prevent rollover
    // Should rollover data be written to the next page?
    // Max Address write address 0x79
    // Page size 16 bytes
    // Verify data to write fits in memory
    if((start_address + length) > 0x80){
        return -1;
    }
    uint8_t buffer[16];

}

bool MCP_24A25UID::ackPoll(){
    write(kI2CFileDescriptor, (uint8_t*)null, 0);
}



void MCP_24A25UID::begin(){};
void MCP_24A25UID::end(){};
