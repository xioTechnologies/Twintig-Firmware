/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.h

  Summary:
    Interrupt vectors mapping

  Description:
    This file contains declarations of device vectors used by Harmony 3
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>



// *****************************************************************************
// *****************************************************************************
// Section: Handler Routines
// *****************************************************************************
// *****************************************************************************
void CHANGE_NOTICE_A_InterruptHandler( void );
void CHANGE_NOTICE_B_InterruptHandler( void );
void CHANGE_NOTICE_D_InterruptHandler( void );
void CHANGE_NOTICE_F_InterruptHandler( void );
void CHANGE_NOTICE_H_InterruptHandler( void );
void CHANGE_NOTICE_J_InterruptHandler( void );
void DRV_USBHS_InterruptHandler( void );
void DRV_USBHS_DMAInterruptHandler( void );

void Timer3InterruptHandler(void);
void Timer6InterruptHandler(void);
void Uart1RXInterruptHandler(void);
void Dma0InterruptHandler(void);
void Dma1InterruptHandler(void);
void Dma3InterruptHandler(void);
void Dma5InterruptHandler(void);
void Dma7InterruptHandler(void);
void Spi2RXInterruptHandler(void);
void Uart3RXInterruptHandler(void);
void Uart3TXInterruptHandler(void);
void Spi5RXInterruptHandler(void);


#endif // INTERRUPTS_H
