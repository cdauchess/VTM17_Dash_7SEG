/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <device.h>
#include <stdio.h>

#define LeftDisp  0
#define RightDisp  5
#define BottomDisp  10

#define Shift1 7000
#define Shift2 7500
#define Shift3 8000
#define Shift4 8500
#define Shift5 9000
#define Overrev 500

//Global Variables for storage of data
extern unsigned short rpm;
extern unsigned short tp;
extern unsigned short map;
extern unsigned short at;
extern unsigned short et;
extern unsigned short la1;
extern unsigned short fp;
extern unsigned short op;
extern unsigned short egt1;
extern unsigned short launch;
extern unsigned short ntrl;
extern unsigned short bp;
extern unsigned short bpf1;
extern double batV;
extern unsigned short ldSpd;
extern unsigned short lgSpd;
extern unsigned short rdSpd;
extern unsigned short rgSpd;
extern unsigned short runTime;
extern unsigned short fuel;
extern unsigned short oilTemp;

//Global Variables for message interfacing 
extern uint RXMessage0[8];
extern uint RXMessage1[8];
extern uint RXMessage2[8];
extern uint RXMessage3[8];
extern uint RXMessage4[8];
extern uint RXMessage5[8];
extern uint RXFlag0;
extern uint RXFlag1;
extern uint RXFlag2;
extern uint RXFlag3;
extern uint RXFlag4;
extern uint RXFlag5;
extern uint RXDLC;
extern uint DispUpdate;

typedef enum{
    Drive, Diag

}STATE;

 STATE state;
 STATE PrevState;

//Shift Light Variables
extern int FlashCounter;
extern int FlashFreq;
extern int OffsetFlash;

/* Global variable used to store receive message mailbox number */
extern volatile uint8 receiveMailboxNumber;


//Function Prototypes

//This function parses the CAN data from Motec into usable and displayable values
int ParseCAN();

//I'd like to change this function to not include the shift lights so that they don't flash, or make a separate function to flash
//This function changes the brightness of the display
//int TargBrightness -> This is the value for the brightness of the shift lights, 4 digit displays
//int GearBrightness -> This is the value for the brightness of the gear indicator
int Brightness(int TargBrightness, int GearBrightness);

//This function updates the display in the driving state
int DriveUpdate(int Gear);

//This function updates the display in the diagnostic state.
int DiagUpdate(int Gear);

//This function performs that state switch, called when an external button is pressed
int SwitchButtonCheck(int StillPressed);

int GearCalculation();

int ShiftLights();
/* [] END OF FILE */
