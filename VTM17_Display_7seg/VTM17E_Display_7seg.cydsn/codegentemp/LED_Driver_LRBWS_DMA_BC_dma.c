/***************************************************************************
* File Name: LED_Driver_LRBWS_DMA_BC_dma.c  
* Version 1.70
*
*  Description:
*   Provides an API for the DMAC component. The API includes functions
*   for the DMA controller, DMA channels and Transfer Descriptors.
*
*
*   Note:
*     This module requires the developer to finish or fill in the auto
*     generated funcions and setup the dma channel and TD's.
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/
#include <CYLIB.H>
#include <CYDMAC.H>
#include <LED_Driver_LRBWS_DMA_BC_dma.H>



/****************************************************************************
* 
* The following defines are available in Cyfitter.h
* 
* 
* 
* LED_Driver_LRBWS_DMA_BC__DRQ_CTL_REG
* 
* 
* LED_Driver_LRBWS_DMA_BC__DRQ_NUMBER
* 
* Number of TD's used by this channel.
* LED_Driver_LRBWS_DMA_BC__NUMBEROF_TDS
* 
* Priority of this channel.
* LED_Driver_LRBWS_DMA_BC__PRIORITY
* 
* True if LED_Driver_LRBWS_DMA_BC_TERMIN_SEL is used.
* LED_Driver_LRBWS_DMA_BC__TERMIN_EN
* 
* TERMIN interrupt line to signal terminate.
* LED_Driver_LRBWS_DMA_BC__TERMIN_SEL
* 
* 
* True if LED_Driver_LRBWS_DMA_BC_TERMOUT0_SEL is used.
* LED_Driver_LRBWS_DMA_BC__TERMOUT0_EN
* 
* 
* TERMOUT0 interrupt line to signal completion.
* LED_Driver_LRBWS_DMA_BC__TERMOUT0_SEL
* 
* 
* True if LED_Driver_LRBWS_DMA_BC_TERMOUT1_SEL is used.
* LED_Driver_LRBWS_DMA_BC__TERMOUT1_EN
* 
* 
* TERMOUT1 interrupt line to signal completion.
* LED_Driver_LRBWS_DMA_BC__TERMOUT1_SEL
* 
****************************************************************************/


/* Zero based index of LED_Driver_LRBWS_DMA_BC dma channel */
uint8 LED_Driver_LRBWS_DMA_BC_DmaHandle = DMA_INVALID_CHANNEL;

/*********************************************************************
* Function Name: uint8 LED_Driver_LRBWS_DMA_BC_DmaInitalize
**********************************************************************
* Summary:
*   Allocates and initialises a channel of the DMAC to be used by the
*   caller.
*
* Parameters:
*   BurstCount.
*       
*       
*   ReqestPerBurst.
*       
*       
*   UpperSrcAddress.
*       
*       
*   UpperDestAddress.
*       
*
* Return:
*   The channel that can be used by the caller for DMA activity.
*   DMA_INVALID_CHANNEL (0xFF) if there are no channels left. 
*
*
*******************************************************************/
uint8 LED_Driver_LRBWS_DMA_BC_DmaInitialize(uint8 BurstCount, uint8 ReqestPerBurst, uint16 UpperSrcAddress, uint16 UpperDestAddress) 
{

    /* Allocate a DMA channel. */
    LED_Driver_LRBWS_DMA_BC_DmaHandle = (uint8)LED_Driver_LRBWS_DMA_BC__DRQ_NUMBER;

    /* Configure the channel. */
    (void)CyDmaChSetConfiguration(LED_Driver_LRBWS_DMA_BC_DmaHandle,
                                  BurstCount,
                                  ReqestPerBurst,
                                  (uint8)LED_Driver_LRBWS_DMA_BC__TERMOUT0_SEL,
                                  (uint8)LED_Driver_LRBWS_DMA_BC__TERMOUT1_SEL,
                                  (uint8)LED_Driver_LRBWS_DMA_BC__TERMIN_SEL);

    /* Set the extended address for the transfers */
    (void)CyDmaChSetExtendedAddress(LED_Driver_LRBWS_DMA_BC_DmaHandle, UpperSrcAddress, UpperDestAddress);

    /* Set the priority for this channel */
    (void)CyDmaChPriority(LED_Driver_LRBWS_DMA_BC_DmaHandle, (uint8)LED_Driver_LRBWS_DMA_BC__PRIORITY);
    
    return LED_Driver_LRBWS_DMA_BC_DmaHandle;
}

/*********************************************************************
* Function Name: void LED_Driver_LRBWS_DMA_BC_DmaRelease
**********************************************************************
* Summary:
*   Frees the channel associated with LED_Driver_LRBWS_DMA_BC.
*
*
* Parameters:
*   void.
*
*
*
* Return:
*   void.
*
*******************************************************************/
void LED_Driver_LRBWS_DMA_BC_DmaRelease(void) 
{
    /* Disable the channel */
    (void)CyDmaChDisable(LED_Driver_LRBWS_DMA_BC_DmaHandle);
}

