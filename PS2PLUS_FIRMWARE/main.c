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
#include <stdint.h>

char response[20] = {DIGITAL_MODE, END_HEADER};
char responseLength = 9;
char cmdCounter = 0;
char analogMode = 0; //0: 0x41 mode 1: 0x73 mode 2: 0x79 mode
char escapeMode = 0;
char previousCmd;
char MODE_LOCK = 0; //when 1, you cannot use the analog button to switch modes
char AN_latch = 0;  //used to toggle analog mode on falling edge of analog button

char INIT_PRESSURE_SENSOR_byte3 = 0;    //stores data from 0x40 command
char INIT_PRESSURE_SENSOR_byte4 = 0;    //stores data from 0x40 command
char MAP_SMALL_MOTOR = 0xFF;
char MAP_LARGE_MOTOR = 0xFF;
char CONTROL_RESPONSE_byte3 = 0;    //stores data from 0x4F command
char CONTROL_RESPONSE_byte4 = 0;    //stores data from 0x4F command
char CONTROL_RESPONSE_byte5 = 0;    //stores data from 0x4F command

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

        switch (cmdCounter) { 
            case 3:
                switch (previousCmd) {
                    case INIT_PRESSURE_SENSOR:                       
                        INIT_PRESSURE_SENSOR_byte3 = cmd;                    
                        break;
                    case MAIN_POLLING:                      
                        if(MAP_SMALL_MOTOR == 0 && cmd == 0xFF) SMALL_MOTOR = 1;
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
                        CONTROL_RESPONSE_byte3 = cmd;
                        break;
                    case MAP_MOTOR:
                        MAP_SMALL_MOTOR = cmd;
                        break;
                }
                break;
            case 4:
                switch (previousCmd) {              
                    case INIT_PRESSURE_SENSOR:                        
                        INIT_PRESSURE_SENSOR_byte4 = cmd;                     
                        break;
                    case MAIN_POLLING:
                        if(MAP_LARGE_MOTOR == 0x80 && reversebyte(cmd) >= 0x40) LARGE_MOTOR = 1;
                        else LARGE_MOTOR = 0;
                        break;
                    case ANALOG_DIGITAL_SWITCH:
                        if(cmd == 0xC0) MODE_LOCK = 1;
                        else MODE_LOCK = 0;
                        break;
                    case CONTROL_RESPONSE:
                        //Obtain which buttons we want to collect analog data from here
                        CONTROL_RESPONSE_byte4 = cmd;
                        break;
                    case MAP_MOTOR:
                        MAP_LARGE_MOTOR = cmd;
                        break;
                }
                break;
            case 5:
                switch (previousCmd) {
                    case CONTROL_RESPONSE:
                        //Obtain which buttons we want to collect analog data from here
                        CONTROL_RESPONSE_byte5 = cmd;
                        break;
                }
                break;
            default:
                switch (cmd) {
                    case INIT_PRESSURE_SENSOR:
                        /* 
                         * This command initializes the pressure buttons individually
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x40;
                        response[5] = 0x00;
                        response[6] = 0x00;
                        response[7] = 0x5A;            
                        previousCmd = cmd;
                        break;
                    case BUTTON_INCLUSION:
                        /*
                         * This command asks for which buttons and sticks are active. This feature
                         * is controlled by CONTROL_RESPONSE. The response is 9 bytes long with 18 total bits
                         * that represent the different button and analog stick states.
                         */
                        if(analogMode == 0) {
                            response[2] = 0x00;
                            response[3] = 0x00;
                            response[4] = 0x00;
                            response[7] = 0x00;
                        }
                        if(analogMode >= 1) {
                            //there isn't any recorded documentation of the button masking being used so we will always return all button masks
                            //response[2] = CONTROL_RESPONSE_byte3;
                            //response[3] = CONTROL_RESPONSE_byte4;
                            //response[4] = CONTROL_RESPONSE_byte5; 
                            response[2] = 0xFF;
                            response[3] = 0xFF;
                            response[4] = 0xC0; //0x03 Reversed 
                            response[7] = 0x5A;
                        }
                        response[5] = 0x00;
                        response[6] = 0x00;
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
                         * to control the motors.
                         */
                        response[2] = MAP_SMALL_MOTOR;
                        response[3] = MAP_LARGE_MOTOR;
                        response[4] = 0xFF;
                        response[5] = 0xFF;
                        response[6] = 0xFF;
                        response[7] = 0xFF;
                        previousCmd = cmd;
                        break;
                    case CONTROL_RESPONSE:
                        /*
                         * This command can add or remove analog values from the response packet
                         */
                        response[2] = 0x00;
                        response[3] = 0x00;
                        response[4] = 0x00;
                        response[5] = 0x00;
                        response[6] = 0x00;
                        response[7] = 0x5A;
                        previousCmd = cmd;
                        if(analogMode == 1) analogMode = 2; //there isn't any recorded documentation of the button masking being used so we will always go into 0x79 mode for now
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
        
        //Analog button mode switch
        if(!AN_btn){
            if(!MODE_LOCK) {
                if(AN_latch) {
                    if(analogMode >= 1) analogMode = 0;
                    else analogMode = 1;
                    AN_latch = 0;
                }
            }
        }
        else AN_latch = 1;
             
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