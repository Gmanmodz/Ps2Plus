/* 
 * File:  Controller.h 
 * Author: Matthew Cranford
 */

#ifndef MAPPING_H
#define	MAPPING_H

#include <xc.h> 

/*
//new mapping
#define SQUARE RA6
#define TRIANGLE RC0
#define XBUTTON RE2
#define CIRCLE RA7

#define DUP RD4
#define DDOWN RD7
#define DLEFT RD6
#define DRIGHT RD5

#define L1BUTTON RC6
#define L2BUTTON RC7
#define L3BUTTON RB2
#define R1BUTTON RC2
#define R2BUTTON RC1
#define R3BUTTON RA3

#define START RA2
#define SELECT RB3

#define LEFT_ANALOG_X RB1
#define LEFT_ANALOG_Y RB0
#define RIGHT_ANALOG_X RE0
#define RIGHT_ANALOG_Y RE1

#define LX_ADC_CHAN 0b001001
#define LY_ADC_CHAN 0b001000
#define RX_ADC_CHAN 0b100000
#define RY_ADC_CHAN 0b100001

#define ANALOG_BTN RA1
#define LED RB4

#define SMALL_MOTOR RA0
#define LARGE_MOTOR RB5

#define SCK RC3
#define MOSI RC4
#define MISO RC5
#define ACK RA4
#define SLAVE_SELECT RA5
*/

//mapping for MRKANE
/*
#define SQUARE RA2
#define TRIANGLE RA3
#define XBUTTON RA1
#define CIRCLE RA0

#define DUP RB2
#define DDOWN RB4
#define DLEFT RB5
#define DRIGHT RB3

#define L1BUTTON RB1
#define L2BUTTON RB0
#define L3BUTTON RD0
#define R1BUTTON RA7
#define R2BUTTON RA6
#define R3BUTTON RC2

#define START RB6
#define SELECT RB7

#define LEFT_ANALOG_X RC6
#define LEFT_ANALOG_Y RC7
#define RIGHT_ANALOG_X RC0
#define RIGHT_ANALOG_Y RC1

#define RX_ADC_CHAN 0b10001
#define RY_ADC_CHAN 0b010000
#define LX_ADC_CHAN 0b010110
#define LY_ADC_CHAN 0b010111

#define ANALOG_BTN RD4
#define LED RD4
 
#define SMALL_MOTOR RD4
#define LARGE_MOTOR RD4

#define SCK RC3
#define MOSI RC4
#define MISO RC5
#define ACK RA4
#define SLAVE_SELECT RA5
*/

//original mapping
#define SQUARE RA0
#define TRIANGLE RA1
#define XBUTTON RA2
#define CIRCLE RA3

#define DUP RA6
#define DDOWN RA7
#define DLEFT RB0
#define DRIGHT RB1

#define L1BUTTON RB2
#define L2BUTTON RB3
#define L3BUTTON RC2
#define R1BUTTON RB4
#define R2BUTTON RB5
#define R3BUTTON RD0

#define START RB6
#define SELECT RB7

#define LEFT_ANALOG_X RC0
#define LEFT_ANALOG_Y RC1
#define RIGHT_ANALOG_X RC6
#define RIGHT_ANALOG_Y RC7

#define ANALOG_BTN RD4
#define LED RD5

#define SMALL_MOTOR RD6
#define LARGE_MOTOR RD7

#define LX_ADC_CHAN 0b10001
#define LY_ADC_CHAN 0b010000
#define RX_ADC_CHAN 0b010110
#define RY_ADC_CHAN 0b010111

#define SCK
#define MOSI
#define MISO
#define ACK RA4
#define SLAVE_SELECT RA5

void IO_MAPPING();

#endif	