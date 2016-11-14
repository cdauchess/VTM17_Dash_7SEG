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

#define Shift1 7000
#define Shift2 7500
#define Shift3 8000
#define Shift4 8500
#define Shift5 9000
#define Overrev 500

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

typedef enum{
    Drive, Diag

}STATE;

STATE state;
STATE PrevState;

int FlashCounter = 0;
int FlashFreq = 5000;

/* Global variable used to store receive message mailbox number */
volatile uint8 receiveMailboxNumber = 0xFFu;

//This function parses the CAN data from Motec into usable and displayable values
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

//I'd like to change this function to not include the shift lights so that they don't flash, or make a separate function to flash
//This function changes the brightness of the display
//int TargBrightness -> This is the value for the brightness of the shift lights, 4 digit displays
//int GearBrightness -> This is the value for the brightness of the gear indicator
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

//This function updates the display in the driving state
int DriveUpdate(int Gear)
{
    //Display the selected values
    if(rpm < 1500){ //Display Throttle Postion while engine off
    LED_Driver_LRBWS_Write7SegNumberDec(tp,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    }
    else{ //Display RPM while running
    LED_Driver_LRBWS_Write7SegNumberDec(rpm,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);            
    }
    LED_Driver_LRBWS_Write7SegNumberDec(100*batV,RightDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    LED_Driver_LRBWS_SetRC(15,1);//Set DP on position 6 of display
    LED_Driver_LRBWS_Write7SegNumberDec(et,BottomDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN); 
    LED_Driver_Gear_Write7SegDigitDec(Gear,0);
               
return 0;
}

//This function updates the display in the diagnostic state.
int DiagUpdate(int Gear)
{
     //Display the selected values
    LED_Driver_LRBWS_Write7SegNumberDec(rpm,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    LED_Driver_LRBWS_Write7SegNumberDec(100*batV,RightDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    LED_Driver_LRBWS_SetRC(15,1);//Set DP on position 6 of display
    LED_Driver_LRBWS_Write7SegNumberDec(8888,BottomDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);  
    LED_Driver_Gear_Write7SegDigitDec(Gear,0);
return 0;
}


//This function performs that state switch, called when an external button is pressed
int SwitchButtonCheck(int StillPressed)
{
    int ButtonVal = 0;
    ButtonVal = Page_Button_Read();
//Switch State to the other state
if(StillPressed == 0 && ButtonVal == 1) //Only switch state first time through the loop, prevent continuous oscillation if you hold the button down
{
    if(PrevState == Drive)
    {
        state = Diag;
    }
    else if(PrevState == Diag)
    {
        state = Drive;
    }
    PrevState = state; //Set the "prevstate" variable to the current state now that it has changed
    StillPressed = 1;
}
else if(StillPressed == 1 && ButtonVal == 0)
    StillPressed = 0;
return StillPressed;
}

int GearCalculation()
{
//Gear Calculation
//Convert Wheel speed to wheel RPM
int Gear = 0;
float Red1 = 18.6955;
float Red2 = 13.4048;
float Red3 = 10.1561;
float Red4 = 8.1218;
float Red5 = 6.4974;

float ToleranceHighGear =  1;
float ToleranceLowGear = 2.25;
 
float TireD = 18;
float TireC = TireD*3.14159;
float TireMPR = TireC/(5280*12);

float AvgRWhlSpd = (ldSpd + rdSpd)/2;

float wheelRPM = AvgRWhlSpd/(60*TireMPR);
float CalcRed = 0;

CalcRed = rpm/wheelRPM;

//Calculate the Gear
if(Red1-ToleranceLowGear < CalcRed && Red1+ToleranceLowGear > CalcRed)
    Gear = 1;
else if(Red2-ToleranceLowGear < CalcRed && Red2+ToleranceLowGear > CalcRed)
    Gear = 2;
else if(Red3-ToleranceHighGear < CalcRed && Red3+ToleranceHighGear > CalcRed)
    Gear = 3;
else if(Red4-ToleranceHighGear < CalcRed && Red4+ToleranceHighGear > CalcRed)
    Gear = 4;
else if(Red5-ToleranceHighGear < CalcRed && Red5+ToleranceHighGear > CalcRed)
    Gear = 5;
else
    Gear = 0;
    
return Gear;
}

int ShiftLights()
{
    int Column = 4;
    int i = 0;
    int Light1 = 8;
    int Light2 = 10;
    int Light3 = 12;
    int Light4 = 15;
    int Light5 = 17;
    
    if(rpm>= Shift1 && rpm < Shift2)
    {
        LED_Driver_LRBWS_SetRC(8,Column);
        LED_Driver_LRBWS_SetRC(9,Column);
        
        LED_Driver_LRBWS_ClearRC(10,Column);
        LED_Driver_LRBWS_ClearRC(11,Column);
        LED_Driver_LRBWS_ClearRC(12,Column);
        LED_Driver_LRBWS_ClearRC(13,Column);
        LED_Driver_LRBWS_ClearRC(14,Column);
        LED_Driver_LRBWS_ClearRC(15,Column);
        LED_Driver_LRBWS_ClearRC(16,Column);
        LED_Driver_LRBWS_ClearRC(17,Column);
    }
    else if(rpm>= Shift2 && rpm < Shift3)
    {
        LED_Driver_LRBWS_SetRC(8,Column);
        LED_Driver_LRBWS_SetRC(9,Column);
        LED_Driver_LRBWS_SetRC(10,Column);
        LED_Driver_LRBWS_SetRC(11,Column);
        
        LED_Driver_LRBWS_ClearRC(12,Column);
        LED_Driver_LRBWS_ClearRC(13,Column);
        LED_Driver_LRBWS_ClearRC(14,Column);
        LED_Driver_LRBWS_ClearRC(15,Column);
        LED_Driver_LRBWS_ClearRC(16,Column);
        LED_Driver_LRBWS_ClearRC(17,Column);        
    }
    else if(rpm>= Shift3 && rpm < Shift4)
    {
        LED_Driver_LRBWS_SetRC(8,Column);
        LED_Driver_LRBWS_SetRC(9,Column);
        LED_Driver_LRBWS_SetRC(10,Column);
        LED_Driver_LRBWS_SetRC(11,Column);
        LED_Driver_LRBWS_SetRC(12,Column);
        LED_Driver_LRBWS_SetRC(13,Column);
        
        LED_Driver_LRBWS_ClearRC(14,Column);
        LED_Driver_LRBWS_ClearRC(15,Column);
        LED_Driver_LRBWS_ClearRC(16,Column);
        LED_Driver_LRBWS_ClearRC(17,Column);        
    }
    else if(rpm>= Shift4 && rpm < Shift5)
    {
        LED_Driver_LRBWS_SetRC(8,Column);
        LED_Driver_LRBWS_SetRC(9,Column);
        LED_Driver_LRBWS_SetRC(10,Column);
        LED_Driver_LRBWS_SetRC(11,Column);
        LED_Driver_LRBWS_SetRC(12,Column);
        LED_Driver_LRBWS_SetRC(13,Column);
        LED_Driver_LRBWS_SetRC(14,Column);
        LED_Driver_LRBWS_SetRC(15,Column);
        
        LED_Driver_LRBWS_ClearRC(16,Column);
        LED_Driver_LRBWS_ClearRC(17,Column);  
    }
    else if(rpm >= Shift5 && rpm < Shift5+Overrev)
    {
        LED_Driver_LRBWS_SetRC(8,Column);
        LED_Driver_LRBWS_SetRC(9,Column);
        LED_Driver_LRBWS_SetRC(10,Column);
        LED_Driver_LRBWS_SetRC(11,Column);
        LED_Driver_LRBWS_SetRC(12,Column);
        LED_Driver_LRBWS_SetRC(13,Column);
        LED_Driver_LRBWS_SetRC(14,Column);
        LED_Driver_LRBWS_SetRC(15,Column);
        LED_Driver_LRBWS_SetRC(16,Column);
        LED_Driver_LRBWS_SetRC(17,Column);
    }
    else if(rpm >= Shift5+Overrev && FlashCounter > FlashFreq)
    {
        
        //Flash all Lights
        for(i = Light1; i<=Light5; i++)
        {
        LED_Driver_LRBWS_ToggleRC(i,Column);
        }
        FlashCounter = 0;
    }
    else if(rpm < Shift1)
    {
    //Turn off all lights
        for(i = Light1; i<=Light5; i++)
        {
        LED_Driver_LRBWS_ClearRC(i,Column);
        }        
    }
    FlashCounter++;
    if(FlashCounter > 10000)
        FlashCounter = 0;
return 0;
}

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
            else if(rpm <7000)
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
            }
            break;
        case Diag: //Display values wanted for diagnostic work: RPM?, TPS?, ECT?: all TBD maybe not even need this
            if(ReadyToDisplay == 1)
            {
                DiagUpdate(Gear);
                ReadyToDisplay = 1;
            }
            break;
        }
        StillPressed = SwitchButtonCheck(StillPressed);
    }  
return 0;
}
/* [] END OF FILE */
