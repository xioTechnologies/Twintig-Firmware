/*******************************************************************************
  SYS PORTS Static Functions for PORTS System Service

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.c

  Summary:
    GPIO function implementations for the GPIO PLIB.

  Description:
    The GPIO PLIB provides a simple interface to manage peripheral
    input-output controller.

*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#include "plib_gpio.h"
#include "interrupts.h"



/* Array to store callback objects of each configured interrupt */
static volatile GPIO_PIN_CALLBACK_OBJ portPinCbObj[20];

/* Array to store number of interrupts in each PORT Channel + previous interrupt count */
static uint8_t portNumCb[10 + 1] = { 0, 2, 5, 5, 6, 6, 7, 7, 17, 20, 20, };

/******************************************************************************
  Function:
    GPIO_Initialize ( void )

  Summary:
    Initialize the GPIO library.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_Initialize ( void )
{

    /* PORTA Initialization */
    ODCASET = 0xc00cU; /* Open Drain Enable */
    LATA = 0xc01dU; /* Initial Latch Value */
    TRISACLR = 0xc41dU; /* Direction Control */
    ANSELACLR = 0x603U; /* Digital Mode Enable */

    /* Change Notice Enable */
    CNCONASET = _CNCONA_ON_MASK;
    PORTA;
    IEC3SET = _IEC3_CNAIE_MASK;
    /* PORTB Initialization */
    LATB = 0x904U; /* Initial Latch Value */
    TRISBCLR = 0x984U; /* Direction Control */
    ANSELBCLR = 0xfffcU; /* Digital Mode Enable */

    /* Change Notice Enable */
    CNCONBSET = _CNCONB_ON_MASK;
    PORTB;
    IEC3SET = _IEC3_CNBIE_MASK;
    /* PORTC Initialization */
    ODCCSET = 0x600cU; /* Open Drain Enable */
    LATC = 0x600cU; /* Initial Latch Value */
    TRISCCLR = 0xe00cU; /* Direction Control */
    ANSELCCLR = 0x1eU; /* Digital Mode Enable */
    /* PORTD Initialization */
    LATD = 0x4000U; /* Initial Latch Value */
    TRISDCLR = 0x6080U; /* Direction Control */
    ANSELDCLR = 0xc000U; /* Digital Mode Enable */
    CNPDDSET = 0x4U; /* Pull-Down Enable */

    /* Change Notice Enable */
    CNCONDSET = _CNCOND_ON_MASK;
    PORTD;
    IEC3SET = _IEC3_CNDIE_MASK;
    /* PORTE Initialization */
    LATE = 0x300U; /* Initial Latch Value */
    TRISECLR = 0x300U; /* Direction Control */
    ANSELECLR = 0x300U; /* Digital Mode Enable */
    /* PORTF Initialization */
    ODCFSET = 0x134U; /* Open Drain Enable */
    LATF = 0x134U; /* Initial Latch Value */
    TRISFCLR = 0x134U; /* Direction Control */
    ANSELFCLR = 0x3000U; /* Digital Mode Enable */

    /* Change Notice Enable */
    CNCONFSET = _CNCONF_ON_MASK;
    PORTF;
    IEC3SET = _IEC3_CNFIE_MASK;
    /* PORTG Initialization */
    ODCGSET = 0x180U; /* Open Drain Enable */
    LATG = 0x180U; /* Initial Latch Value */
    TRISGCLR = 0x380U; /* Direction Control */
    ANSELGCLR = 0x3c0U; /* Digital Mode Enable */
    /* PORTH Initialization */
    LATH = 0xd8U; /* Initial Latch Value */
    TRISHCLR = 0xf8U; /* Direction Control */
    ANSELHCLR = 0x73U; /* Digital Mode Enable */

    /* Change Notice Enable */
    CNCONHSET = _CNCONH_ON_MASK;
    PORTH;
    IEC3SET = _IEC3_CNHIE_MASK;
    /* PORTJ Initialization */
    LATJ = 0x8000U; /* Initial Latch Value */
    TRISJCLR = 0x8000U; /* Direction Control */
    ANSELJCLR = 0x800U; /* Digital Mode Enable */

    /* Change Notice Enable */
    CNCONJSET = _CNCONJ_ON_MASK;
    PORTJ;
    IEC3SET = _IEC3_CNJIE_MASK;
    /* PORTK Initialization */
    LATK = 0x7fU; /* Initial Latch Value */
    TRISKCLR = 0xffU; /* Direction Control */

    /* Unlock system for PPS configuration */
    SYSKEY = 0x00000000U;
    SYSKEY = 0xAA996655U;
    SYSKEY = 0x556699AAU;

    CFGCONbits.IOLOCK = 0U;

    /* PPS Input Remapping */
    SDI2R = 10;
    SDI5R = 8;
    SDI6R = 5;
    SDI3R = 6;
    SDI4R = 3;
    SDI1R = 0;
    U1CTSR = 4;
    U1RXR = 12;

    /* PPS Output Remapping */
    RPC1R = 6;
    RPB3R = 9;
    RPB9R = 7;
    RPB15R = 10;
    RPD0R = 8;
    RPD3R = 5;
    RPD5R = 1;
    RPF1R = 1;
    RPG0R = 1;

        /* Lock back the system after PPS configuration */
    CFGCONbits.IOLOCK = 1U;

    SYSKEY = 0x00000000U;

    uint32_t i;
    /* Initialize Interrupt Pin data structures */
    portPinCbObj[2 + 0].pin = GPIO_PIN_RB4;
    
    portPinCbObj[17 + 0].pin = GPIO_PIN_RJ11;
    
    portPinCbObj[17 + 1].pin = GPIO_PIN_RJ13;
    
    portPinCbObj[17 + 2].pin = GPIO_PIN_RJ14;
    
    portPinCbObj[0 + 0].pin = GPIO_PIN_RA9;
    
    portPinCbObj[7 + 0].pin = GPIO_PIN_RH0;
    
    portPinCbObj[7 + 1].pin = GPIO_PIN_RH1;
    
    portPinCbObj[7 + 2].pin = GPIO_PIN_RH2;
    
    portPinCbObj[0 + 1].pin = GPIO_PIN_RA1;
    
    portPinCbObj[6 + 0].pin = GPIO_PIN_RF12;
    
    portPinCbObj[2 + 1].pin = GPIO_PIN_RB12;
    
    portPinCbObj[2 + 2].pin = GPIO_PIN_RB13;
    
    portPinCbObj[7 + 3].pin = GPIO_PIN_RH8;
    
    portPinCbObj[7 + 4].pin = GPIO_PIN_RH9;
    
    portPinCbObj[7 + 5].pin = GPIO_PIN_RH10;
    
    portPinCbObj[7 + 6].pin = GPIO_PIN_RH11;
    
    portPinCbObj[5 + 0].pin = GPIO_PIN_RD9;
    
    portPinCbObj[7 + 7].pin = GPIO_PIN_RH12;
    
    portPinCbObj[7 + 8].pin = GPIO_PIN_RH13;
    
    portPinCbObj[7 + 9].pin = GPIO_PIN_RH14;
    
    for(i=0U; i<20U; i++)
    {
        portPinCbObj[i].callback = NULL;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: GPIO APIs which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t GPIO_PortRead ( GPIO_PORT port )

  Summary:
    Read all the I/O lines of the selected port.

  Description:
    This function reads the live data values on all the I/O lines of the
    selected port.  Bit values returned in each position indicate corresponding
    pin levels.
    1 = Pin is high.
    0 = Pin is low.

    This function reads the value regardless of pin configuration, whether it is
    set as as an input, driven by the GPIO Controller, or driven by a peripheral.

  Remarks:
    If the port has less than 32-bits, unimplemented pins will read as
    low (0).
    Implemented pins are Right aligned in the 32-bit return value.
*/
uint32_t GPIO_PortRead(GPIO_PORT port)
{
    return (*(volatile uint32_t *)(&PORTA + (port * 0x40U)));
}

// *****************************************************************************
/* Function:
    void GPIO_PortWrite (GPIO_PORT port, uint32_t mask, uint32_t value);

  Summary:
    Write the value on the masked I/O lines of the selected port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value)
{
    *(volatile uint32_t *)(&LATA + (port * 0x40U)) = (*(volatile uint32_t *)(&LATA + (port * 0x40U)) & (~mask)) | (mask & value);
}

// *****************************************************************************
/* Function:
    uint32_t GPIO_PortLatchRead ( GPIO_PORT port )

  Summary:
    Read the latched value on all the I/O lines of the selected port.

  Remarks:
    See plib_gpio.h for more details.
*/
uint32_t GPIO_PortLatchRead(GPIO_PORT port)
{
    return (*(volatile uint32_t *)(&LATA + (port * 0x40U)));
}

// *****************************************************************************
/* Function:
    void GPIO_PortSet ( GPIO_PORT port, uint32_t mask )

  Summary:
    Set the selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortSet(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATASET + (port * 0x40U)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortClear ( GPIO_PORT port, uint32_t mask )

  Summary:
    Clear the selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortClear(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATACLR + (port * 0x40U)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortToggle ( GPIO_PORT port, uint32_t mask )

  Summary:
    Toggles the selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortToggle(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&LATAINV + (port * 0x40U))= mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortInputEnable ( GPIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as input.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&TRISASET + (port * 0x40U)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortOutputEnable ( GPIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as output(s).

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&TRISACLR + (port * 0x40U)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask)

  Summary:
    Enables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&CNENASET + (port * 0x40U)) = mask;
}

// *****************************************************************************
/* Function:
    void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask)

  Summary:
    Disables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask)
{
    *(volatile uint32_t *)(&CNENACLR + (port * 0x40U)) = mask;
}

// *****************************************************************************
// *****************************************************************************
// Section: GPIO APIs which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void GPIO_PinIntEnable(GPIO_PIN pin, GPIO_INTERRUPT_STYLE style)

  Summary:
    Enables IO interrupt of particular style on selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PinIntEnable(GPIO_PIN pin, GPIO_INTERRUPT_STYLE style)
{
    GPIO_PORT port;
    uint32_t mask;

    port = (GPIO_PORT)(pin>>4U);
    mask =  0x1UL << (pin & 0xFU);

    if (style == GPIO_INTERRUPT_ON_MISMATCH)
    {
        *(volatile uint32_t *)(&CNENASET + (port * 0x40U)) = mask;
    }
    else if (style == GPIO_INTERRUPT_ON_RISING_EDGE)
    {
        *(volatile uint32_t *)(&CNENASET + (port * 0x40U)) = mask;
        *(volatile uint32_t *)(&CNNEACLR + (port * 0x40U)) = mask;
    }
    else if (style == GPIO_INTERRUPT_ON_FALLING_EDGE)
    {
        *(volatile uint32_t *)(&CNENACLR + (port * 0x40U)) = mask;
        *(volatile uint32_t *)(&CNNEASET + (port * 0x40U)) = mask;
    }
    else if (style == GPIO_INTERRUPT_ON_BOTH_EDGES)
    {
        *(volatile uint32_t *)(&CNENASET + (port * 0x40U)) = mask;
        *(volatile uint32_t *)(&CNNEASET + (port * 0x40U)) = mask;
    }
    else
    {
        /* Nothing to process */
    }
}

