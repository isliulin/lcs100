/*****************************************************************************
 *   gpio.c:  GPIO C file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.20  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "LPC11xx.h"			/* LPC11xx Peripheral Registers */
#include "gpio.h"
#include "24C02.h"
#include "timer32.h"
/* Shadow registers used to prevent chance of read-modify-write errors */
/* Ultra-conservative approach... */
volatile uint32_t GPIOShadowPort0;
volatile uint32_t GPIOShadowPort1;
volatile uint32_t GPIOShadowPort2;
volatile uint32_t GPIOShadowPort3;

volatile uint32_t gpio0_counter = 0;
volatile uint32_t gpio1_counter = 0;
volatile uint32_t gpio2_counter = 0;
volatile uint32_t gpio3_counter = 0;
volatile uint32_t p0_1_counter  = 0;
volatile uint32_t p1_1_counter  = 0;
volatile uint32_t p2_1_counter  = 0;
volatile uint32_t p3_1_counter  = 0;

#if 0
/*****************************************************************************
** Function name:		PIOINT0_IRQHandler
**
** Descriptions:		Use one GPIO pin(port0 pin1) as interrupt source
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void PIOINT0_IRQHandler(void)
{
    uint32_t regVal;

    gpio0_counter++;
    regVal = GPIOIntStatus( PORT0, 1 );
    if ( regVal )
    {
        p0_1_counter++;
        GPIOIntClear( PORT0, 1 );
    }
    return;
}

/*****************************************************************************
** Function name:		PIOINT1_IRQHandler
**
** Descriptions:		Use one GPIO pin(port1 pin1) as interrupt source
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void PIOINT1_IRQHandler(void)
{
    uint32_t regVal;

    gpio1_counter++;
    regVal = GPIOIntStatus( PORT1, 1 );
    if ( regVal )
    {
        p1_1_counter++;
        GPIOIntClear( PORT1, 1 );
    }
    return;
}

/*****************************************************************************
** Function name:		PIOINT2_IRQHandler
**
** Descriptions:		Use one GPIO pin(port2 pin1) as interrupt source
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void PIOINT2_IRQHandler(void)
{
    uint32_t regVal;

    gpio2_counter++;
    regVal = GPIOIntStatus( PORT2, 1 );
    if ( regVal )
    {
        p2_1_counter++;
        GPIOIntClear( PORT2, 1 );
    }
    return;
}

/*****************************************************************************
** Function name:		PIOINT3_IRQHandler
**
** Descriptions:		Use one GPIO pin(port3 pin1) as interrupt source
**
** parameters:			None
** Returned value:		None
**
*****************************************************************************/
void PIOINT3_IRQHandler(void)
{
    uint32_t regVal;

    gpio3_counter++;
    regVal = GPIOIntStatus( PORT3, 1 );
    if ( regVal )
    {
        p3_1_counter++;
        GPIOIntClear( PORT3, 1 );
    }
    return;
}

#endif
/*****************************************************************************
** Function name:		GPIOInit
**
** Descriptions:		Initialize GPIO, install the
**						GPIO interrupt handler
**
** parameters:			None
** Returned value:		true or false, return false if the VIC table
**						is full and GPIO interrupt handler can be
**						installed.
**
*****************************************************************************/
void GPIOInit( void )
{
    /* Enable AHB clock to the GPIO domain. */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);	//ENABLE GPIO CLOCK
    /*I2C*///***********************************************************//
    LPC_SYSCON->PRESETCTRL |= (0x1<<1);
    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);  //ENABLE I2C  CLOCK
    LPC_IOCON->PIO0_4 &= ~0x3F;	/*  I2C I/O config */
    LPC_IOCON->PIO0_4 |= 0x01;		/* I2C SCL */
    LPC_IOCON->PIO0_5 &= ~0x3F;
    LPC_IOCON->PIO0_5 |= 0x01;		/* I2C SDA */
