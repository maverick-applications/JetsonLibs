#include "24a25uid.h"
#include <cstring>


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
    return error;
}

int MCP_24A25UID::setAddress(uint8_t address){
    return i2c_smbus_write_byte(kI2CFileDescriptor, address);
}

int MCP_24A25UID::byteWrite(uint8_t address, uint8_t writeValue){
    uint8_t buffer[2] = {address, writeValue};
    return write(kI2CFileDescriptor, buffer, 2);
};

int MCP_24A25UID::pageWrite(uint8_t start_address, uint8_t* data, int length){
    // This should track page location and prevent rollover
    // Should rollover data be written to the next page?
    // Max Address write address 0x79
    // Page size 16 bytes
    // Verify data to write fits in memory
    if((start_address + length) > 0x80){
        return -1;
    }
    // printf("Start Address: %02X\n", start_address);
    // printf("Length: %d\n", length);
    uint8_t buffer[17] = {start_address};
    uint8_t write_len = (start_address%16)+length>16?16 - (start_address%16):length;
    std::memcpy(&buffer[1], data, write_len);
    //int return_val = write_len;
    // printf("Write Length: %d\n", write_len);
    int return_val = write(kI2CFileDescriptor, buffer, write_len+1);
    // TODO: Make this non-blocking
    while(ackPoll() != 1){
       continue;
    }
    //for(int i = 0; i < write_len+1; ++i){
    //    printf("%02X ", buffer[i]);
    //}
    //printf("\n");

    int remaining_bytes = length - write_len;
    // printf("Remaining bytes: %d\n", remaining_bytes);
    if (remaining_bytes > 0){
        return return_val + pageWrite(start_address + write_len, data + write_len, remaining_bytes);
    } else {
         return return_val;
    }
}

bool MCP_24A25UID::ackPoll(){
    uint8_t buffer = 0;
    return write(kI2CFileDescriptor, &buffer, 1);
}

int MCP_24A25UID::i2cread(uint8_t* data, int length){
    return read(kI2CFileDescriptor, data, length);
}

int MCP_24A25UID::readFromAddress(uint8_t address, uint8_t* data, int length){
    setAddress(address);
    return i2cread(data, length);
}



void MCP_24A25UID::begin(){};
void MCP_24A25UID::end(){};
