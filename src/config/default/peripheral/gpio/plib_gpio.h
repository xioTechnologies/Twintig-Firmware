/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h UUUUUUUUU

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for SDO_CH5 pin ***/
#define SDO_CH5_Get()               ((PORTC >> 1) & 0x1U)
#define SDO_CH5_GetLatch()          ((LATC >> 1) & 0x1U)
#define SDO_CH5_PIN                  GPIO_PIN_RC1

/*** Macros for SCL_HAPTIC pin ***/
#define SCL_HAPTIC_Set()               (LATCSET = (1U<<2))
#define SCL_HAPTIC_Clear()             (LATCCLR = (1U<<2))
#define SCL_HAPTIC_Toggle()            (LATCINV= (1U<<2))
#define SCL_HAPTIC_OutputEnable()      (TRISCCLR = (1U<<2))
#define SCL_HAPTIC_InputEnable()       (TRISCSET = (1U<<2))
#define SCL_HAPTIC_Get()               ((PORTC >> 2) & 0x1U)
#define SCL_HAPTIC_GetLatch()          ((LATC >> 2) & 0x1U)
#define SCL_HAPTIC_PIN                  GPIO_PIN_RC2

/*** Macros for SDA_HAPTIC pin ***/
#define SDA_HAPTIC_Set()               (LATCSET = (1U<<3))
#define SDA_HAPTIC_Clear()             (LATCCLR = (1U<<3))
#define SDA_HAPTIC_Toggle()            (LATCINV= (1U<<3))
#define SDA_HAPTIC_OutputEnable()      (TRISCCLR = (1U<<3))
#define SDA_HAPTIC_InputEnable()       (TRISCSET = (1U<<3))
#define SDA_HAPTIC_Get()               ((PORTC >> 3) & 0x1U)
#define SDA_HAPTIC_GetLatch()          ((LATC >> 3) & 0x1U)
#define SDA_HAPTIC_PIN                  GPIO_PIN_RC3

/*** Macros for SDI_CH5 pin ***/
#define SDI_CH5_Get()               ((PORTC >> 4) & 0x1U)
#define SDI_CH5_GetLatch()          ((LATC >> 4) & 0x1U)
#define SDI_CH5_PIN                  GPIO_PIN_RC4

/*** Macros for SCLK_CH5 pin ***/
#define SCLK_CH5_Get()               ((PORTG >> 6) & 0x1U)
#define SCLK_CH5_GetLatch()          ((LATG >> 6) & 0x1U)
#define SCLK_CH5_PIN                  GPIO_PIN_RG6

/*** Macros for SDA_CH5 pin ***/
#define SDA_CH5_Get()               ((PORTG >> 7) & 0x1U)
#define SDA_CH5_GetLatch()          ((LATG >> 7) & 0x1U)
#define SDA_CH5_PIN                  GPIO_PIN_RG7

/*** Macros for SCL_CH5 pin ***/
#define SCL_CH5_Get()               ((PORTG >> 8) & 0x1U)
#define SCL_CH5_GetLatch()          ((LATG >> 8) & 0x1U)
#define SCL_CH5_PIN                  GPIO_PIN_RG8

/*** Macros for CS1_CH5 pin ***/
#define CS1_CH5_Set()               (LATKSET = (1U<<0))
#define CS1_CH5_Clear()             (LATKCLR = (1U<<0))
#define CS1_CH5_Toggle()            (LATKINV= (1U<<0))
#define CS1_CH5_OutputEnable()      (TRISKCLR = (1U<<0))
#define CS1_CH5_InputEnable()       (TRISKSET = (1U<<0))
#define CS1_CH5_Get()               ((PORTK >> 0) & 0x1U)
#define CS1_CH5_GetLatch()          ((LATK >> 0) & 0x1U)
#define CS1_CH5_PIN                  GPIO_PIN_RK0

/*** Macros for ENABLE_CH5 pin ***/
#define ENABLE_CH5_Set()               (LATGSET = (1U<<9))
#define ENABLE_CH5_Clear()             (LATGCLR = (1U<<9))
#define ENABLE_CH5_Toggle()            (LATGINV= (1U<<9))
#define ENABLE_CH5_OutputEnable()      (TRISGCLR = (1U<<9))
#define ENABLE_CH5_InputEnable()       (TRISGSET = (1U<<9))
#define ENABLE_CH5_Get()               ((PORTG >> 9) & 0x1U)
#define ENABLE_CH5_GetLatch()          ((LATG >> 9) & 0x1U)
#define ENABLE_CH5_PIN                  GPIO_PIN_RG9

/*** Macros for CS2_CH5 pin ***/
#define CS2_CH5_Set()               (LATASET = (1U<<0))
#define CS2_CH5_Clear()             (LATACLR = (1U<<0))
#define CS2_CH5_Toggle()            (LATAINV= (1U<<0))
#define CS2_CH5_OutputEnable()      (TRISACLR = (1U<<0))
#define CS2_CH5_InputEnable()       (TRISASET = (1U<<0))
#define CS2_CH5_Get()               ((PORTA >> 0) & 0x1U)
#define CS2_CH5_GetLatch()          ((LATA >> 0) & 0x1U)
#define CS2_CH5_PIN                  GPIO_PIN_RA0

/*** Macros for CS3_CH5 pin ***/
#define CS3_CH5_Set()               (LATESET = (1U<<8))
#define CS3_CH5_Clear()             (LATECLR = (1U<<8))
#define CS3_CH5_Toggle()            (LATEINV= (1U<<8))
#define CS3_CH5_OutputEnable()      (TRISECLR = (1U<<8))
#define CS3_CH5_InputEnable()       (TRISESET = (1U<<8))
#define CS3_CH5_Get()               ((PORTE >> 8) & 0x1U)
#define CS3_CH5_GetLatch()          ((LATE >> 8) & 0x1U)
#define CS3_CH5_PIN                  GPIO_PIN_RE8

/*** Macros for CS4_CH5 pin ***/
#define CS4_CH5_Set()               (LATESET = (1U<<9))
#define CS4_CH5_Clear()             (LATECLR = (1U<<9))
#define CS4_CH5_Toggle()            (LATEINV= (1U<<9))
#define CS4_CH5_OutputEnable()      (TRISECLR = (1U<<9))
#define CS4_CH5_InputEnable()       (TRISESET = (1U<<9))
#define CS4_CH5_Get()               ((PORTE >> 9) & 0x1U)
#define CS4_CH5_GetLatch()          ((LATE >> 9) & 0x1U)
#define CS4_CH5_PIN                  GPIO_PIN_RE9

/*** Macros for SDI_CH4 pin ***/
#define SDI_CH4_Get()               ((PORTB >> 5) & 0x1U)
#define SDI_CH4_GetLatch()          ((LATB >> 5) & 0x1U)
#define SDI_CH4_PIN                  GPIO_PIN_RB5