//***********************************************************//
    LPC_IOCON->PIO1_9 &= ~0x07; 										/* 将P1.9初始化为GPIO功能,继电器		*/
    LPC_GPIO1->DIR	  |= Relay;											/* 将P1.9方向设置为输出 		*/
    //LPC_GPIO1->DATA   |= Relay;		  									/* 将P1.9初始化输出高电平       */
    LPC_GPIO1->DATA   &= ~Relay;
    LPC_IOCON->PIO1_8 &= ~0x07; 										/* 将P1.8初始化为GPIO功能		zigbee CFG pin*/
//	LPC_GPIO1->DIR	  |= McuLed; 										/* 将P1.8方向设置为输出 		*/
//	LPC_GPIO1->DATA   |= McuLed; 										/* 将P1.8初始化输出高电平		*/
//	LPC_GPIO1->DIR	  |= Zigbee_CFG;
    GPIOSetDir(1,8,1);
    GPIOSetValue(1,8,1);



//***********************************************************//


#ifdef __JTAG_DISABLED
    LPC_IOCON->JTAG_TDO_PIO1_1  &= ~0x07;
    LPC_IOCON->JTAG_TDO_PIO1_1  |= 0x01;
#endif

    /* Set up NVIC when I/O pins are configured as external interrupts. */
//  NVIC_EnableIRQ(EINT0_IRQn);
//  NVIC_EnableIRQ(EINT1_IRQn);
//  NVIC_EnableIRQ(EINT2_IRQn);
//  NVIC_EnableIRQ(EINT3_IRQn);
    return;
}

/*****************************************************************************
** Function name:		GPIOSetDir
**
** Descriptions:		Set the direction in GPIO port
**
** parameters:			port num, bit position, direction (1 out, 0 input)
** Returned value:		None
**
*****************************************************************************/
void GPIOSetDir( uint32_t portNum, uint32_t bitPosi, uint32_t dir )
{
    /* if DIR is OUT(1), but GPIOx_DIR is not set, set DIR
    to OUT(1); if DIR is IN(0), but GPIOx_DIR is set, clr
    DIR to IN(0). All the other cases are ignored.
    On port3(bit 0 through 3 only), no error protection if
    bit value is out of range. */
    switch ( portNum )
    {
    case PORT0:
        if ( !(LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 1) )
            LPC_GPIO0->DIR |= (0x1<<bitPosi);
        else if ( (LPC_GPIO0->DIR & (0x1<<bitPosi)) && (dir == 0) )
            LPC_GPIO0->DIR &= ~(0x1<<bitPosi);
        break;
    case PORT1:
        if ( !(LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 1) )
            LPC_GPIO1->DIR |= (0x1<<bitPosi);
        else if ( (LPC_GPIO1->DIR & (0x1<<bitPosi)) && (dir == 0) )
            LPC_GPIO1->DIR &= ~(0x1<<bitPosi);
        break;
    case PORT2:
        if ( !(LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 1) )
            LPC_GPIO2->DIR |= (0x1<<bitPosi);
        else if ( (LPC_GPIO2->DIR & (0x1<<bitPosi)) && (dir == 0) )
            LPC_GPIO2->DIR &= ~(0x1<<bitPosi);
        break;
    case PORT3:
        if ( !(LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 1) )
            LPC_GPIO3->DIR |= (0x1<<bitPosi);
        else if ( (LPC_GPIO3->DIR & (0x1<<bitPosi)) && (dir == 0) )
            LPC_GPIO3->DIR &= ~(0x1<<bitPosi);
        break;
    default:
        break;
    }
    return;
}

