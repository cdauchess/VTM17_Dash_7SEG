#include <device.h>
#include <stdio.h>
#include "Dash_Funcs.h"


//This function parses the CAN data from Motec into usable and displayable values
int ParseCAN()
{
    //Message ID 0x187
    motorSpeed = (RXMessage0[0] | (RXMessage0[1]<<8));
    nodeStatus =(RXMessage0[2] | (RXMessage0[3]<<8));
    nodeDCVoltage = (RXMessage0[4] | (RXMessage0[5]<<8))*0.01; //Centivolts //Make variable a float
    rmsMotorCurrent = (RXMessage0[6] | (RXMessage0[7]<<8));
    //Message ID 0x287
    heatSinkTemp = (RXMessage1[0] | (RXMessage1[1]<<8));
    motorTemp =(RXMessage1[2] | (RXMessage1[3]<<8));
    dcCurrent = (RXMessage1[4] | (RXMessage1[5]<<8));
    voltageAngle = (RXMessage1[6] | (RXMessage1[7]<<8));
    //Message ID 0x205
    vmc30State = (RXMessage2[0] | (RXMessage2[1]<<8));
    steeringFiltered =(RXMessage2[2] | (RXMessage2[3]<<8));
    throttleFiltered = (RXMessage2[4] | (RXMessage2[5]<<8));
    errorBits = (RXMessage1[6] | (RXMessage2[7]<<8)); 
    //Message ID 0x201
    commandWord = (RXMessage3[0] | (RXMessage3[1]<<8));
    commandCurrent =(RXMessage3[2] | (RXMessage3[3]<<8));
    accelCurrentLimit = (RXMessage3[4] | (RXMessage3[5]<<8));
    decelCurrentLimit = (RXMessage3[6] | (RXMessage3[7]<<8));   
    //Message ID 0x183
    bmsCCL = (RXMessage4[0] | (RXMessage4[1]<<8));
    bmsDCL =(RXMessage4[2] | (RXMessage4[3]<<8));
    bmsHighTemp = (RXMessage4[4] | (RXMessage4[5]<<8));
    bmsPackCurrent = (RXMessage4[6] | (RXMessage4[7]<<8));
    //Message ID 0x305
    regen1 = (RXMessage5[0]);
    regen2 = (RXMessage5[1]);
    brakePercent = (RXMessage5[2] | (RXMessage5[3]<<8));
    throttlePercent = (RXMessage5[4] | (RXMessage5[5]<<8));
    
    //Message ID 0x100
    cellID = RXMessage6[0]+1;
    instVoltage =(RXMessage6[2] | (RXMessage6[3]<<8));
    openVoltage = (RXMessage6[4] | (RXMessage6[5]<<8));
    checkSum = (RXMessage6[7]);
    //All messages received, update the display
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
    LED_Driver_LRBWS_Write7SegNumberDec(motorSpeed,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);            
    LED_Driver_LRBWS_Write7SegNumberDec(100*nodeDCVoltage,RightDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    LED_Driver_LRBWS_SetRC(15,1);//Set DP on position 6 of display
    LED_Driver_LRBWS_Write7SegNumberDec(motorTemp,BottomDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN); 
    LED_Driver_Gear_Write7SegDigitDec(Gear,0);
               
return 0;
}

//This function updates the display in the diagnostic state.
int DiagUpdate(int Gear)
{
     //Display the selected values
    LED_Driver_LRBWS_Write7SegNumberDec(throttleFiltered,LeftDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    LED_Driver_LRBWS_Write7SegNumberDec(vmc30State,RightDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);
    LED_Driver_LRBWS_SetRC(15,1);//Set DP on position 6 of display
    LED_Driver_LRBWS_Write7SegNumberDec(throttlePercent,BottomDisp,4,LED_Driver_LRBWS_RIGHT_ALIGN);  
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

//Gear Calculation not neccesary on EV
//int GearCalculation()
//{
////Gear Calculation
////Convert Wheel speed to wheel RPM
//int Gear = 0;
////Gear Reduction values for each gear.    
//float Red1 = 18.6955;
//float Red2 = 13.4048;
//float Red3 = 10.1561;
//float Red4 = 8.1218;
//float Red5 = 6.4974;
//
//float ToleranceHighGear =  1;
//float ToleranceLowGear = 2.25;
// 
//float TireD = 18;
//float TireC = TireD*3.14159;
//float TireMPR = TireC/(5280*12);
//
//float AvgRWhlSpd = motorSpeed;
//
//float wheelRPM = AvgRWhlSpd/(60*TireMPR);
//float CalcRed = 0;
//
//CalcRed = rpm/wheelRPM;
//
////Calculate the Gear
//if(Red1-ToleranceLowGear < CalcRed && Red1+ToleranceLowGear > CalcRed)
//    Gear = 1;
//else if(Red2-ToleranceLowGear < CalcRed && Red2+ToleranceLowGear > CalcRed)
//    Gear = 2;
//else if(Red3-ToleranceHighGear < CalcRed && Red3+ToleranceHighGear > CalcRed)
//    Gear = 3;
//else if(Red4-ToleranceHighGear < CalcRed && Red4+ToleranceHighGear > CalcRed)
//    Gear = 4;
//else if(Red5-ToleranceHighGear < CalcRed && Red5+ToleranceHighGear > CalcRed)
//    Gear = 5;
//else
//    Gear = 0;
//    
//return Gear;
//}

int ShiftLights() //Need to decide what to display here
{
    int Column = 4;
    int i = 0;
    int Light1 = 8;
    int Light2 = 10;
    int Light3 = 12;
    int Light4 = 15;
    int Light5 = 17;
    
    if(motorSpeed>= Shift1 && motorSpeed < Shift2)
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
        
        OffsetFlash = 0;
    }
    else if(motorSpeed>= Shift2 && motorSpeed < Shift3)
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
        
        OffsetFlash = 0;
    }
    else if(motorSpeed>= Shift3 && motorSpeed < Shift4)
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
        
        OffsetFlash = 0;
    }
    else if(motorSpeed>= Shift4 && motorSpeed < Shift5)
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
        
        OffsetFlash = 0;
    }
    else if(motorSpeed >= Shift5 && motorSpeed < Shift5+Overrev)
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
        
        OffsetFlash = 0;
    }
    else if(motorSpeed >= Shift5+Overrev && FlashCounter > FlashFreq)
    {
        if(OffsetFlash == 0)
        {
        LED_Driver_LRBWS_ToggleRC(16,Column);
        LED_Driver_LRBWS_ToggleRC(17,Column);
        OffsetFlash = 1;
        }
                
        //Flash all Lights
        for(i = Light1; i<=Light5; i++)
        {
        LED_Driver_LRBWS_ToggleRC(i,Column);
        }
        FlashCounter = 0;
    }
    else if(motorSpeed < Shift1)
    {
    //Turn off all lights
        for(i = Light1; i<=Light5; i++)
        {
        LED_Driver_LRBWS_ClearRC(i,Column);
        }
        OffsetFlash = 0;
    }
    FlashCounter++;
    if(FlashCounter > 10000)
        FlashCounter = 0;