/*** Macros for INT1_CH5 pin ***/
#define INT1_CH5_Set()               (LATBSET = (1U<<4))
#define INT1_CH5_Clear()             (LATBCLR = (1U<<4))
#define INT1_CH5_Toggle()            (LATBINV= (1U<<4))
#define INT1_CH5_OutputEnable()      (TRISBCLR = (1U<<4))
#define INT1_CH5_InputEnable()       (TRISBSET = (1U<<4))
#define INT1_CH5_Get()               ((PORTB >> 4) & 0x1U)
#define INT1_CH5_GetLatch()          ((LATB >> 4) & 0x1U)
#define INT1_CH5_PIN                  GPIO_PIN_RB4
#define INT1_CH5_InterruptEnable()   (CNENBSET = (1U<<4))
#define INT1_CH5_InterruptDisable()  (CNENBCLR = (1U<<4))

/*** Macros for INT2_CH5 pin ***/
#define INT2_CH5_Set()               (LATJSET = (1U<<11))
#define INT2_CH5_Clear()             (LATJCLR = (1U<<11))
#define INT2_CH5_Toggle()            (LATJINV= (1U<<11))
#define INT2_CH5_OutputEnable()      (TRISJCLR = (1U<<11))
#define INT2_CH5_InputEnable()       (TRISJSET = (1U<<11))
#define INT2_CH5_Get()               ((PORTJ >> 11) & 0x1U)
#define INT2_CH5_GetLatch()          ((LATJ >> 11) & 0x1U)
#define INT2_CH5_PIN                  GPIO_PIN_RJ11
#define INT2_CH5_InterruptEnable()   (CNENJSET = (1U<<11))
#define INT2_CH5_InterruptDisable()  (CNENJCLR = (1U<<11))

/*** Macros for INT3_CH5 pin ***/
#define INT3_CH5_Set()               (LATJSET = (1U<<13))
#define INT3_CH5_Clear()             (LATJCLR = (1U<<13))
#define INT3_CH5_Toggle()            (LATJINV= (1U<<13))
#define INT3_CH5_OutputEnable()      (TRISJCLR = (1U<<13))
#define INT3_CH5_InputEnable()       (TRISJSET = (1U<<13))
#define INT3_CH5_Get()               ((PORTJ >> 13) & 0x1U)
#define INT3_CH5_GetLatch()          ((LATJ >> 13) & 0x1U)
#define INT3_CH5_PIN                  GPIO_PIN_RJ13
#define INT3_CH5_InterruptEnable()   (CNENJSET = (1U<<13))
#define INT3_CH5_InterruptDisable()  (CNENJCLR = (1U<<13))

/*** Macros for INT4_CH5 pin ***/
#define INT4_CH5_Set()               (LATJSET = (1U<<14))
#define INT4_CH5_Clear()             (LATJCLR = (1U<<14))
#define INT4_CH5_Toggle()            (LATJINV= (1U<<14))
#define INT4_CH5_OutputEnable()      (TRISJCLR = (1U<<14))
#define INT4_CH5_InputEnable()       (TRISJSET = (1U<<14))
#define INT4_CH5_Get()               ((PORTJ >> 14) & 0x1U)
#define INT4_CH5_GetLatch()          ((LATJ >> 14) & 0x1U)
#define INT4_CH5_PIN                  GPIO_PIN_RJ14
#define INT4_CH5_InterruptEnable()   (CNENJSET = (1U<<14))
#define INT4_CH5_InterruptDisable()  (CNENJCLR = (1U<<14))

/*** Macros for CS2_CH4 pin ***/
#define CS2_CH4_Set()               (LATJSET = (1U<<15))
#define CS2_CH4_Clear()             (LATJCLR = (1U<<15))
#define CS2_CH4_Toggle()            (LATJINV= (1U<<15))
#define CS2_CH4_OutputEnable()      (TRISJCLR = (1U<<15))
#define CS2_CH4_InputEnable()       (TRISJSET = (1U<<15))
#define CS2_CH4_Get()               ((PORTJ >> 15) & 0x1U)
#define CS2_CH4_GetLatch()          ((LATJ >> 15) & 0x1U)
#define CS2_CH4_PIN                  GPIO_PIN_RJ15

/*** Macros for SDO_CH4 pin ***/
#define SDO_CH4_Get()               ((PORTB >> 3) & 0x1U)
#define SDO_CH4_GetLatch()          ((LATB >> 3) & 0x1U)
#define SDO_CH4_PIN                  GPIO_PIN_RB3

/*** Macros for CS1_CH4 pin ***/
#define CS1_CH4_Set()               (LATBSET = (1U<<2))
#define CS1_CH4_Clear()             (LATBCLR = (1U<<2))
#define CS1_CH4_Toggle()            (LATBINV= (1U<<2))
#define CS1_CH4_OutputEnable()      (TRISBCLR = (1U<<2))
#define CS1_CH4_InputEnable()       (TRISBSET = (1U<<2))
#define CS1_CH4_Get()               ((PORTB >> 2) & 0x1U)
#define CS1_CH4_GetLatch()          ((LATB >> 2) & 0x1U)
#define CS1_CH4_PIN                  GPIO_PIN_RB2

/*** Macros for SDI_CH2 pin ***/
#define SDI_CH2_Get()               ((PORTB >> 6) & 0x1U)
#define SDI_CH2_GetLatch()          ((LATB >> 6) & 0x1U)
#define SDI_CH2_PIN                  GPIO_PIN_RB6

/*** Macros for ENABLE_CH4 pin ***/
#define ENABLE_CH4_Set()               (LATBSET = (1U<<7))
#define ENABLE_CH4_Clear()             (LATBCLR = (1U<<7))
#define ENABLE_CH4_Toggle()            (LATBINV= (1U<<7))
#define ENABLE_CH4_OutputEnable()      (TRISBCLR = (1U<<7))
#define ENABLE_CH4_InputEnable()       (TRISBSET = (1U<<7))
#define ENABLE_CH4_Get()               ((PORTB >> 7) & 0x1U)
#define ENABLE_CH4_GetLatch()          ((LATB >> 7) & 0x1U)
#define ENABLE_CH4_PIN                  GPIO_PIN_RB7

/*** Macros for INT1_CH4 pin ***/
#define INT1_CH4_Set()               (LATASET = (1U<<9))
#define INT1_CH4_Clear()             (LATACLR = (1U<<9))
#define INT1_CH4_Toggle()            (LATAINV= (1U<<9))
#define INT1_CH4_OutputEnable()      (TRISACLR = (1U<<9))
#define INT1_CH4_InputEnable()       (TRISASET = (1U<<9))
#define INT1_CH4_Get()               ((PORTA >> 9) & 0x1U)
#define INT1_CH4_GetLatch()          ((LATA >> 9) & 0x1U)
#define INT1_CH4_PIN                  GPIO_PIN_RA9
#define INT1_CH4_InterruptEnable()   (CNENASET = (1U<<9))
#define INT1_CH4_InterruptDisable()  (CNENACLR = (1U<<9))

/*** Macros for ENABLE_CH3 pin ***/
#define ENABLE_CH3_Set()               (LATASET = (1U<<10))
#define ENABLE_CH3_Clear()             (LATACLR = (1U<<10))
#define ENABLE_CH3_Toggle()            (LATAINV= (1U<<10))
#define ENABLE_CH3_OutputEnable()      (TRISACLR = (1U<<10))
#define ENABLE_CH3_InputEnable()       (TRISASET = (1U<<10))
#define ENABLE_CH3_Get()               ((PORTA >> 10) & 0x1U)
#define ENABLE_CH3_GetLatch()          ((LATA >> 10) & 0x1U)
#define ENABLE_CH3_PIN                  GPIO_PIN_RA10

