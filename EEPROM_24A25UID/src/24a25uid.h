#ifndef _24A25UID_H
#define _24A25UID_H

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


class MCP_24A25UID {
  public:
    unsigned char kI2CBus;
    int kI2CFileDescriptor;
    int kI2CAddress;
    int error;
    MCP_24A25UID(int address=0x50);
    ~MCP_24A25UID();

    bool i2c_open();
    void i2c_close();
    int getError();

    int setAddress(uint8_t address);
    int byteWrite(uint8_t address, uint8_t writeValue);
    int pageWrite(uint8_t start_address, uint8_t* data, int length);
    bool ackPoll();

    int i2cread(uint8_t* data, int length);
    uint8_t getCurrentReadAddress();

    void begin();
    void end();
};



#endif // _24A25UID_H
