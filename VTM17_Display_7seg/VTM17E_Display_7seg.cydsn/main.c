/*******************************************************************************
* File Name: main.c  
* Version 1.0
*
* Description:
*  REFER TO "PSOC PIN INFO.xlsx" in 2017\Electrical\VTM17Display for info on row/column assignments
*   
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <device.h>
#include <stdio.h>
#include "Dash_Funcs.h"

//#define PWM_PULSE_WIDTH_STEP        (10u)
//#define SWITCH_PRESSED              (0u)
//#define PWM_MESSAGE_ID              (0x1AAu)
//#define PWM_MESSAGE_IDE             (0u)    /* Standard message */
//#define PWM_MESSAGE_IRQ             (0u)    /* No transmit IRQ */
//#define PWM_MESSAGE_RTR             (0u)    /* No RTR */
//#define CAN_RX_MAILBOX_0_SHIFT      (1u)
//#define CAN_RX_MAILBOX_1_SHIFT      (2u)
//#define DATA_SIZE                   (6u)
//#define ONE_BYTE_OFFSET             (8u)

//Global Variables
/* Global variables used to store configuration and data for BASIC CAN mailbox */
CAN_DATA_BYTES_MSG dataPWM;
CAN_TX_MSG messagePWM;

//Global Variables for storage of data
signed short motorSpeed;
unsigned short nodeStatus;
double nodeDCVoltage;
unsigned short rmsMotorCurrent;
unsigned short heatSinkTemp;
unsigned short motorTemp;
unsigned short dcCurrent;
unsigned short voltageAngle;
unsigned short vmc30State;
unsigned short steeringFiltered;
unsigned short throttleFiltered;
unsigned short errorBits;
unsigned short commandWord;
unsigned short commandCurrent;
unsigned short accelCurrentLimit;
signed short decelCurrentLimit;
unsigned short bmsCCL;
signed short bmsDCL;
unsigned short bmsHighTemp;
signed short bmsPackCurrent;
unsigned short regen1;
unsigned short regen2;
unsigned short throttlePercent;
unsigned short brakePercent;
unsigned short cellID;
unsigned short instVoltage;
unsigned short openVoltage;
unsigned short checkSum;

//Global Variables for message interfacing 
uint RXMessage0[8];
uint RXMessage1[8];
uint RXMessage2[8];
uint RXMessage3[8];
uint RXMessage4[8];
uint RXMessage5[8];
uint RXMessage6[8];
uint RXFlag0 = 0;
uint RXFlag1 = 0;
uint RXFlag2 = 0;
uint RXFlag3 = 0;
uint RXFlag4 = 0;
uint RXFlag5 = 0;
uint RXFlag6 = 0;
uint RXDLC = 0;
uint DispUpdate = 0;


//typedef enum{
//    Drive, Diag
//
//}STATE;

extern STATE state;
extern STATE PrevState;

//Shift Light Variables
int FlashCounter = 0;
int FlashFreq = 5000;
int OffsetFlash = 0;

/* Global variable used to store receive message mailbox number */
volatile uint8 receiveMailboxNumber = 0xFFu;


int main()
{
    //Parameters
    int BrightGear = 120;
    int Bright4Dig = 255;
    
    //Variables
    int PageButton = 0;
    int ReadyToDisplay = 1;
    int count = 0;
    int wait = 0;
    int StillPressed = 0;
    
    //Test variable sets
    nodeDCVoltage = 13.00;
    throttleFiltered = 15;
    motorSpeed = 6400;
    int Left = 8;
    int Right = 9;
    int Direction = 1; //1 is up 0 is down
    
   // int PrevState = 1;//is this needed?
    
    CyGlobalIntEnable; //Enable global interrupts
    //Start CAN
    CAN_Init();
    CAN_Start();
    
    //Start LED Drivers
    LED_Driver_LRBWS_Start();
    LED_Driver_Gear_Start();
    state = Drive;
    PrevState = state;
    
    Brightness(Bright4Dig, BrightGear); //Set initial Brightness
    int i = 0;
        for(i = 8; i <= 17; i++)
        {
        LED_Driver_LRBWS_SetRC(i,4);
        }    
    
	for(;;)
    {   
        //The following if statement items are for testing only, remove when display is on vehicle.
        //FIXME
//        if(wait == 500)
//        {
//            motorTemp++;
//            count++;    
//            nodeDCVoltage = nodeDCVoltage+0.01;
//            wait = 0;
//            //TEST ITEMS
//            //Toggle Shift LEDS
//            if(Direction == 1)
//                motorSpeed = motorSpeed+10;
//            else if(Direction == 0)
//                motorSpeed = motorSpeed -10;
//            
//            if(motorSpeed > 10000)
//                Direction = 0;
//            else if(motorSpeed <6500)
//                Direction = 1;           
//        }
//        wait++;
//        if(count == 10)
//        {
//            count = 0;
//        }
        if(RXFlag0 || RXFlag1 || RXFlag2 || RXFlag3 || RXFlag4 || RXFlag5 || RXFlag6) //If any data is new update the display
        {
            //Parse Data
            ParseCAN();
            //Reset flags, ready for next round of data    
            RXFlag0 =0;
            RXFlag1 =0;
            RXFlag2 =0;
            RXFlag3 =0;
            RXFlag4 =0;
            RXFlag5 =0;
            RXFlag6 =0;
            ReadyToDisplay = 1; 
        }
        switch(state)
        {
        case Drive: //Display items needed for driving
            if(ReadyToDisplay == 1)
            {
                DriveUpdate(0);
                ReadyToDisplay = 1; 
                ShiftLights();
                WarningLights();
            }
            break;
        case Diag: //Display values wanted for diagnostic work
            if(ReadyToDisplay == 1)
            {
                DiagUpdate(0);
                ShiftLights();
                WarningLights();
                ReadyToDisplay = 1;
            }
            break;
        }
        StillPressed = SwitchButtonCheck(StillPressed);
    }  
return 0;
}
/* [] END OF FILE */