/*** Macros for INT2_CH4 pin ***/
#define INT2_CH4_Set()               (LATHSET = (1U<<0))
#define INT2_CH4_Clear()             (LATHCLR = (1U<<0))
#define INT2_CH4_Toggle()            (LATHINV= (1U<<0))
#define INT2_CH4_OutputEnable()      (TRISHCLR = (1U<<0))
#define INT2_CH4_InputEnable()       (TRISHSET = (1U<<0))
#define INT2_CH4_Get()               ((PORTH >> 0) & 0x1U)
#define INT2_CH4_GetLatch()          ((LATH >> 0) & 0x1U)
#define INT2_CH4_PIN                  GPIO_PIN_RH0
#define INT2_CH4_InterruptEnable()   (CNENHSET = (1U<<0))
#define INT2_CH4_InterruptDisable()  (CNENHCLR = (1U<<0))

/*** Macros for INT3_CH4 pin ***/
#define INT3_CH4_Set()               (LATHSET = (1U<<1))
#define INT3_CH4_Clear()             (LATHCLR = (1U<<1))
#define INT3_CH4_Toggle()            (LATHINV= (1U<<1))
#define INT3_CH4_OutputEnable()      (TRISHCLR = (1U<<1))
#define INT3_CH4_InputEnable()       (TRISHSET = (1U<<1))
#define INT3_CH4_Get()               ((PORTH >> 1) & 0x1U)
#define INT3_CH4_GetLatch()          ((LATH >> 1) & 0x1U)
#define INT3_CH4_PIN                  GPIO_PIN_RH1
#define INT3_CH4_InterruptEnable()   (CNENHSET = (1U<<1))
#define INT3_CH4_InterruptDisable()  (CNENHCLR = (1U<<1))

/*** Macros for INT4_CH4 pin ***/
#define INT4_CH4_Set()               (LATHSET = (1U<<2))
#define INT4_CH4_Clear()             (LATHCLR = (1U<<2))
#define INT4_CH4_Toggle()            (LATHINV= (1U<<2))
#define INT4_CH4_OutputEnable()      (TRISHCLR = (1U<<2))
#define INT4_CH4_InputEnable()       (TRISHSET = (1U<<2))
#define INT4_CH4_Get()               ((PORTH >> 2) & 0x1U)
#define INT4_CH4_GetLatch()          ((LATH >> 2) & 0x1U)
#define INT4_CH4_PIN                  GPIO_PIN_RH2
#define INT4_CH4_InterruptEnable()   (CNENHSET = (1U<<2))
#define INT4_CH4_InterruptDisable()  (CNENHCLR = (1U<<2))

/*** Macros for CS4_CH4 pin ***/
#define CS4_CH4_Set()               (LATHSET = (1U<<3))
#define CS4_CH4_Clear()             (LATHCLR = (1U<<3))
#define CS4_CH4_Toggle()            (LATHINV= (1U<<3))
#define CS4_CH4_OutputEnable()      (TRISHCLR = (1U<<3))
#define CS4_CH4_InputEnable()       (TRISHSET = (1U<<3))
#define CS4_CH4_Get()               ((PORTH >> 3) & 0x1U)
#define CS4_CH4_GetLatch()          ((LATH >> 3) & 0x1U)
#define CS4_CH4_PIN                  GPIO_PIN_RH3

/*** Macros for CS3_CH4 pin ***/
#define CS3_CH4_Set()               (LATBSET = (1U<<8))
#define CS3_CH4_Clear()             (LATBCLR = (1U<<8))
#define CS3_CH4_Toggle()            (LATBINV= (1U<<8))
#define CS3_CH4_OutputEnable()      (TRISBCLR = (1U<<8))
#define CS3_CH4_InputEnable()       (TRISBSET = (1U<<8))
#define CS3_CH4_Get()               ((PORTB >> 8) & 0x1U)
#define CS3_CH4_GetLatch()          ((LATB >> 8) & 0x1U)
#define CS3_CH4_PIN                  GPIO_PIN_RB8

/*** Macros for SDO_CH3 pin ***/
#define SDO_CH3_Get()               ((PORTB >> 9) & 0x1U)
#define SDO_CH3_GetLatch()          ((LATB >> 9) & 0x1U)
#define SDO_CH3_PIN                  GPIO_PIN_RB9

/*** Macros for SDI_CH3 pin ***/
#define SDI_CH3_Get()               ((PORTB >> 10) & 0x1U)
#define SDI_CH3_GetLatch()          ((LATB >> 10) & 0x1U)
#define SDI_CH3_PIN                  GPIO_PIN_RB10

/*** Macros for CS1_CH3 pin ***/
#define CS1_CH3_Set()               (LATBSET = (1U<<11))
#define CS1_CH3_Clear()             (LATBCLR = (1U<<11))
#define CS1_CH3_Toggle()            (LATBINV= (1U<<11))
#define CS1_CH3_OutputEnable()      (TRISBCLR = (1U<<11))
#define CS1_CH3_InputEnable()       (TRISBSET = (1U<<11))
#define CS1_CH3_Get()               ((PORTB >> 11) & 0x1U)
#define CS1_CH3_GetLatch()          ((LATB >> 11) & 0x1U)
#define CS1_CH3_PIN                  GPIO_PIN_RB11

/*** Macros for CS2_CH3 pin ***/
#define CS2_CH3_Set()               (LATKSET = (1U<<1))
#define CS2_CH3_Clear()             (LATKCLR = (1U<<1))
#define CS2_CH3_Toggle()            (LATKINV= (1U<<1))
#define CS2_CH3_OutputEnable()      (TRISKCLR = (1U<<1))
#define CS2_CH3_InputEnable()       (TRISKSET = (1U<<1))
#define CS2_CH3_Get()               ((PORTK >> 1) & 0x1U)
#define CS2_CH3_GetLatch()          ((LATK >> 1) & 0x1U)
#define CS2_CH3_PIN                  GPIO_PIN_RK1

/*** Macros for CS3_CH3 pin ***/
#define CS3_CH3_Set()               (LATKSET = (1U<<2))
#define CS3_CH3_Clear()             (LATKCLR = (1U<<2))
#define CS3_CH3_Toggle()            (LATKINV= (1U<<2))
#define CS3_CH3_OutputEnable()      (TRISKCLR = (1U<<2))
#define CS3_CH3_InputEnable()       (TRISKSET = (1U<<2))
#define CS3_CH3_Get()               ((PORTK >> 2) & 0x1U)
#define CS3_CH3_GetLatch()          ((LATK >> 2) & 0x1U)
#define CS3_CH3_PIN                  GPIO_PIN_RK2

/*** Macros for CS4_CH3 pin ***/
#define CS4_CH3_Set()               (LATKSET = (1U<<3))
#define CS4_CH3_Clear()             (LATKCLR = (1U<<3))
#define CS4_CH3_Toggle()            (LATKINV= (1U<<3))
#define CS4_CH3_OutputEnable()      (TRISKCLR = (1U<<3))
#define CS4_CH3_InputEnable()       (TRISKSET = (1U<<3))
#define CS4_CH3_Get()               ((PORTK >> 3) & 0x1U)
#define CS4_CH3_GetLatch()          ((LATK >> 3) & 0x1U)
#define CS4_CH3_PIN                  GPIO_PIN_RK3

