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

int main()
{	
    uint8 bright, cntMode;
    
    LED_Driver_Start();
    LED_Driver_WriteString7Seg("- PSoC -",0u);
    /* Display the large Hexadecimal "C" */
    LED_Driver_Write7SegDigitHex(0x0Cu,5u);
    
    bright = 0xFF;
    cntMode = 1u;
    
	for(;;)
    {   
        CyDelay(4u);
                
        /* Change the brightness level */
        LED_Driver_SetBrightness(bright,0u);
        LED_Driver_SetBrightness(bright,7u);
        
        if(bright == 0x01u)
        {
            cntMode = 1u;
        }
        else if(bright == 0xFFu)
        {
            cntMode = 0u;
        }
        
        if(cntMode)
        {
            bright++;
        }
        else
        {
            bright--;
        }
    }
}

/* [] END OF FILE */
