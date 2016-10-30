/*******************************************************************************
* File Name: main.c  
* Version 1.0
*
* Description:
*  Prints "- PSoC -" on an external x8 7-segment active high common and 
*  active high segment display at a refresh rate of 250 Hz per common.
*  The brightness of the "-" symbols are varied such that they gradually 
*  increase in brightness and then fades at a rate of approximately 2 Hz.
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <device.h>
#include <stdio.h>

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

#define LeftDisp  0
#define RightDisp  5
#define BottomDisp  10


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

typedef enum{
    Drive, Diag

}STATE;

STATE state;

/* Global variable used to store receive message mailbox number */
volatile uint8 receiveMailboxNumber = 0xFFu;

//Global Variables 

int ParseCAN()
{
    //Message ID 0x6F0
    rpm = (RXMessage0[0] << 8 | RXMessage0[1]);
    tp = (RXMessage0[2] << 8 | RXMessage0[3])*0.1;
    map = (RXMessage0[4] << 8 | RXMessage0[5])*0.1;
    at = (RXMessage0[6] << 8 | RXMessage0[7])*0.1;
    //Message ID 0x6F1
    et = (RXMessage1[0] << 8 | RXMessage1[1])*0.1;
    la1 = (RXMessage1[2] << 8 | RXMessage1[3])*0.001;
    fp = (RXMessage1[4] << 8 | RXMessage1[5])*0.1;
    op = (RXMessage1[6] << 8 | RXMessage1[7])*0.1;
    //Message ID 0x6F2
    egt1 = RXMessage2[0] << 8 | RXMessage2[1];
    launch = RXMessage2[2] << 8 | RXMessage2[3];
    ntrl = RXMessage2[4] << 8 | RXMessage2[5];
    bp = RXMessage2[6] << 8 | RXMessage2[7];
    //Message ID 0x6F3
    bpf1 = RXMessage3[0] << 8 | RXMessage3[1];
    batV = (double)(RXMessage3[2] << 8 | RXMessage3[3])*0.01;
    ldSpd = (RXMessage3[4] << 8 | RXMessage3[5])*0.1;
    lgSpd = (RXMessage3[6] << 8 | RXMessage3[7])*0.1;     
    //Message ID 0x6F4
    rdSpd = (RXMessage4[0] << 8 | RXMessage4[1]);
    rgSpd = (RXMessage4[2] << 8 | RXMessage4[3]);
    runTime = (RXMessage4[4] << 8 | RXMessage4[5]);
    fuel = (RXMessage4[6] << 8| RXMessage4[7]);
    //Message ID 0x6F5
    oilTemp = (RXMessage5[0] << 8 | RXMessage5[1]);
    DispUpdate = 1;
    
    return 0;
}

int Brightness(int TargBrightness, int GearBrightness)
{
    int i = 0;
    for(i = 0;i < 13; i++)
    {
    LED_Driver_LRBWS_SetBrightness(TargBrightness,i);
    }
    LED_Driver_Gear_SetBrightness(GearBrightness,0);
    return 0;
}

int DriveUpdate()
{
    
return 0;
}