/*** Macros for INT1_CH3 pin ***/
#define INT1_CH3_Set()               (LATASET = (1U<<1))
#define INT1_CH3_Clear()             (LATACLR = (1U<<1))
#define INT1_CH3_Toggle()            (LATAINV= (1U<<1))
#define INT1_CH3_OutputEnable()      (TRISACLR = (1U<<1))
#define INT1_CH3_InputEnable()       (TRISASET = (1U<<1))
#define INT1_CH3_Get()               ((PORTA >> 1) & 0x1U)
#define INT1_CH3_GetLatch()          ((LATA >> 1) & 0x1U)
#define INT1_CH3_PIN                  GPIO_PIN_RA1
#define INT1_CH3_InterruptEnable()   (CNENASET = (1U<<1))
#define INT1_CH3_InterruptDisable()  (CNENACLR = (1U<<1))

/*** Macros for SCK_CH4 pin ***/
#define SCK_CH4_Get()               ((PORTF >> 13) & 0x1U)
#define SCK_CH4_GetLatch()          ((LATF >> 13) & 0x1U)
#define SCK_CH4_PIN                  GPIO_PIN_RF13

/*** Macros for INT2_CH3 pin ***/
#define INT2_CH3_Set()               (LATFSET = (1U<<12))
#define INT2_CH3_Clear()             (LATFCLR = (1U<<12))
#define INT2_CH3_Toggle()            (LATFINV= (1U<<12))
#define INT2_CH3_OutputEnable()      (TRISFCLR = (1U<<12))
#define INT2_CH3_InputEnable()       (TRISFSET = (1U<<12))
#define INT2_CH3_Get()               ((PORTF >> 12) & 0x1U)
#define INT2_CH3_GetLatch()          ((LATF >> 12) & 0x1U)
#define INT2_CH3_PIN                  GPIO_PIN_RF12
#define INT2_CH3_InterruptEnable()   (CNENFSET = (1U<<12))
#define INT2_CH3_InterruptDisable()  (CNENFCLR = (1U<<12))

/*** Macros for INT3_CH3 pin ***/
#define INT3_CH3_Set()               (LATBSET = (1U<<12))
#define INT3_CH3_Clear()             (LATBCLR = (1U<<12))
#define INT3_CH3_Toggle()            (LATBINV= (1U<<12))
#define INT3_CH3_OutputEnable()      (TRISBCLR = (1U<<12))
#define INT3_CH3_InputEnable()       (TRISBSET = (1U<<12))
#define INT3_CH3_Get()               ((PORTB >> 12) & 0x1U)
#define INT3_CH3_GetLatch()          ((LATB >> 12) & 0x1U)
#define INT3_CH3_PIN                  GPIO_PIN_RB12
#define INT3_CH3_InterruptEnable()   (CNENBSET = (1U<<12))
#define INT3_CH3_InterruptDisable()  (CNENBCLR = (1U<<12))

/*** Macros for INT4_CH3 pin ***/
#define INT4_CH3_Set()               (LATBSET = (1U<<13))
#define INT4_CH3_Clear()             (LATBCLR = (1U<<13))
#define INT4_CH3_Toggle()            (LATBINV= (1U<<13))
#define INT4_CH3_OutputEnable()      (TRISBCLR = (1U<<13))
#define INT4_CH3_InputEnable()       (TRISBSET = (1U<<13))
#define INT4_CH3_Get()               ((PORTB >> 13) & 0x1U)
#define INT4_CH3_GetLatch()          ((LATB >> 13) & 0x1U)
#define INT4_CH3_PIN                  GPIO_PIN_RB13
#define INT4_CH3_InterruptEnable()   (CNENBSET = (1U<<13))
#define INT4_CH3_InterruptDisable()  (CNENBCLR = (1U<<13))

/*** Macros for SCK_CH3 pin ***/
#define SCK_CH3_Get()               ((PORTB >> 14) & 0x1U)
#define SCK_CH3_GetLatch()          ((LATB >> 14) & 0x1U)
#define SCK_CH3_PIN                  GPIO_PIN_RB14

/*** Macros for SDO_CH2 pin ***/
#define SDO_CH2_Get()               ((PORTB >> 15) & 0x1U)
#define SDO_CH2_GetLatch()          ((LATB >> 15) & 0x1U)
#define SDO_CH2_PIN                  GPIO_PIN_RB15

/*** Macros for CS4_CH2 pin ***/
#define CS4_CH2_Set()               (LATHSET = (1U<<4))
#define CS4_CH2_Clear()             (LATHCLR = (1U<<4))
#define CS4_CH2_Toggle()            (LATHINV= (1U<<4))
#define CS4_CH2_OutputEnable()      (TRISHCLR = (1U<<4))
#define CS4_CH2_InputEnable()       (TRISHSET = (1U<<4))
#define CS4_CH2_Get()               ((PORTH >> 4) & 0x1U)
#define CS4_CH2_GetLatch()          ((LATH >> 4) & 0x1U)
#define CS4_CH2_PIN                  GPIO_PIN_RH4

/*** Macros for ENABLE_CH2 pin ***/
#define ENABLE_CH2_Set()               (LATHSET = (1U<<5))
#define ENABLE_CH2_Clear()             (LATHCLR = (1U<<5))
#define ENABLE_CH2_Toggle()            (LATHINV= (1U<<5))
#define ENABLE_CH2_OutputEnable()      (TRISHCLR = (1U<<5))
#define ENABLE_CH2_InputEnable()       (TRISHSET = (1U<<5))
#define ENABLE_CH2_Get()               ((PORTH >> 5) & 0x1U)
#define ENABLE_CH2_GetLatch()          ((LATH >> 5) & 0x1U)
#define ENABLE_CH2_PIN                  GPIO_PIN_RH5

/*** Macros for CS3_CH2 pin ***/
#define CS3_CH2_Set()               (LATHSET = (1U<<6))
#define CS3_CH2_Clear()             (LATHCLR = (1U<<6))
#define CS3_CH2_Toggle()            (LATHINV= (1U<<6))
#define CS3_CH2_OutputEnable()      (TRISHCLR = (1U<<6))
#define CS3_CH2_InputEnable()       (TRISHSET = (1U<<6))
#define CS3_CH2_Get()               ((PORTH >> 6) & 0x1U)
#define CS3_CH2_GetLatch()          ((LATH >> 6) & 0x1U)
#define CS3_CH2_PIN                  GPIO_PIN_RH6

/*** Macros for CS2_CH2 pin ***/
#define CS2_CH2_Set()               (LATHSET = (1U<<7))
#define CS2_CH2_Clear()             (LATHCLR = (1U<<7))
#define CS2_CH2_Toggle()            (LATHINV= (1U<<7))
#define CS2_CH2_OutputEnable()      (TRISHCLR = (1U<<7))
#define CS2_CH2_InputEnable()       (TRISHSET = (1U<<7))
#define CS2_CH2_Get()               ((PORTH >> 7) & 0x1U)
#define CS2_CH2_GetLatch()          ((LATH >> 7) & 0x1U)
#define CS2_CH2_PIN                  GPIO_PIN_RH7

