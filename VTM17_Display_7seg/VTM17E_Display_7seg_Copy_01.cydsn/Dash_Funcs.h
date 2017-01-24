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
extern signed short motorSpeed;
extern unsigned short nodeStatus;
extern double nodeDCVoltage;
extern unsigned short rmsMotorCurrent;
extern unsigned short heatSinkTemp;
extern unsigned short motorTemp;
extern unsigned short dcCurrent;
extern unsigned short voltageAngle;
extern unsigned short vmc30State;
extern unsigned short steeringFiltered;
extern unsigned short throttleFiltered;
extern unsigned short errorBits;
extern unsigned short commandWord;
extern unsigned short commandCurrent;
extern unsigned short accelCurrentLimit;
extern signed short decelCurrentLimit;
extern unsigned short bmsCCL;
extern signed short bmsDCL;
extern unsigned short bmsHighTemp;
extern signed short bmsPackCurrent;
extern unsigned short regen;
extern unsigned short cellID;
extern unsigned short instVoltage;
extern unsigned short openVoltage;
extern unsigned short checkSum;

//Global Variables for message interfacing 
extern uint RXMessage0[8];
extern uint RXMessage1[8];
extern uint RXMessage2[8];
extern uint RXMessage3[8];
extern uint RXMessage4[8];
extern uint RXMessage5[8];
extern uint RXMessage6[8];
extern uint RXFlag0;
extern uint RXFlag1;
extern uint RXFlag2;
extern uint RXFlag3;
extern uint RXFlag4;
extern uint RXFlag5;
extern uint RXFlag6;
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

//This function Calculates the current gear that the engine is in using rear wheel speeds and engine RPM
//This function returns an integer indicating the current gear (0,1,2,3,4,5)
int GearCalculation();

//This function handles whether or not to turn the shift lights on
//The cutoff values are in the defines above
int ShiftLights();

//This Function handles the illumination of the warning lights
int WarningLights();

/* [] END OF FILE */
