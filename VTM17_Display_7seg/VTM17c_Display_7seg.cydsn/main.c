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



/* Global variables used to store configuration and data for BASIC CAN mailbox */
CAN_DATA_BYTES_MSG dataPWM;
CAN_TX_MSG messagePWM;

/* Global variable used to store PWM pulse width value */
uint8 pulseWidthValue = 0u;
uint16 RPM = 1000;
uint16 TPS = 10;
uint RXMessage[8];
uint RXFlag = 0;
uint RXDLC = 0;

/* Global variable used to store receive message mailbox number */
volatile uint8 receiveMailboxNumber = 0xFFu;

//Global Variables 


int main()
{
    
    CyGlobalIntEnable;
    CAN_Init();
    CAN_Start();
    
    LED_Driver_LRBWS_Start();
    
	for(;;)
    {   
        if(RXFlag)
        {
            //Parse Data
        RXFlag =0;
        }        
    }
}

/* [] END OF FILE */