/*** Macros for CS1_CH2 pin ***/
#define CS1_CH2_Set()               (LATDSET = (1U<<14))
#define CS1_CH2_Clear()             (LATDCLR = (1U<<14))
#define CS1_CH2_Toggle()            (LATDINV= (1U<<14))
#define CS1_CH2_OutputEnable()      (TRISDCLR = (1U<<14))
#define CS1_CH2_InputEnable()       (TRISDSET = (1U<<14))
#define CS1_CH2_Get()               ((PORTD >> 14) & 0x1U)
#define CS1_CH2_GetLatch()          ((LATD >> 14) & 0x1U)
#define CS1_CH2_PIN                  GPIO_PIN_RD14

/*** Macros for SCK_CH2 pin ***/
#define SCK_CH2_Get()               ((PORTD >> 15) & 0x1U)
#define SCK_CH2_GetLatch()          ((LATD >> 15) & 0x1U)
#define SCK_CH2_PIN                  GPIO_PIN_RD15

/*** Macros for ENABLE_CH1 pin ***/
#define ENABLE_CH1_Set()               (LATCSET = (1U<<15))
#define ENABLE_CH1_Clear()             (LATCCLR = (1U<<15))
#define ENABLE_CH1_Toggle()            (LATCINV= (1U<<15))
#define ENABLE_CH1_OutputEnable()      (TRISCCLR = (1U<<15))
#define ENABLE_CH1_InputEnable()       (TRISCSET = (1U<<15))
#define ENABLE_CH1_Get()               ((PORTC >> 15) & 0x1U)
#define ENABLE_CH1_GetLatch()          ((LATC >> 15) & 0x1U)
#define ENABLE_CH1_PIN                  GPIO_PIN_RC15

/*** Macros for SDA_CH1 pin ***/
#define SDA_CH1_Get()               ((PORTF >> 2) & 0x1U)
#define SDA_CH1_GetLatch()          ((LATF >> 2) & 0x1U)
#define SDA_CH1_PIN                  GPIO_PIN_RF2

/*** Macros for SCL_CH1 pin ***/
#define SCL_CH1_Get()               ((PORTF >> 8) & 0x1U)
#define SCL_CH1_GetLatch()          ((LATF >> 8) & 0x1U)
#define SCL_CH1_PIN                  GPIO_PIN_RF8

/*** Macros for INT4_CH2 pin ***/
#define INT4_CH2_Set()               (LATHSET = (1U<<8))
#define INT4_CH2_Clear()             (LATHCLR = (1U<<8))
#define INT4_CH2_Toggle()            (LATHINV= (1U<<8))
#define INT4_CH2_OutputEnable()      (TRISHCLR = (1U<<8))
#define INT4_CH2_InputEnable()       (TRISHSET = (1U<<8))
#define INT4_CH2_Get()               ((PORTH >> 8) & 0x1U)
#define INT4_CH2_GetLatch()          ((LATH >> 8) & 0x1U)
#define INT4_CH2_PIN                  GPIO_PIN_RH8
#define INT4_CH2_InterruptEnable()   (CNENHSET = (1U<<8))
#define INT4_CH2_InterruptDisable()  (CNENHCLR = (1U<<8))

/*** Macros for INT3_CH2 pin ***/
#define INT3_CH2_Set()               (LATHSET = (1U<<9))
#define INT3_CH2_Clear()             (LATHCLR = (1U<<9))
#define INT3_CH2_Toggle()            (LATHINV= (1U<<9))
#define INT3_CH2_OutputEnable()      (TRISHCLR = (1U<<9))
#define INT3_CH2_InputEnable()       (TRISHSET = (1U<<9))
#define INT3_CH2_Get()               ((PORTH >> 9) & 0x1U)
#define INT3_CH2_GetLatch()          ((LATH >> 9) & 0x1U)
#define INT3_CH2_PIN                  GPIO_PIN_RH9
#define INT3_CH2_InterruptEnable()   (CNENHSET = (1U<<9))
#define INT3_CH2_InterruptDisable()  (CNENHCLR = (1U<<9))

/*** Macros for INT2_CH2 pin ***/
#define INT2_CH2_Set()               (LATHSET = (1U<<10))
#define INT2_CH2_Clear()             (LATHCLR = (1U<<10))
#define INT2_CH2_Toggle()            (LATHINV= (1U<<10))
#define INT2_CH2_OutputEnable()      (TRISHCLR = (1U<<10))
#define INT2_CH2_InputEnable()       (TRISHSET = (1U<<10))
#define INT2_CH2_Get()               ((PORTH >> 10) & 0x1U)
#define INT2_CH2_GetLatch()          ((LATH >> 10) & 0x1U)
#define INT2_CH2_PIN                  GPIO_PIN_RH10
#define INT2_CH2_InterruptEnable()   (CNENHSET = (1U<<10))
#define INT2_CH2_InterruptDisable()  (CNENHCLR = (1U<<10))

/*** Macros for INT1_CH2 pin ***/
#define INT1_CH2_Set()               (LATHSET = (1U<<11))
#define INT1_CH2_Clear()             (LATHCLR = (1U<<11))
#define INT1_CH2_Toggle()            (LATHINV= (1U<<11))
#define INT1_CH2_OutputEnable()      (TRISHCLR = (1U<<11))
#define INT1_CH2_InputEnable()       (TRISHSET = (1U<<11))
#define INT1_CH2_Get()               ((PORTH >> 11) & 0x1U)
#define INT1_CH2_GetLatch()          ((LATH >> 11) & 0x1U)
#define INT1_CH2_PIN                  GPIO_PIN_RH11
#define INT1_CH2_InterruptEnable()   (CNENHSET = (1U<<11))
#define INT1_CH2_InterruptDisable()  (CNENHCLR = (1U<<11))

/*** Macros for SCL_CH2 pin ***/
#define SCL_CH2_Get()               ((PORTA >> 2) & 0x1U)
#define SCL_CH2_GetLatch()          ((LATA >> 2) & 0x1U)
#define SCL_CH2_PIN                  GPIO_PIN_RA2

/*** Macros for SDA_CH2 pin ***/
#define SDA_CH2_Get()               ((PORTA >> 3) & 0x1U)
#define SDA_CH2_GetLatch()          ((LATA >> 3) & 0x1U)
#define SDA_CH2_PIN                  GPIO_PIN_RA3

/*** Macros for CS1_CH1 pin ***/
#define CS1_CH1_Set()               (LATASET = (1U<<4))
#define CS1_CH1_Clear()             (LATACLR = (1U<<4))
#define CS1_CH1_Toggle()            (LATAINV= (1U<<4))
#define CS1_CH1_OutputEnable()      (TRISACLR = (1U<<4))
#define CS1_CH1_InputEnable()       (TRISASET = (1U<<4))
#define CS1_CH1_Get()               ((PORTA >> 4) & 0x1U)
#define CS1_CH1_GetLatch()          ((LATA >> 4) & 0x1U)
#define CS1_CH1_PIN                  GPIO_PIN_RA4

/*** Macros for SDA_CH3 pin ***/
#define SDA_CH3_Get()               ((PORTF >> 4) & 0x1U)
#define SDA_CH3_GetLatch()          ((LATF >> 4) & 0x1U)
#define SDA_CH3_PIN                  GPIO_PIN_RF4