/*****************************************************************************
** Function name:		GPIOSetValue
**
** Descriptions:		Set/clear a bitvalue in a specific bit position
**						in GPIO portX(X is the port number.)
**
** parameters:			port num, bit position, bit value
** Returned value:		None
**
*****************************************************************************/
void GPIOSetValue( uint32_t portNum, uint32_t bitPosi, uint32_t bitVal )
{
    /* if bitVal is 1, the bitPosi bit is set in the GPIOShadowPortx. Then
     * GPIOShadowPortx is written to the I/O port register. */
    switch ( portNum )
    {
    case PORT0:
        if(bitVal)
            GPIOShadowPort0 |= (1<<bitPosi);
        else
            GPIOShadowPort0 &= ~(1<<bitPosi);

        /* Use of shadow prevents bit operation error if the read value
        * (external hardware state) of a pin differs from the I/O latch
        * value. A potential side effect is that other GPIO code in this
        * project that is not aware of the shadow will have its GPIO
        * state overwritten.
        */
        LPC_GPIO0->DATA = GPIOShadowPort0;
        break;
    case PORT1:
        if(bitVal)
            GPIOShadowPort1 |= (1<<bitPosi);
        else
            GPIOShadowPort1 &= ~(1<<bitPosi);

        LPC_GPIO1->DATA = GPIOShadowPort1;
        break;
    case PORT2:
        if(bitVal)
            GPIOShadowPort2 |= (1<<bitPosi);
        else
            GPIOShadowPort2 &= ~(1<<bitPosi);

        LPC_GPIO2->DATA = GPIOShadowPort2;
        break;
    case PORT3:
        if(bitVal)
            GPIOShadowPort3 |= (1<<bitPosi);
        else
            GPIOShadowPort3 &= ~(1<<bitPosi);

        LPC_GPIO3->DATA = GPIOShadowPort3;
        break;
    default:
        break;
    }
    return;
}

#if 0
/*****************************************************************************
** Function name:		GPIOSetInterrupt
**
** Descriptions:		Set interrupt sense, event, etc.
**						edge or level, 0 is edge, 1 is level
**						single or double edge, 0 is single, 1 is double
**						active high or low, etc.
**
** parameters:			port num, bit position, sense, single/doube, polarity
** Returned value:		None
**
*****************************************************************************/
void GPIOSetInterrupt( uint32_t portNum, uint32_t bitPosi, uint32_t sense,
                       uint32_t single, uint32_t event )
{
    switch ( portNum )
    {
    case PORT0:
        if ( sense == 0 )
        {
            LPC_GPIO0->IS &= ~(0x1<<bitPosi);
            /* single or double only applies when sense is 0(edge trigger). */
            if ( single == 0 )
                LPC_GPIO0->IBE &= ~(0x1<<bitPosi);
            else
                LPC_GPIO0->IBE |= (0x1<<bitPosi);
        }
        else
            LPC_GPIO0->IS |= (0x1<<bitPosi);
        if ( event == 0 )
            LPC_GPIO0->IEV &= ~(0x1<<bitPosi);
        else
            LPC_GPIO0->IEV |= (0x1<<bitPosi);
        break;
    case PORT1:
        if ( sense == 0 )
        {
            LPC_GPIO1->IS &= ~(0x1<<bitPosi);
            /* single or double only applies when sense is 0(edge trigger). */
            if ( single == 0 )
                LPC_GPIO1->IBE &= ~(0x1<<bitPosi);
            else
                LPC_GPIO1->IBE |= (0x1<<bitPosi);
        }
        else
            LPC_GPIO1->IS |= (0x1<<bitPosi);
        if ( event == 0 )
            LPC_GPIO1->IEV &= ~(0x1<<bitPosi);
        else
            LPC_GPIO1->IEV |= (0x1<<bitPosi);
        break;
    case PORT2:
        if ( sense == 0 )
        {
            LPC_GPIO2->IS &= ~(0x1<<bitPosi);
            /* single or double only applies when sense is 0(edge trigger). */
            if ( single == 0 )
                LPC_GPIO2->IBE &= ~(0x1<<bitPosi);
            else
                LPC_GPIO2->IBE |= (0x1<<bitPosi);
        }
        else
            LPC_GPIO2->IS |= (0x1<<bitPosi);
        if ( event == 0 )
            LPC_GPIO2->IEV &= ~(0x1<<bitPosi);
        else
            LPC_GPIO2->IEV |= (0x1<<bitPosi);
        break;
    case PORT3:
        if ( sense == 0 )
        {
            LPC_GPIO3->IS &= ~(0x1<<bitPosi);
            /* single or double only applies when sense is 0(edge trigger). */
            if ( single == 0 )
                LPC_GPIO3->IBE &= ~(0x1<<bitPosi);
            else
                LPC_GPIO3->IBE |= (0x1<<bitPosi);
        }
        else
            LPC_GPIO3->IS |= (0x1<<bitPosi);
        if ( event == 0 )
            LPC_GPIO3->IEV &= ~(0x1<<bitPosi);
        else
            LPC_GPIO3->IEV |= (0x1<<bitPosi);
        break;
    default:
        break;
    }
    return;
}

