/*******************************************************************************
* File Name: Page_Button.h  
* Version 1.90
*
* Description:
*  This file containts Status Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_STATUS_REG_Page_Button_H) /* CY_STATUS_REG_Page_Button_H */
#define CY_STATUS_REG_Page_Button_H

#include "cytypes.h"
#include "CyLib.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 statusState;

} Page_Button_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

uint8 Page_Button_Read(void) ;
void Page_Button_InterruptEnable(void) ;
void Page_Button_InterruptDisable(void) ;
void Page_Button_WriteMask(uint8 mask) ;
uint8 Page_Button_ReadMask(void) ;


/***************************************
*           API Constants
***************************************/

#define Page_Button_STATUS_INTR_ENBL    0x10u


/***************************************
*         Parameter Constants
***************************************/

/* Status Register Inputs */
#define Page_Button_INPUTS              1


/***************************************
*             Registers
***************************************/

/* Status Register */
#define Page_Button_Status             (* (reg8 *) Page_Button_sts_sts_reg__STATUS_REG )
#define Page_Button_Status_PTR         (  (reg8 *) Page_Button_sts_sts_reg__STATUS_REG )
#define Page_Button_Status_Mask        (* (reg8 *) Page_Button_sts_sts_reg__MASK_REG )
#define Page_Button_Status_Aux_Ctrl    (* (reg8 *) Page_Button_sts_sts_reg__STATUS_AUX_CTL_REG )

#endif /* End CY_STATUS_REG_Page_Button_H */


/* [] END OF FILE */