/*** Macros for SCL_CH3 pin ***/
#define SCL_CH3_Get()               ((PORTF >> 5) & 0x1U)
#define SCL_CH3_GetLatch()          ((LATF >> 5) & 0x1U)
#define SCL_CH3_PIN                  GPIO_PIN_RF5

/*** Macros for CS2_CH1 pin ***/
#define CS2_CH1_Set()               (LATKSET = (1U<<4))
#define CS2_CH1_Clear()             (LATKCLR = (1U<<4))
#define CS2_CH1_Toggle()            (LATKINV= (1U<<4))
#define CS2_CH1_OutputEnable()      (TRISKCLR = (1U<<4))
#define CS2_CH1_InputEnable()       (TRISKSET = (1U<<4))
#define CS2_CH1_Get()               ((PORTK >> 4) & 0x1U)
#define CS2_CH1_GetLatch()          ((LATK >> 4) & 0x1U)
#define CS2_CH1_PIN                  GPIO_PIN_RK4

/*** Macros for CS3_CH1 pin ***/
#define CS3_CH1_Set()               (LATKSET = (1U<<5))
#define CS3_CH1_Clear()             (LATKCLR = (1U<<5))
#define CS3_CH1_Toggle()            (LATKINV= (1U<<5))
#define CS3_CH1_OutputEnable()      (TRISKCLR = (1U<<5))
#define CS3_CH1_InputEnable()       (TRISKSET = (1U<<5))
#define CS3_CH1_Get()               ((PORTK >> 5) & 0x1U)
#define CS3_CH1_GetLatch()          ((LATK >> 5) & 0x1U)
#define CS3_CH1_PIN                  GPIO_PIN_RK5

/*** Macros for CS4_CH1 pin ***/
#define CS4_CH1_Set()               (LATKSET = (1U<<6))
#define CS4_CH1_Clear()             (LATKCLR = (1U<<6))
#define CS4_CH1_Toggle()            (LATKINV= (1U<<6))
#define CS4_CH1_OutputEnable()      (TRISKCLR = (1U<<6))
#define CS4_CH1_InputEnable()       (TRISKSET = (1U<<6))
#define CS4_CH1_Get()               ((PORTK >> 6) & 0x1U)
#define CS4_CH1_GetLatch()          ((LATK >> 6) & 0x1U)
#define CS4_CH1_PIN                  GPIO_PIN_RK6

/*** Macros for SCL_CH4 pin ***/
#define SCL_CH4_Get()               ((PORTA >> 14) & 0x1U)
#define SCL_CH4_GetLatch()          ((LATA >> 14) & 0x1U)
#define SCL_CH4_PIN                  GPIO_PIN_RA14

/*** Macros for SDA_CH4 pin ***/
#define SDA_CH4_Get()               ((PORTA >> 15) & 0x1U)
#define SDA_CH4_GetLatch()          ((LATA >> 15) & 0x1U)
#define SDA_CH4_PIN                  GPIO_PIN_RA15

/*** Macros for INT1_CH1 pin ***/
#define INT1_CH1_Set()               (LATDSET = (1U<<9))
#define INT1_CH1_Clear()             (LATDCLR = (1U<<9))
#define INT1_CH1_Toggle()            (LATDINV= (1U<<9))
#define INT1_CH1_OutputEnable()      (TRISDCLR = (1U<<9))
#define INT1_CH1_InputEnable()       (TRISDSET = (1U<<9))
#define INT1_CH1_Get()               ((PORTD >> 9) & 0x1U)
#define INT1_CH1_GetLatch()          ((LATD >> 9) & 0x1U)
#define INT1_CH1_PIN                  GPIO_PIN_RD9
#define INT1_CH1_InterruptEnable()   (CNENDSET = (1U<<9))
#define INT1_CH1_InterruptDisable()  (CNENDCLR = (1U<<9))

/*** Macros for SCK_CH1 pin ***/
#define SCK_CH1_Get()               ((PORTD >> 10) & 0x1U)
#define SCK_CH1_GetLatch()          ((LATD >> 10) & 0x1U)
#define SCK_CH1_PIN                  GPIO_PIN_RD10

/*** Macros for SDI_CH1 pin ***/
#define SDI_CH1_Get()               ((PORTD >> 11) & 0x1U)
#define SDI_CH1_GetLatch()          ((LATD >> 11) & 0x1U)
#define SDI_CH1_PIN                  GPIO_PIN_RD11

/*** Macros for INT2_CH1 pin ***/
#define INT2_CH1_Set()               (LATHSET = (1U<<12))
#define INT2_CH1_Clear()             (LATHCLR = (1U<<12))
#define INT2_CH1_Toggle()            (LATHINV= (1U<<12))
#define INT2_CH1_OutputEnable()      (TRISHCLR = (1U<<12))
#define INT2_CH1_InputEnable()       (TRISHSET = (1U<<12))
#define INT2_CH1_Get()               ((PORTH >> 12) & 0x1U)
#define INT2_CH1_GetLatch()          ((LATH >> 12) & 0x1U)
#define INT2_CH1_PIN                  GPIO_PIN_RH12
#define INT2_CH1_InterruptEnable()   (CNENHSET = (1U<<12))
#define INT2_CH1_InterruptDisable()  (CNENHCLR = (1U<<12))

/*** Macros for INT4_CH1 pin ***/
#define INT4_CH1_Set()               (LATHSET = (1U<<13))
#define INT4_CH1_Clear()             (LATHCLR = (1U<<13))
#define INT4_CH1_Toggle()            (LATHINV= (1U<<13))
#define INT4_CH1_OutputEnable()      (TRISHCLR = (1U<<13))
#define INT4_CH1_InputEnable()       (TRISHSET = (1U<<13))
#define INT4_CH1_Get()               ((PORTH >> 13) & 0x1U)
#define INT4_CH1_GetLatch()          ((LATH >> 13) & 0x1U)
#define INT4_CH1_PIN                  GPIO_PIN_RH13
#define INT4_CH1_InterruptEnable()   (CNENHSET = (1U<<13))
#define INT4_CH1_InterruptDisable()  (CNENHCLR = (1U<<13))

/*** Macros for INT3_CH1 pin ***/
#define INT3_CH1_Set()               (LATHSET = (1U<<14))
#define INT3_CH1_Clear()             (LATHCLR = (1U<<14))
#define INT3_CH1_Toggle()            (LATHINV= (1U<<14))
#define INT3_CH1_OutputEnable()      (TRISHCLR = (1U<<14))
#define INT3_CH1_InputEnable()       (TRISHSET = (1U<<14))
#define INT3_CH1_Get()               ((PORTH >> 14) & 0x1U)
#define INT3_CH1_GetLatch()          ((LATH >> 14) & 0x1U)
#define INT3_CH1_PIN                  GPIO_PIN_RH14
#define INT3_CH1_InterruptEnable()   (CNENHSET = (1U<<14))
#define INT3_CH1_InterruptDisable()  (CNENHCLR = (1U<<14))

/*** Macros for SDO_CH1 pin ***/
#define SDO_CH1_Get()               ((PORTD >> 0) & 0x1U)
#define SDO_CH1_GetLatch()          ((LATD >> 0) & 0x1U)
#define SDO_CH1_PIN                  GPIO_PIN_RD0

