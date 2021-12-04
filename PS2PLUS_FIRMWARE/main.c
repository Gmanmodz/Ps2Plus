/*
 * File:   main.c
 * Author: Matthew Cranford
 *
 * Created on June 25, 2019, 10:49 PM
 */

#include <xc.h>
#include "Opcodes.h"
#include "Spi.h"
#include "PicConfig.h"
#include "Controller.h"
#include "Nvm.h"
#include "MAPPING.h"

char response[20] = {DIGITAL_MODE, END_HEADER};
char responseLength = 9;
char cmdCounter = 0;
char analogMode = 0; //0: digital mode 1: 0x73 mode 2: 0x79 mode
char escapeMode = 0;
char previousCmd;

char INIT_PRESSURE_SENSOR_byte3 = 0;    //stores data from 0x40 command
char INIT_PRESSURE_SENSOR_byte4 = 0;    //stores data from 0x40 command

void pollController(char response[20]) {
    response[2] = digitalStateFirst;
    response[3] = digitalStateSecond;
    response[4] = rxData;
    response[5] = ryData;
    response[6] = lxData;
    response[7] = lyData;   
    if(analogMode == 2) {
        responseLength = 20;
        response[8] = analogStateFirst[2];
        response[9] = analogStateFirst[0];
        response[10] = analogStateFirst[3];
        response[11] = analogStateFirst[1];
        response[12] = analogStateSecond[3];
        response[13] = analogStateSecond[2];
        response[14] = analogStateSecond[1];
        response[15] = analogStateSecond[0];
        response[16] = analogStateSecond[5];
        response[17] = analogStateSecond[4];
        response[18] = analogStateSecond[7];
        response[19] = analogStateSecond[6];
    }
    else {
        responseLength = 5;
    }
}