// *****************************************************************************
/* Function:
    void GPIO_PinIntDisable(GPIO_PIN pin)

  Summary:
    Disables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_gpio.h for more details.
*/
void GPIO_PinIntDisable(GPIO_PIN pin)
{
    GPIO_PORT port;
    uint32_t mask;

    port = (GPIO_PORT)(pin>>4U);
    mask =  0x1UL << (pin & 0xFU);

    *(volatile uint32_t *)(&CNENACLR + (port * 0x40U)) = mask;
    *(volatile uint32_t *)(&CNNEACLR + (port * 0x40U)) = mask;
}
// *****************************************************************************
/* Function:
    bool GPIO_PinInterruptCallbackRegister(
        GPIO_PIN pin,
        const GPIO_PIN_CALLBACK callback,
        uintptr_t context
    );

  Summary:
    Allows application to register callback for configured pin.

  Remarks:
    See plib_gpio.h for more details.
*/
bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const GPIO_PIN_CALLBACK callback,
    uintptr_t context
)
{
    uint8_t i;
    uint8_t portIndex;

    portIndex = (uint8_t)(pin >> 4U);

    for(i = portNumCb[portIndex]; i < portNumCb[portIndex +1]; i++)
    {
        if (portPinCbObj[i].pin == pin)
        {
            portPinCbObj[i].callback = callback;
            portPinCbObj[i].context  = context;
            return true;
        }
    }
    return false;
}