/*** Macros for SDA_EEPROM pin ***/
#define SDA_EEPROM_Set()               (LATCSET = (1U<<13))
#define SDA_EEPROM_Clear()             (LATCCLR = (1U<<13))
#define SDA_EEPROM_Toggle()            (LATCINV= (1U<<13))
#define SDA_EEPROM_OutputEnable()      (TRISCCLR = (1U<<13))
#define SDA_EEPROM_InputEnable()       (TRISCSET = (1U<<13))
#define SDA_EEPROM_Get()               ((PORTC >> 13) & 0x1U)
#define SDA_EEPROM_GetLatch()          ((LATC >> 13) & 0x1U)
#define SDA_EEPROM_PIN                  GPIO_PIN_RC13

/*** Macros for SCL_EEPROM pin ***/
#define SCL_EEPROM_Set()               (LATCSET = (1U<<14))
#define SCL_EEPROM_Clear()             (LATCCLR = (1U<<14))
#define SCL_EEPROM_Toggle()            (LATCINV= (1U<<14))
#define SCL_EEPROM_OutputEnable()      (TRISCCLR = (1U<<14))
#define SCL_EEPROM_InputEnable()       (TRISCSET = (1U<<14))
#define SCL_EEPROM_Get()               ((PORTC >> 14) & 0x1U)
#define SCL_EEPROM_GetLatch()          ((LATC >> 14) & 0x1U)
#define SCL_EEPROM_PIN                  GPIO_PIN_RC14

/*** Macros for RGB_SCK pin ***/
#define RGB_SCK_Get()               ((PORTD >> 1) & 0x1U)
#define RGB_SCK_GetLatch()          ((LATD >> 1) & 0x1U)
#define RGB_SCK_PIN                  GPIO_PIN_RD1

/*** Macros for RGB_SDI pin ***/
#define RGB_SDI_Get()               ((PORTD >> 2) & 0x1U)
#define RGB_SDI_GetLatch()          ((LATD >> 2) & 0x1U)
#define RGB_SDI_PIN                  GPIO_PIN_RD2

/*** Macros for RGB pin ***/
#define RGB_Get()               ((PORTD >> 3) & 0x1U)
#define RGB_GetLatch()          ((LATD >> 3) & 0x1U)
#define RGB_PIN                  GPIO_PIN_RD3

/*** Macros for ENABLE pin ***/
#define ENABLE_Set()               (LATDSET = (1U<<13))
#define ENABLE_Clear()             (LATDCLR = (1U<<13))
#define ENABLE_Toggle()            (LATDINV= (1U<<13))
#define ENABLE_OutputEnable()      (TRISDCLR = (1U<<13))
#define ENABLE_InputEnable()       (TRISDSET = (1U<<13))
#define ENABLE_Get()               ((PORTD >> 13) & 0x1U)
#define ENABLE_GetLatch()          ((LATD >> 13) & 0x1U)
#define ENABLE_PIN                  GPIO_PIN_RD13

/*** Macros for VBUS_DETECT pin ***/
#define VBUS_DETECT_Set()               (LATJSET = (1U<<3))
#define VBUS_DETECT_Clear()             (LATJCLR = (1U<<3))
#define VBUS_DETECT_Toggle()            (LATJINV= (1U<<3))
#define VBUS_DETECT_OutputEnable()      (TRISJCLR = (1U<<3))
#define VBUS_DETECT_InputEnable()       (TRISJSET = (1U<<3))
#define VBUS_DETECT_Get()               ((PORTJ >> 3) & 0x1U)
#define VBUS_DETECT_GetLatch()          ((LATJ >> 3) & 0x1U)
#define VBUS_DETECT_PIN                  GPIO_PIN_RJ3

/*** Macros for RTS pin ***/
#define RTS_Get()               ((PORTD >> 4) & 0x1U)
#define RTS_GetLatch()          ((LATD >> 4) & 0x1U)
#define RTS_PIN                  GPIO_PIN_RD4

/*** Macros for CTS pin ***/
#define CTS_Get()               ((PORTD >> 5) & 0x1U)
#define CTS_GetLatch()          ((LATD >> 5) & 0x1U)
#define CTS_PIN                  GPIO_PIN_RD5

/*** Macros for GPIO_1 pin ***/
#define GPIO_1_Set()               (LATDSET = (1U<<7))
#define GPIO_1_Clear()             (LATDCLR = (1U<<7))
#define GPIO_1_Toggle()            (LATDINV= (1U<<7))
#define GPIO_1_OutputEnable()      (TRISDCLR = (1U<<7))
#define GPIO_1_InputEnable()       (TRISDSET = (1U<<7))
#define GPIO_1_Get()               ((PORTD >> 7) & 0x1U)
#define GPIO_1_GetLatch()          ((LATD >> 7) & 0x1U)
#define GPIO_1_PIN                  GPIO_PIN_RD7

/*** Macros for DEBUG pin ***/
#define DEBUG_Get()               ((PORTF >> 1) & 0x1U)
#define DEBUG_GetLatch()          ((LATF >> 1) & 0x1U)
#define DEBUG_PIN                  GPIO_PIN_RF1

/*** Macros for GPIO_2 pin ***/
#define GPIO_2_Set()               (LATKSET = (1U<<7))
#define GPIO_2_Clear()             (LATKCLR = (1U<<7))
#define GPIO_2_Toggle()            (LATKINV= (1U<<7))
#define GPIO_2_OutputEnable()      (TRISKCLR = (1U<<7))
#define GPIO_2_InputEnable()       (TRISKSET = (1U<<7))
#define GPIO_2_Get()               ((PORTK >> 7) & 0x1U)
#define GPIO_2_GetLatch()          ((LATK >> 7) & 0x1U)
#define GPIO_2_PIN                  GPIO_PIN_RK7

/*** Macros for RX pin ***/
#define RX_Get()               ((PORTG >> 1) & 0x1U)
#define RX_GetLatch()          ((LATG >> 1) & 0x1U)
#define RX_PIN                  GPIO_PIN_RG1

/*** Macros for TX pin ***/
#define TX_Get()               ((PORTG >> 0) & 0x1U)
#define TX_GetLatch()          ((LATG >> 0) & 0x1U)
#define TX_PIN                  GPIO_PIN_RG0


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/


#define    GPIO_PORT_A  (0)
#define    GPIO_PORT_B  (1)
#define    GPIO_PORT_C  (2)
#define    GPIO_PORT_D  (3)
#define    GPIO_PORT_E  (4)
#define    GPIO_PORT_F  (5)
#define    GPIO_PORT_G  (6)
#define    GPIO_PORT_H  (7)
#define    GPIO_PORT_J  (8)
#define    GPIO_PORT_K  (9)
typedef uint32_t GPIO_PORT;

typedef enum
{
    GPIO_INTERRUPT_ON_MISMATCH,
    GPIO_INTERRUPT_ON_RISING_EDGE,
    GPIO_INTERRUPT_ON_FALLING_EDGE,
    GPIO_INTERRUPT_ON_BOTH_EDGES,
}GPIO_INTERRUPT_STYLE;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/


