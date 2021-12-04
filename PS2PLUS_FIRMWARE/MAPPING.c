/*
 * File:   MAPPING.c
 * Author: Gunnar
 *
 * Created on December 4, 2021, 1:24 PM
 */

#include "MAPPING.h"
#include "PicConfig.h"
#include <xc.h>

/*
void IO_MAPPING() {
    
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xFF;
    TRISE = 0xFF;
    
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    
    //rumble and led outputs
    TRISBbits.TRISB4 = 0;   //led
    TRISAbits.TRISA0 = 0;   //small motor
    TRISBbits.TRISB5 = 0;   //large motor
    
    //analog inputs
    ANSELBbits.ANSB1 = 1;   //LX
    ANSELBbits.ANSB0 = 1;   //LY
    ANSELEbits.ANSE0 = 1;   //RX
    ANSELEbits.ANSE1 = 1;   //RY

    //weak pull-ups on digital button pins
    WPUA = 0b11001110;
    WPUB = 0b00001100;
    WPUC = 0b11000111;
    WPUD = 0b11110000;
    WPUE = 0;
    
    TRISCbits.TRISC5 = 0; //clear pin for SDO1
    TRISAbits.TRISA4 = 0; //Set RA4 as output for ack
    
    ppsUnlock();
    RC5PPS = 0x15; //SDO1 on RC5 
    ppsLock();    
}
*/

/*
 //MRKANE mapping
void IO_MAPPING() {
    //all ports inputs
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xEF; //Rumble on RD4
    TRISE = 0xFF;

    //analog pins on RC0, RC1, RC6, RC7  
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0xC3;
    ANSELD = 0;
    ANSELE = 0;

    //weak pull-ups on digital button pins
    WPUA = 0xCF;
    WPUB = 0xFF;
    WPUC = 0x04;
    WPUD = 0x01;
    WPUE = 0;

    TRISCbits.TRISC5 = 0; //clear pin for SDO1
    TRISAbits.TRISA4 = 0; //Set RA4 as output for ack
    ppsUnlock();
    RC5PPS = 0x15; //SDO1 on RC5 
    ppsLock();
} 
 */


 //old mapping
void IO_MAPPING() {
    //all ports inputs
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xEF; //Rumble on RD4
    TRISE = 0xFF;

    //analog pins on RC0, RC1, RC6, RC7  
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0xC3;
    ANSELD = 0;
    ANSELE = 0;

    //weak pull-ups on digital button pins
    WPUA = 0xCF;
    WPUB = 0xFF;
    WPUC = 0x04;
    WPUD = 0x01;
    WPUE = 0;

    TRISCbits.TRISC5 = 0; //clear pin for SDO1
    TRISAbits.TRISA4 = 0; //Set RA4 as output for ack
    ppsUnlock();
    RC5PPS = 0x15; //SDO1 on RC5 
    ppsLock();
} 