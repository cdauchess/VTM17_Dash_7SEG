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

#define PWM_PULSE_WIDTH_STEP        (10u)
#define SWITCH_PRESSED              (0u)
#define PWM_MESSAGE_ID              (0x1AAu)
#define PWM_MESSAGE_IDE             (0u)    /* Standard message */
#define PWM_MESSAGE_IRQ             (0u)    /* No transmit IRQ */
#define PWM_MESSAGE_RTR             (0u)    /* No RTR */
#define CAN_RX_MAILBOX_0_SHIFT      (1u)
#define CAN_RX_MAILBOX_1_SHIFT      (2u)
#define DATA_SIZE                   (6u)
#define ONE_BYTE_OFFSET             (8u)

//Global Variables
/* Global variables used to store configuration and data for BASIC CAN mailbox */
CAN_DATA_BYTES_MSG dataPWM;
CAN_TX_MSG messagePWM;

//Global Variables for storage of data
unsigned short rpm;
unsigned short tp;
unsigned short map;
unsigned short at;
unsigned short et;
unsigned short la1;
unsigned short fp;
unsigned short op;
unsigned short egt1;
unsigned short launch;
unsigned short ntrl;
unsigned short bp;
unsigned short bpf1;
double batV;
unsigned short ldSpd;
unsigned short lgSpd;
unsigned short rdSpd;
unsigned short rgSpd;
unsigned short runTime;
unsigned short fuel;
unsigned short oilTemp;

//Global Variables for message interfacing 
uint RXMessage0[8];
uint RXMessage1[8];
uint RXMessage2[8];
uint RXMessage3[8];
uint RXMessage4[8];
uint RXMessage5[8];
uint RXFlag0 = 0;
uint RXFlag1 = 0;
uint RXFlag2 = 0;
uint RXFlag3 = 0;
uint RXFlag4 = 0;
uint RXFlag5 = 0;
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
    int Gear = 0;
    int PageButton = 0;
    int ReadyToDisplay = 1;
    int count = 0;
    int wait = 0;
    int StillPressed = 0;
    
    batV = 13.00;
    tp = 15;
    rpm = 6400;
    int PrevState = 1;
    
    //Test Variables
    int Left = 8;
    int Right = 9;
    int Direction = 1; //1 is up 0 is down
    
    CyGlobalIntEnable;
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
        if(wait == 500)
        {
            et++;
            count++;    
           batV = batV+0.01;
            wait = 0;
            //TEST ITEMS
            //Toggle Shift LEDS
            if(Direction == 1)
                rpm= rpm+10;
            else if(Direction == 0)
                rpm = rpm -10;
            
            if(rpm > 10000)
                Direction = 0;
            else if(rpm <6500)
                Direction = 1;           
        }
        wait++;
        if(count == 10)
        {
            count = 0;
        }
        if(RXFlag0 && RXFlag1 && RXFlag2 && RXFlag3 && RXFlag4 && RXFlag5)
        {
            //Parse Data
            ParseCAN();
            Gear = GearCalculation();
            //Reset flags, ready for next round of data    
            RXFlag0 =0;
            RXFlag1 =0;
            RXFlag2 =0;
            RXFlag3 =0;
            RXFlag4 =0;
            RXFlag5 =0;
            ReadyToDisplay = 1; 
        }
        switch(state)
        {
        case Drive: //Display items needed for driving: BatV, ECT, TBD, Gear, Shift Lights
            if(ReadyToDisplay == 1)
            {
                DriveUpdate(Gear);
                ReadyToDisplay = 1; 
                ShiftLights();
                WarningLights();
            }
            break;
        case Diag: //Display values wanted for diagnostic work: RPM?, TPS?, ECT?: all TBD maybe not even need this
            if(ReadyToDisplay == 1)
            {
                DiagUpdate(Gear);
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
