#ifndef _PN532_H
#define _PN532_H

#include <cstddef>
#include <linux/i2c-dev.h>
extern "C"{
  #include <i2c/smbus.h>
}
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>


class NFC_PN532 {
  public:
    unsigned char kI2CBus;
    int kI2CFileDescriptor;
    int kI2CAddress;
    int error;
    NFC_PN532(int address=0x24);
    ~NFC_PN532();

    bool i2c_open();
    void i2c_close();
    int getError();

    int i2cwrite(int writeValue);
    int i2cwrite(uint8_t* data, int length);
    int i2cread(uint8_t* data, int length);

    void begin();
    void end();
};



#endif // _PN532_H