void __interrupt() PS2Command() {

    if (SSP1IF) {
        char cmd = spiRead();

        switch (cmdCounter) { //We only care about the first command, the 3rd command and the 4th.
            case 3:
                switch (previousCmd) {
                    case INIT_PRESSURE_SENSOR:                       
                        INIT_PRESSURE_SENSOR_byte3 = cmd;                    
                        break;
                    case MAIN_POLLING:                      
                        if(cmd == 0xFF) SMALL_MOTOR = 1;
                        else SMALL_MOTOR = 0;                   
                        break;
                    case ENTER_EXIT_ESCAPE:
                        if (cmd == 0x80) escapeMode = 1;
                        else escapeMode = 0;
                        break;
                    case ANALOG_DIGITAL_SWITCH:
                        if (cmd == 0x80) analogMode = 1;
                        else analogMode = 0; 
                        break;
                    case DEVICE_DESCRIPTOR_FIRST:
                        if (cmd == 0x80) { //command 0x01
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x80; //0x01 reversed
                            response[5] = 0x80; //0x01 reversed
                            response[6] = 0x80; //0x01 reversed
                            response[7] = 0x28; //0x14 reversed
                        } else {
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x80; //0x01 reversed
                            response[5] = 0x40; //0x02 reversed
                            response[6] = 0x00;
                            response[7] = 0x50; //0x0A reversed
                        }                                                  
                        break;
                    case DEVICE_DESCRIPTOR_LAST:
                        if (cmd == 0x80) {
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x00;
                            response[5] = 0xE0; 
                            response[6] = 0x00;
                            response[7] = 0x00;
                        } else {
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x00;
                            response[5] = 0x20;
                            response[6] = 0x00;
                            response[7] = 0x00;
                        }
                        break;
                    case CONTROL_RESPONSE:
                        //Obtain which buttons we want to collect analog data from here
                        break;
                }
                break;

            case 4:
                switch (previousCmd) { //Large Motor                
                    case INIT_PRESSURE_SENSOR:                        
                        INIT_PRESSURE_SENSOR_byte4 = cmd;                     
                        break;
                    case MAIN_POLLING:
                        if (reversebyte(cmd) >= 0x40) LARGE_MOTOR = 1;
                        else LARGE_MOTOR = 0;
                        break;
                    case CONTROL_RESPONSE:
                        //Obtain which buttons we want to collect analog data from here
                        break;
                }
                break;
            default:
                switch (cmd) {
                    case INIT_PRESSURE_SENSOR:
                        /* 
                         * This command tells us what buttons it wants 
                         * from analog information from.
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x40;
                        response[5] = 0x00;
                        response[6] = 0x00;
                        response[7] = 0x5A;            
                        analogMode = 2;        //if we get here, let's go into 0x79 mode for now
                        previousCmd = cmd;
                        break;
                    case BUTTON_INCLUSION:
                        /*
                         * This command asks for which buttons and sticks are turned on. This feature
                         * is controlled by CONTROL_RESPONSE. The response is 9 bytes long with 18 total bits
                         * that represent the different button and analog stick states.
                         */
                        if (analogMode >= 1) {
                            response[2] = 0xFF;
                            response[3] = 0xFF;
                            response[4] = 0xC0; //0x03 Reversed 
                            response[5] = 0x00;
                            response[6] = 0x00;
                            response[7] = 0x5A;
                        } else {
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x00;
                            response[5] = 0x00;
                            response[6] = 0x00;
                            response[7] = 0x00;
                        }
                        break;
                    case MAIN_POLLING:
                        /*
                         * This is the main polling command. If the 3rd and 4th cmd bytes
                         * of the command have data in them then we need to turn on motors.
                         */
                        pollController(response);
                        previousCmd = cmd;
                        break;
                    case ENTER_EXIT_ESCAPE:
                        /*
                         * This command acts exactly like MAIN_POLLING command 
                         * except that depending on the 3rd byte tells the controller
                         * to be in escape mode or not.
                         */
                        if(escapeMode) {
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x00;
                            response[5] = 0x00;
                            response[6] = 0x00;
                            response[7] = 0x00;
                        }
                        else {
                            pollController(response);              
                        }                     
                        previousCmd = cmd;                      
                        break;
                    case ANALOG_DIGITAL_SWITCH:
                        /*
                         * Depending on what the 3rd byte of this command is
                         * the controller will change between analog and digital states.
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x00;
                        response[5] = 0x00;
                        response[6] = 0x00;
                        response[7] = 0x00;
                        previousCmd = cmd;                       
                        break;
                    case STATUS_INFO:
                        /*
                         * The PS2 is requesting certain information about the controller
                         * like whether the controller is a guitar hero controller
                         * or DualShock controller.
                         * 
                         */
                        response[2] = 0xC0; //0x03 reversed (DualShock)
                        response[3] = 0x40; //0x02 reversed
                        if (analogMode >= 1) response[4] = 0x80; //States whether the LED is on.
                        else response[4] = 0x00;
                        response[5] = 0x40; //0x02 reversed
                        response[6] = 0x80; //0x01 reversed
                        response[7] = 0x00;
                        break;
                    case DEVICE_DESCRIPTOR_FIRST:
                        /*
                         * This commands deal with reading a fixed constant from
                         * the controller. The following was obtained from a sample
                         * playing Kingdom Hearts 2. The one I found online did not work.
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x80; //0x01 reversed
                        response[5] = 0x80;
                        response[6] = 0x80;
                        response[7] = 0x50; //0x0A reversed                       
                        previousCmd = cmd;
                        break;
                    case DEVICE_DESCRIPTOR_SECOND:
                        /*
                         * This commands deal with reading a fixed constant from
                         * the controller. The following was obtained from a sample
                         * playing Kingdom Hearts 2. The one I found online did not work
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x40; //0x02 reversed
                        response[5] = 0x00;
                        response[6] = 0x80; //0x01 reversed
                        response[7] = 0x00;
                        break;
                    case DEVICE_DESCRIPTOR_LAST:
                        /*
                         * This commands deal with reading a fixed constant from
                         * the controller. The following was obtained from a sample
                         * playing Kingdom Hearts 2. The one I found online did not work
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x00;
                        response[5] = 0x20; //04 reversed
                        response[6] = 0x00; //0x01 reversed
                        response[7] = 0x00;
                        previousCmd = cmd;
                        break;
                    case MAP_MOTOR:
                        /*
                         * This command tells the controller that MAIN_POLLING will now issue commands
                         * to control the motors. There is no need to keep track of the state since we 
                         * have logic in place to already handle this scenario. Instead we just send
                         * a default response.
                         */
                        response[2] = 0xFF;
                        response[3] = 0xFF;
                        response[4] = 0xFF;
                        response[5] = 0xFF;
                        response[6] = 0xFF;
                        response[7] = 0xFF;
                        break;
                    case CONTROL_RESPONSE:
                        /*
                         * Difficult command as it controls whether or not we return analog values for buttons
                         * i.e. keeping track of more state. For now will create default response. 
                         * Further testing across many different games is needed to see 
                         * if this will cause problems
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x00;
                        response[5] = 0x00;
                        response[6] = 0x00;
                        response[7] = 0x5A;
                        break;
                }
                break;
        }

        //transmit
        spiWrite(response[cmdCounter]);
        
        ACK = 0;
        __delay_us(1);
        ACK = 1;

        //Increment index to next command
        if (cmdCounter < responseLength) {
            cmdCounter++;
        }
        
        //Output the correct controller state
        if (escapeMode) {
            response[0] = ESCAPE_MODE;
        } 
        else {
            if(analogMode == 0) response[0] = DIGITAL_MODE;
            if(analogMode == 1) response[0] = ANALOG_MODE;
            if(analogMode > 1) response[0] = ANALOG_MODE_PRESSURE;
        }        
     
        SSP1IF = 0;

    }
}

void main(void) {

    picInit();
    IO_MAPPING();
    adcInit();
    lutInit();

    response[1] = END_HEADER; //Finish header

    char slaveSelect;
    char slaveSelectStatePrev = 0;
    char count = 0;
    ACK = 1; //Acknowledge is an active low line, so we initialize it high.
    
    while (1) {
        //L1 L2 Select
        if(digitalStateFirst == 0x7F && digitalStateSecond == 0x5F){    
            configureController();
        }     
        
        if(analogMode >= 1) LED = 1;
        else LED = 0;
        
        slaveSelect = SLAVE_SELECT;
        if (slaveSelect) if(count < 3) count++;
        if (slaveSelect ^ slaveSelectStatePrev) count = 0;
        slaveSelectStatePrev = slaveSelect;
        if (count >= 3) { //Clear transmission counters
            cmdCounter = 0;
            responseLength = 9;
            SSP1BUF = 0xFF;
            previousCmd = 0x00;
        }

        readController(analogMode);
        readControllerAnalog();
    }  
}