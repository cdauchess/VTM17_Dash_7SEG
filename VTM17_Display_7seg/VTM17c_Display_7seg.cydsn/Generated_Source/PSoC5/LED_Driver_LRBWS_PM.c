/*******************************************************************************
* File Name: LED_Driver_LRBWS_PM.c
* Version 1.10
*
* Description:
*  This file provides the power management source code to API for the
*  LED_Driver component.
*
* Note:
*  None
*
*******************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "LED_Driver_LRBWS.h"
static LED_Driver_LRBWS_backupStruct LED_Driver_LRBWS_backup;


/*******************************************************************************
* Function Name: LED_Driver_LRBWS_Sleep
********************************************************************************
*
* Summary:
*  Stop and Save the user configuration
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LED_Driver_LRBWS_Sleep(void) 
{
    uint8 i;
    
    /* Store the component enable state */
    LED_Driver_LRBWS_backup.ledEnableState = LED_Driver_LRBWS_enableVar;
    
    /* Save the segment buffers */
    for(i = 0u; i < LED_Driver_LRBWS_NUM_COM; i++)
    {
        LED_Driver_LRBWS_backup.segLBackup[i] = LED_Driver_LRBWS_segLBuffer[i];
        #if (LED_Driver_LRBWS_NUM_SEG > 8u) 
            LED_Driver_LRBWS_backup.segMBackup[i] = LED_Driver_LRBWS_segMBuffer[i];
        #endif /* (LED_Driver_LRBWS_NUM_SEG > 8u) */
        #if (LED_Driver_LRBWS_NUM_SEG > 16u) 
            LED_Driver_LRBWS_backup.segHBackup[i] = LED_Driver_LRBWS_segHBuffer[i];
        #endif /* (LED_Driver_LRBWS_NUM_SEG > 16u) */
    }
    
    LED_Driver_LRBWS_Stop();
}


/*******************************************************************************
* Function Name: LED_Driver_LRBWS_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LED_Driver_LRBWS_Wakeup(void) 
{
    uint8 i;
    
    /* Restore the segment buffers */
    for(i = 0u; i < LED_Driver_LRBWS_NUM_COM; i++)
    {
        LED_Driver_LRBWS_segLBuffer[i] = LED_Driver_LRBWS_backup.segLBackup[i];
        #if (LED_Driver_LRBWS_NUM_SEG > 8u) 
            LED_Driver_LRBWS_segMBuffer[i] = LED_Driver_LRBWS_backup.segMBackup[i];
        #endif /* (LED_Driver_LRBWS_NUM_SEG > 8u) */
        #if (LED_Driver_LRBWS_NUM_SEG > 16u) 
            LED_Driver_LRBWS_segHBuffer[i] = LED_Driver_LRBWS_backup.segHBackup[i];
        #endif /* (LED_Driver_LRBWS_NUM_SEG > 16u) */
    }
    
    if(LED_Driver_LRBWS_backup.ledEnableState == LED_Driver_LRBWS_ENABLED)
    {
        /* Enable the component */
        LED_Driver_LRBWS_Enable();
    }
}


/* [] END OF FILE */

