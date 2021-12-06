/* 
 * File:  Controller.h 
 * Author: Matthew Cranford
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include <xc.h> 

#define LX_MIN_EEPROM 0x60
#define LX_MAX_EEPROM 0x61
#define LY_MIN_EEPROM 0x62
#define LY_MAX_EEPROM 0x63

#define RX_MIN_EEPROM 0x64
#define RX_MAX_EEPROM 0x65
#define RY_MIN_EEPROM 0x66
#define RY_MAX_EEPROM 0x67

#define RESET_EEPROM 0x68

typedef enum {
    //values are in order for data_byte1 LSB first
    DLeft,
    DDown,
    DRight,
    DUp,
    Start,
    R3,
    L3,
    Select
} digitalByteFirst;

typedef enum {
    //values are in order for data_byte2 LSB first    
    Square,
    X,
    Circle,
    Triangle,
    R1,
    L1,
    R2,
    L2
} digitalByteSecond;

extern char AN_btn = 1;

extern char digitalStateFirst = 0xFF;
extern char digitalStateSecond = 0xFF;

extern char analogStateFirst[8] = {0};
extern char analogStateSecond[8] = {0};

//analog stick bytes
extern char rxData = 0xF7;
extern char ryData = 0xF7;
extern char lxData = 0xF7;
extern char lyData = 0xF7;

char lutLX[256];
char lutLY[256];
char lutRX[256];
char lutRY[256];

unsigned char debounceLoops = 3;

unsigned char DigitalControllerByte1[8];
unsigned char DigitalControllerByte2[8];
unsigned char PreviousDigitalByte1[8];
unsigned char PreviousDigitalByte2[8];

unsigned char IndexDigitalByte1[8] = {0};
unsigned char IndexDigitalByte2[8] = {0};

unsigned char index = 0;

unsigned int readADC(int channel);
char reversebyte(char byte);

void readController(char analogMode);
void readControllerAnalog();
void lutInit();
void configureController();

#endif	/* CONTROLLER_H */