// *****************************************************************************
// *****************************************************************************
// Section: Local Function Implementation
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Function:
    void CHANGE_NOTICE_A_InterruptHandler(void)

  Summary:
    Interrupt Handler for change notice interrupt for channel A.

  Remarks:
    It is an internal function called from ISR, user should not call it directly.
*/
    
void __attribute__((used)) CHANGE_NOTICE_A_InterruptHandler(void)
{
    uint8_t i;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATA;
    status &= CNENA;

    PORTA;
    IFS3CLR = _IFS3_CNAIF_MASK;

    /* Check pending events and call callback if registered */
    for(i = 0; i < 2; i++)
    {
        pin = portPinCbObj[i].pin;

        if((portPinCbObj[i].callback != NULL) && ((status & ((uint32_t)1U << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[i].context;

            portPinCbObj[i].callback (pin, context);
        }
    }
}

// *****************************************************************************
/* Function:
    void CHANGE_NOTICE_B_InterruptHandler(void)

  Summary:
    Interrupt Handler for change notice interrupt for channel B.

  Remarks:
    It is an internal function called from ISR, user should not call it directly.
*/
    
void __attribute__((used)) CHANGE_NOTICE_B_InterruptHandler(void)
{
    uint8_t i;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATB;
    status &= CNENB;

    PORTB;
    IFS3CLR = _IFS3_CNBIF_MASK;

    /* Check pending events and call callback if registered */
    for(i = 2; i < 5; i++)
    {
        pin = portPinCbObj[i].pin;

        if((portPinCbObj[i].callback != NULL) && ((status & ((uint32_t)1U << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[i].context;

            portPinCbObj[i].callback (pin, context);
        }
    }
}

// *****************************************************************************
/* Function:
    void CHANGE_NOTICE_D_InterruptHandler(void)

  Summary:
    Interrupt Handler for change notice interrupt for channel D.

  Remarks:
    It is an internal function called from ISR, user should not call it directly.
*/
    
void __attribute__((used)) CHANGE_NOTICE_D_InterruptHandler(void)
{
    uint8_t i;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATD;
    status &= CNEND;

    PORTD;
    IFS3CLR = _IFS3_CNDIF_MASK;

    /* Check pending events and call callback if registered */
    for(i = 5; i < 6; i++)
    {
        pin = portPinCbObj[i].pin;

        if((portPinCbObj[i].callback != NULL) && ((status & ((uint32_t)1U << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[i].context;

            portPinCbObj[i].callback (pin, context);
        }
    }
}

// *****************************************************************************
/* Function:
    void CHANGE_NOTICE_F_InterruptHandler(void)

  Summary:
    Interrupt Handler for change notice interrupt for channel F.

  Remarks:
    It is an internal function called from ISR, user should not call it directly.
*/
    
void __attribute__((used)) CHANGE_NOTICE_F_InterruptHandler(void)
{
    uint8_t i;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATF;
    status &= CNENF;

    PORTF;
    IFS3CLR = _IFS3_CNFIF_MASK;

    /* Check pending events and call callback if registered */
    for(i = 6; i < 7; i++)
    {
        pin = portPinCbObj[i].pin;

        if((portPinCbObj[i].callback != NULL) && ((status & ((uint32_t)1U << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[i].context;

            portPinCbObj[i].callback (pin, context);
        }
    }
}

// *****************************************************************************
/* Function:
    void CHANGE_NOTICE_H_InterruptHandler(void)

  Summary:
    Interrupt Handler for change notice interrupt for channel H.

  Remarks:
    It is an internal function called from ISR, user should not call it directly.
*/
    
void __attribute__((used)) CHANGE_NOTICE_H_InterruptHandler(void)
{
    uint8_t i;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATH;
    status &= CNENH;

    PORTH;
    IFS3CLR = _IFS3_CNHIF_MASK;

    /* Check pending events and call callback if registered */
    for(i = 7; i < 17; i++)
    {
        pin = portPinCbObj[i].pin;

        if((portPinCbObj[i].callback != NULL) && ((status & ((uint32_t)1U << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[i].context;

            portPinCbObj[i].callback (pin, context);
        }
    }
}

// *****************************************************************************
/* Function:
    void CHANGE_NOTICE_J_InterruptHandler(void)

  Summary:
    Interrupt Handler for change notice interrupt for channel J.

  Remarks:
    It is an internal function called from ISR, user should not call it directly.
*/
    
void __attribute__((used)) CHANGE_NOTICE_J_InterruptHandler(void)
{
    uint8_t i;
    uint32_t status;
    GPIO_PIN pin;
    uintptr_t context;

    status  = CNSTATJ;
    status &= CNENJ;

    PORTJ;
    IFS3CLR = _IFS3_CNJIF_MASK;

    /* Check pending events and call callback if registered */
    for(i = 17; i < 20; i++)
    {
        pin = portPinCbObj[i].pin;

        if((portPinCbObj[i].callback != NULL) && ((status & ((uint32_t)1U << (pin & 0xFU))) != 0U))
        {
            context = portPinCbObj[i].context;

            portPinCbObj[i].callback (pin, context);
        }
    }
}


/*******************************************************************************
 End of File
*/
