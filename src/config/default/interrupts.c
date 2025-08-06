/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "interrupts.h"
#include "definitions.h"



// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


/* All the handlers are defined here.  Each will call its PLIB-specific function. */

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector declarations
// *****************************************************************************
// *****************************************************************************
void TIMER_3_Handler (void);
void TIMER_6_Handler (void);
void UART1_RX_Handler (void);
void CHANGE_NOTICE_A_Handler (void);
void CHANGE_NOTICE_B_Handler (void);
void CHANGE_NOTICE_D_Handler (void);
void CHANGE_NOTICE_F_Handler (void);
void CHANGE_NOTICE_H_Handler (void);
void CHANGE_NOTICE_J_Handler (void);
void USB_Handler (void);
void USB_DMA_Handler (void);
void DMA0_Handler (void);
void DMA1_Handler (void);
void DMA3_Handler (void);
void DMA5_Handler (void);
void DMA7_Handler (void);
void SPI2_RX_Handler (void);
void UART3_RX_Handler (void);
void UART3_TX_Handler (void);
void SPI5_RX_Handler (void);


// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector definitions
// *****************************************************************************
// *****************************************************************************
void __attribute__((used)) __ISR(_TIMER_3_VECTOR, ipl7SRS) TIMER_3_Handler (void)
{
    Timer3InterruptHandler();
}

void __attribute__((used)) __ISR(_TIMER_6_VECTOR, ipl1SRS) TIMER_6_Handler (void)
{
    Timer6InterruptHandler();
}

void __attribute__((used)) __ISR(_UART1_RX_VECTOR, ipl1SRS) UART1_RX_Handler (void)
{
    Uart1RxInterruptHandler();
}

void __attribute__((used)) __ISR(_CHANGE_NOTICE_A_VECTOR, ipl5SRS) CHANGE_NOTICE_A_Handler (void)
{
    CHANGE_NOTICE_A_InterruptHandler();
}

void __attribute__((used)) __ISR(_CHANGE_NOTICE_B_VECTOR, ipl5SRS) CHANGE_NOTICE_B_Handler (void)
{
    CHANGE_NOTICE_B_InterruptHandler();
}

void __attribute__((used)) __ISR(_CHANGE_NOTICE_D_VECTOR, ipl5SRS) CHANGE_NOTICE_D_Handler (void)
{
    CHANGE_NOTICE_D_InterruptHandler();
}

void __attribute__((used)) __ISR(_CHANGE_NOTICE_F_VECTOR, ipl5SRS) CHANGE_NOTICE_F_Handler (void)
{
    CHANGE_NOTICE_F_InterruptHandler();
}

void __attribute__((used)) __ISR(_CHANGE_NOTICE_H_VECTOR, ipl5SRS) CHANGE_NOTICE_H_Handler (void)
{
    CHANGE_NOTICE_H_InterruptHandler();
}

void __attribute__((used)) __ISR(_CHANGE_NOTICE_J_VECTOR, ipl5SRS) CHANGE_NOTICE_J_Handler (void)
{
    CHANGE_NOTICE_J_InterruptHandler();
}

void __attribute__((used)) __ISR(_USB_VECTOR, ipl1SRS) USB_Handler (void)
{
    DRV_USBHS_InterruptHandler();
}

void __attribute__((used)) __ISR(_USB_DMA_VECTOR, ipl1SRS) USB_DMA_Handler (void)
{
    DRV_USBHS_DMAInterruptHandler();
}

void __attribute__((used)) __ISR(_DMA0_VECTOR, ipl1SRS) DMA0_Handler (void)
{
    Dma0InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA1_VECTOR, ipl1SRS) DMA1_Handler (void)
{
    Dma1InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA3_VECTOR, ipl1SRS) DMA3_Handler (void)
{
    Dma3InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA5_VECTOR, ipl1SRS) DMA5_Handler (void)
{
    Dma5InterruptHandler();
}

void __attribute__((used)) __ISR(_DMA7_VECTOR, ipl1SRS) DMA7_Handler (void)
{
    Dma7InterruptHandler();
}

void __attribute__((used)) __ISR(_SPI2_RX_VECTOR, ipl1SRS) SPI2_RX_Handler (void)
{
    Spi2RxInterruptHandler();
}

void __attribute__((used)) __ISR(_UART3_RX_VECTOR, ipl1SRS) UART3_RX_Handler (void)
{
    Uart3RxInterruptHandler();
}

void __attribute__((used)) __ISR(_UART3_TX_VECTOR, ipl1SRS) UART3_TX_Handler (void)
{
    Uart3TxInterruptHandler();
}

void __attribute__((used)) __ISR(_SPI5_RX_VECTOR, ipl1SRS) SPI5_RX_Handler (void)
{
    Spi5RxInterruptHandler();
}




/*******************************************************************************
 End of File
*/