/*****************************************************************************
** Function name:		GPIOIntEnable
**
** Descriptions:		Enable Interrupt Mask for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
void GPIOIntEnable( uint32_t portNum, uint32_t bitPosi )
{
    switch ( portNum )
    {
    case PORT0:
        LPC_GPIO0->IE |= (0x1<<bitPosi);
        break;
    case PORT1:
        LPC_GPIO1->IE |= (0x1<<bitPosi);
        break;
    case PORT2:
        LPC_GPIO2->IE |= (0x1<<bitPosi);
        break;
    case PORT3:
        LPC_GPIO3->IE |= (0x1<<bitPosi);
        break;
    default:
        break;
    }
    return;
}

/*****************************************************************************
** Function name:		GPIOIntDisable
**
** Descriptions:		Disable Interrupt Mask for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
void GPIOIntDisable( uint32_t portNum, uint32_t bitPosi )
{
    switch ( portNum )
    {
    case PORT0:
        LPC_GPIO0->IE &= ~(0x1<<bitPosi);
        break;
    case PORT1:
        LPC_GPIO1->IE &= ~(0x1<<bitPosi);
        break;
    case PORT2:
        LPC_GPIO2->IE &= ~(0x1<<bitPosi);
        break;
    case PORT3:
        LPC_GPIO3->IE &= ~(0x1<<bitPosi);
        break;
    default:
        break;
    }
    return;
}

/*****************************************************************************
** Function name:		GPIOIntStatus
**
** Descriptions:		Get Interrupt status for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
uint32_t GPIOIntStatus( uint32_t portNum, uint32_t bitPosi )
{
    uint32_t regVal = 0;

    switch ( portNum )
    {
    case PORT0:
        if ( LPC_GPIO0->MIS & (0x1<<bitPosi) )
            regVal = 1;
        break;
    case PORT1:
        if ( LPC_GPIO1->MIS & (0x1<<bitPosi) )
            regVal = 1;
        break;
    case PORT2:
        if ( LPC_GPIO2->MIS & (0x1<<bitPosi) )
            regVal = 1;
        break;
    case PORT3:
        if ( LPC_GPIO3->MIS & (0x1<<bitPosi) )
            regVal = 1;
        break;
    default:
        break;
    }
    return ( regVal );
}

/*****************************************************************************
** Function name:		GPIOIntClear
**
** Descriptions:		Clear Interrupt for a port pin.
**
** parameters:			port num, bit position
** Returned value:		None
**
*****************************************************************************/
void GPIOIntClear( uint32_t portNum, uint32_t bitPosi )
{
    switch ( portNum )
    {
    case PORT0:
        LPC_GPIO0->IC |= (0x1<<bitPosi);
        break;
    case PORT1:
        LPC_GPIO1->IC |= (0x1<<bitPosi);
        break;
    case PORT2:
        LPC_GPIO2->IC |= (0x1<<bitPosi);
        break;
    case PORT3:
        LPC_GPIO3->IC |= (0x1<<bitPosi);
        break;
    default:
        break;
    }
    return;
}

#endif
/******************************************************************************
**                            End Of File
******************************************************************************/