#define     GPIO_PIN_RA0  (0U)
#define     GPIO_PIN_RA1  (1U)
#define     GPIO_PIN_RA2  (2U)
#define     GPIO_PIN_RA3  (3U)
#define     GPIO_PIN_RA4  (4U)
#define     GPIO_PIN_RA5  (5U)
#define     GPIO_PIN_RA6  (6U)
#define     GPIO_PIN_RA7  (7U)
#define     GPIO_PIN_RA9  (9U)
#define     GPIO_PIN_RA10  (10U)
#define     GPIO_PIN_RA14  (14U)
#define     GPIO_PIN_RA15  (15U)
#define     GPIO_PIN_RB0  (16U)
#define     GPIO_PIN_RB1  (17U)
#define     GPIO_PIN_RB2  (18U)
#define     GPIO_PIN_RB3  (19U)
#define     GPIO_PIN_RB4  (20U)
#define     GPIO_PIN_RB5  (21U)
#define     GPIO_PIN_RB6  (22U)
#define     GPIO_PIN_RB7  (23U)
#define     GPIO_PIN_RB8  (24U)
#define     GPIO_PIN_RB9  (25U)
#define     GPIO_PIN_RB10  (26U)
#define     GPIO_PIN_RB11  (27U)
#define     GPIO_PIN_RB12  (28U)
#define     GPIO_PIN_RB13  (29U)
#define     GPIO_PIN_RB14  (30U)
#define     GPIO_PIN_RB15  (31U)
#define     GPIO_PIN_RC1  (33U)
#define     GPIO_PIN_RC2  (34U)
#define     GPIO_PIN_RC3  (35U)
#define     GPIO_PIN_RC4  (36U)
#define     GPIO_PIN_RC12  (44U)
#define     GPIO_PIN_RC13  (45U)
#define     GPIO_PIN_RC14  (46U)
#define     GPIO_PIN_RC15  (47U)
#define     GPIO_PIN_RD0  (48U)
#define     GPIO_PIN_RD1  (49U)
#define     GPIO_PIN_RD2  (50U)
#define     GPIO_PIN_RD3  (51U)
#define     GPIO_PIN_RD4  (52U)
#define     GPIO_PIN_RD5  (53U)
#define     GPIO_PIN_RD6  (54U)
#define     GPIO_PIN_RD7  (55U)
#define     GPIO_PIN_RD9  (57U)
#define     GPIO_PIN_RD10  (58U)
#define     GPIO_PIN_RD11  (59U)
#define     GPIO_PIN_RD12  (60U)
#define     GPIO_PIN_RD13  (61U)
#define     GPIO_PIN_RD14  (62U)
#define     GPIO_PIN_RD15  (63U)
#define     GPIO_PIN_RE0  (64U)
#define     GPIO_PIN_RE1  (65U)
#define     GPIO_PIN_RE2  (66U)
#define     GPIO_PIN_RE3  (67U)
#define     GPIO_PIN_RE4  (68U)
#define     GPIO_PIN_RE5  (69U)
#define     GPIO_PIN_RE6  (70U)
#define     GPIO_PIN_RE7  (71U)
#define     GPIO_PIN_RE8  (72U)
#define     GPIO_PIN_RE9  (73U)
#define     GPIO_PIN_RF0  (80U)
#define     GPIO_PIN_RF1  (81U)
#define     GPIO_PIN_RF2  (82U)
#define     GPIO_PIN_RF3  (83U)
#define     GPIO_PIN_RF4  (84U)
#define     GPIO_PIN_RF5  (85U)
#define     GPIO_PIN_RF8  (88U)
#define     GPIO_PIN_RF12  (92U)
#define     GPIO_PIN_RF13  (93U)
#define     GPIO_PIN_RG0  (96U)
#define     GPIO_PIN_RG1  (97U)
#define     GPIO_PIN_RG6  (102U)
#define     GPIO_PIN_RG7  (103U)
#define     GPIO_PIN_RG8  (104U)
#define     GPIO_PIN_RG9  (105U)
#define     GPIO_PIN_RG12  (108U)
#define     GPIO_PIN_RG13  (109U)
#define     GPIO_PIN_RG14  (110U)
#define     GPIO_PIN_RG15  (111U)
#define     GPIO_PIN_RH0  (112U)
#define     GPIO_PIN_RH1  (113U)
#define     GPIO_PIN_RH2  (114U)
#define     GPIO_PIN_RH3  (115U)
#define     GPIO_PIN_RH4  (116U)
#define     GPIO_PIN_RH5  (117U)
#define     GPIO_PIN_RH6  (118U)
#define     GPIO_PIN_RH7  (119U)
#define     GPIO_PIN_RH8  (120U)
#define     GPIO_PIN_RH9  (121U)
#define     GPIO_PIN_RH10  (122U)
#define     GPIO_PIN_RH11  (123U)
#define     GPIO_PIN_RH12  (124U)
#define     GPIO_PIN_RH13  (125U)
#define     GPIO_PIN_RH14  (126U)
#define     GPIO_PIN_RH15  (127U)
#define     GPIO_PIN_RJ0  (128U)
#define     GPIO_PIN_RJ1  (129U)
#define     GPIO_PIN_RJ2  (130U)
#define     GPIO_PIN_RJ3  (131U)
#define     GPIO_PIN_RJ4  (132U)
#define     GPIO_PIN_RJ5  (133U)
#define     GPIO_PIN_RJ6  (134U)
#define     GPIO_PIN_RJ7  (135U)
#define     GPIO_PIN_RJ8  (136U)
#define     GPIO_PIN_RJ9  (137U)
#define     GPIO_PIN_RJ10  (138U)
#define     GPIO_PIN_RJ11  (139U)
#define     GPIO_PIN_RJ12  (140U)
#define     GPIO_PIN_RJ13  (141U)
#define     GPIO_PIN_RJ14  (142U)
#define     GPIO_PIN_RJ15  (143U)
#define     GPIO_PIN_RK0  (144U)
#define     GPIO_PIN_RK1  (145U)
#define     GPIO_PIN_RK2  (146U)
#define     GPIO_PIN_RK3  (147U)
#define     GPIO_PIN_RK4  (148U)
#define     GPIO_PIN_RK5  (149U)
#define     GPIO_PIN_RK6  (150U)
#define     GPIO_PIN_RK7  (151U)

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
#define    GPIO_PIN_NONE   (-1)

typedef uint32_t GPIO_PIN;

typedef  void (*GPIO_PIN_CALLBACK) ( GPIO_PIN pin, uintptr_t context);

void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    GPIO_PIN                 pin;

    /* Callback for event on target pin*/
    GPIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} GPIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
     uint32_t xvalue = (uint32_t)value;
    GPIO_PortWrite((pin>>4U), (uint32_t)(0x1U) << (pin & 0xFU), (xvalue) << (pin & 0xFU));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return ((((GPIO_PortRead((GPIO_PORT)(pin>>4U))) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (((GPIO_PortLatchRead((GPIO_PORT)(pin>>4U)) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

#define GPIO_PinInterruptEnable(pin)       GPIO_PinIntEnable(pin, GPIO_INTERRUPT_ON_MISMATCH)
#define GPIO_PinInterruptDisable(pin)      GPIO_PinIntDisable(pin)

void GPIO_PinIntEnable(GPIO_PIN pin, GPIO_INTERRUPT_STYLE style);
void GPIO_PinIntDisable(GPIO_PIN pin);

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const   GPIO_PIN_CALLBACK callback,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