return 0;
}

//Motortemp, voltage, battery temp?, 
int WarningLights()
{
    //Values to be checked for warning 
    int Left1 = motorTemp;
    int Left2 = 1;
    int Left3 = 1;
    double Right1 = nodeDCVoltage;
    int Right2 = 0;
    int Right3 = 1;
    //Warning Thresholds
    int Left1Thresh = 215; //>
    int Left2Thresh = 1600;//>
    int Left3Thresh = 250;//>
    double Right1Thresh = 12.00;//<
    int Right2Thresh = 0;
    int Right3Thresh = 40;//<
        
    //Left1
        if(Left1 > Left1Thresh)
        {
        LED_Driver_LRBWS_SetRC(0,4);
        }
        else
        {
        LED_Driver_LRBWS_ClearRC(0,4);
        }
    //Left2
        if(Left2 > Left2Thresh)
        {
        LED_Driver_LRBWS_SetRC(1,4);
        }
        else
        {
        LED_Driver_LRBWS_ClearRC(1,4);
        }        
    //Left3
         if(Left3 > Left3Thresh)
        {
        LED_Driver_LRBWS_SetRC(2,4);
        }
        else
        {
        LED_Driver_LRBWS_ClearRC(2,4);
        }       
    //Right1
        if(Right1 < Right1Thresh)
        {
        LED_Driver_LRBWS_SetRC(3,4);
        }
        else
        {
        LED_Driver_LRBWS_ClearRC(3,4);
        }        
    //Right2
        if(Right2 > Right2Thresh)
        {
        LED_Driver_LRBWS_SetRC(4,4);
        }
        else
        {
        LED_Driver_LRBWS_ClearRC(4,4);
        }        
    //Right3
        if(Right3 < Right3Thresh)
        {
        LED_Driver_LRBWS_SetRC(5,4);
        }
        else
        {
        LED_Driver_LRBWS_ClearRC(5,4);
        }        
    return 0;
}
