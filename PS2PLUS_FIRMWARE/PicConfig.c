/*
 * File:   PicConfig.C
 * Author: Matthew Cranford 
 */


#include "PicConfig.h"

void ppsUnlock() {
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 0; //PPS unlocked	
}

void ppsLock() {
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCK = 1; //PPS locked   	
}

void adcInit() {
    ADCON0 = 0x04;
    ADCON1 = 0;
    ADCON2 = 0;
    ADREF = 0; //VDD is VREF    
    ADCLK = 0x3F; //ADC clock = FOSC/128
    ADCON0bits.ADON = 1;
}

void picInit() {
    //enable SPI slave mode
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_HIGH, SPI_ACTIVE_2_IDLE);

    //enable interrupts
    INTCONbits.GIE = 1; //enable all global interrupts
    INTCONbits.PEIE = 1; //peripheral interrupt enable    
    SSP1IF = 0;
    SSP1IE = 1; //enable MSSP interrupt    
}