int main()
{
    int test = 1;
    int ReadyToDisplay = 0;
    int count = 0;
    int wait = 0;
    int BrightGear = 180;
    int Bright4Dig = 255;
    batV = 13.00;
    tp = 15;
    int dummy = 8888;
    int i = 0;
    int PrevState = 1;
    char8 disp = 'A';
    CyGlobalIntEnable;
    //Start CAN
    CAN_Init();
    CAN_Start();
    
    //Start LED Drivers
    LED_Driver_LRBWS_Start();
    LED_Driver_Gear_Start();
    state = Drive;
    
    Brightness(Bright4Dig, BrightGear);
//    for(i = 0;i < 13; i++)
//    {
//    LED_Driver_LRBWS_SetBrightness(brightness1,0);
//    LED_Driver_LRBWS_SetBrightness(brightness1,1);
//    LED_Driver_LRBWS_SetBrightness(brightness1,2);
//    LED_Driver_LRBWS_SetBrightness(brightness1,3);
//    
//    LED_Driver_LRBWS_SetBrightness(brightness1,5);
//    LED_Driver_LRBWS_SetBrightness(brightness1,6);
//    LED_Driver_LRBWS_SetBrightness(brightness1,7);
//    LED_Driver_LRBWS_SetBrightness(brightness1,8);
//    
//    LED_Driver_LRBWS_SetBrightness(brightness1,10);
//    LED_Driver_LRBWS_SetBrightness(brightness1,11);
//    LED_Driver_LRBWS_SetBrightness(brightness1,12);
//    LED_Driver_LRBWS_SetBrightness(brightness1,13);
//    }
    
	for(;;)
    {   
        if(wait == 500000)
        {
            //LED_Driver_Gear_WriteString7Seg(&disp,0);
            LED_Driver_Gear_Write7SegNumberDec(count,0,1,LED_Driver_Gear_RIGHT_ALIGN);
           // LED_Driver_Gear_SetBrightness(brightness,0);
            LED_Driver_LRBWS_Write7SegNumberDec(et+20,0,4,LED_Driver_LRBWS_RIGHT_ALIGN);
            LED_Driver_LRBWS_Write7SegNumberDec(et-10,5,4,LED_Driver_LRBWS_RIGHT_ALIGN);
            LED_Driver_LRBWS_Write7SegNumberDec(et,10,4,LED_Driver_LRBWS_RIGHT_ALIGN);
            et++;
            count++;
            if(et > 200)
            {
                if(PrevState == 1)
                {
                Brightness(0,0);
                PrevState = 0;
                }
                else if(PrevState == 0)
                {
                Brightness(Bright4Dig,BrightGear);
                PrevState = 1;
                }
            }
            
           batV = batV+0.01;
            wait = 0;
            //disp++;
        }
        wait++;
        if(count == 10)
        {
            count = 0;
        }
        if(RXFlag0 && RXFlag1 && RXFlag2 && RXFlag3 && RXFlag4 && RXFlag5)
        {
            //Parse Data
            dummy = ParseCAN();

            //Reset flags, ready for next round of data    
            RXFlag0 =0;
            RXFlag1 =0;
            RXFlag2 =0;
            RXFlag3 =0;
            RXFlag4 =0;
            RXFlag5 =0;
            ReadyToDisplay = 1; 
        }
        if (test == 0)
        {
        switch(state)
        {
        case Drive: //Display items needed for driving: BatV, ECT, TBD, Gear, Shift Lights
            
            if(ReadyToDisplay == 1)
            {
            //Display the selected values
            if(rpm < 1500){ //Display Throttle Postion while cranking
            LED_Driver_LRBWS_Write7SegNumberDec(tp,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
            }
            else{ //Display RPM while running
            LED_Driver_LRBWS_Write7SegNumberDec(rpm,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);            
            }
            LED_Driver_LRBWS_Write7SegNumberDec(100*batV,RightDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN); //FIXME - need to set static decimal point
            LED_Driver_LRBWS_Write7SegNumberDec(et,BottomDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN); 
            ReadyToDisplay = 0;
            }
            break;
        case Diag: //Display values wanted for diagnostic work: RPM?, TPS?, ECT?: all TBD maybe not even need this
            if(ReadyToDisplay == 1)
            {
            //Display the selected values
            LED_Driver_LRBWS_Write7SegNumberDec(rpm,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
            LED_Driver_LRBWS_Write7SegNumberDec(100*batV,RightDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN); //FIXME - need to set static decimal point
            LED_Driver_LRBWS_Write7SegNumberDec(et,BottomDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN); 
            ReadyToDisplay = 0;
            }
            break;
        }
        }
    }
    
return 0;
}
/* [] END OF FILE */
