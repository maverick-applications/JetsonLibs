#include "PN532.h"


NFC_PN532::NFC_PN532(int address){
    kI2CBus = 1;
    kI2CAddress = address;
    error = 0;
};

NFC_PN532::~NFC_PN532(){
    i2c_close();
};

bool NFC_PN532::i2c_open(){
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

void NFC_PN532::i2c_close(){
    if (kI2CFileDescriptor > 0){
        close(kI2CFileDescriptor);
        // WARNING: This is not quite right, need to check for error first
        kI2CFileDescriptor = -1;
    }
};

int NFC_PN532::i2cwrite(int writeValue){
    int toReturn = i2c_smbus_write_byte(kI2CFileDescriptor, writeValue);
    printf("%X\n", toReturn);
    return toReturn;
};

int NFC_PN532::i2cwrite(uint8_t* data, int length){
    return write(kI2CFileDescriptor, data, length);

}

int NFC_PN532::i2cread(uint8_t* data, int length){
    uint8_t buffer[6];
    read(kI2CFileDescriptor, buffer, 6);
    for(int i = 0; i < 6; ++i){
        printf("0x%X\n", buffer[i]);
    }
    return 1;
    // return i2c_smbus_read_byte(kI2CFileDescriptor);
}

void NFC_PN532::begin(){
    uint8_t cmdBuffer[9] = {0x00, 0x00, 0xFF, 0x02, 0xFE, 0xD4, 0x02, 0x2A, 0x00};
    i2cwrite(cmdBuffer, 9);
    uint8_t buffer[6] = {0,0,0,0,0,0};
    i2cread(buffer, 6);
    // for (int i = 0; i < 6; ++i){
    //     buffer[i] = (uint8_t)i2cread();
    //     printf("0x%X\n", buffer[i]);
    // }
};
void NFC_PN532::end(){};
