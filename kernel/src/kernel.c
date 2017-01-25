/**
 *    Brief comment
 *    
 *    Detailed comment.
 *
 *
 *    File:       <FILENAME>
 *    Author:     <AUTHORNAME>
 *    Mail:       <AUTHORMAIL>
 *    Date:       <COMMITTERDATEISO8601>
 *    Ident:      <COMMITHASH>
 *    Branch:     <BRANCH>
 *
 *    <CHANGELOG:--reverse --grep "^tags.*relevant":-1:%an : %ai : %s>
 *
 *
 *    (C) Copyright 2016 Filippo Giuliani <mail@filippogiuliani.it>
 *
 *    This file is part of keOS.
 *
 *    keOS is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    keOS is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with keOS.  If not, see <http://www.gnu.org/licenses/>.
 */

/******************************************************************************
 ***** Include                                                            *****
 ******************************************************************************/

#include <types.h>
#include <bsp.h>

/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_DEPTH    16      /* 16 or 32-bit */

#define COLOUR_DELTA    0.05    /* Float from 0 to 1 incremented by this amount */

/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/

void krnl_info (void );
void krnl_periodicHandler ( void);

/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;

void print_uart0(const char *s) {
    while(*s != '\0') { /* Loop until end of string */
        *UART0DR = (unsigned int)(*s); /* Transmit char */
        s++; /* Next char */
    }
}

void _cstartup (void )
{
	console_preference_t c = { 0x1111, 0xCCCC };
	monitor_preference_t m = { SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH };

    device_initializeMonitor ( c, m );

    krnl_info ();

    device_printLine ( "Enable IRQ ARMTimer\n");
    bsp_enable_irq (IRQARMTimer);
    device_printLine ( "Set timer to 0x400\n");
    bsp_armtimer_load (0x400);
    device_printLine ( "Set ARMTimer Prescale to 256\n");
    bsp_armtimer_setPrescale (BSP_ARMTIMER_PRESCALE_256);
    device_printLine ( "Enable ARMTimer interrupt\n");
    bsp_armtimer_int_enable ();
    device_printLine ( "Enable ARMTimer\n");
    bsp_armtimer_enable ();
    bsp_register_irq_handler (IRQARMTimer, &krnl_periodicHandler);
    device_printLine ( "Enable interrupts\n");
    bsp_enable_interrupts ();
    device_printLine ( "What now?\n");

    while (1);  // this function should never return
}

/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/

void krnl_periodicHandler ( void)
{
	static uint32_t counter = 0U;
	counter++;
	bsp_armtimer_irqclear();
    device_printLine ( "Period Handler: ");
	device_printNum ( counter, FMT_HEX32 );
    device_printLine ( "\n");
}

void krnl_info (void )
{
    bsp_mb_property_t* mp;

    bsp_mb_propertyInit();
    bsp_mb_propertyAddTag( DEV_TAG_GET_BOARD_MODEL );
    bsp_mb_propertyAddTag( DEV_TAG_GET_BOARD_REVISION );
    bsp_mb_propertyAddTag( DEV_TAG_GET_FIRMWARE_VERSION );
    bsp_mb_propertyAddTag( DEV_TAG_GET_BOARD_MAC_ADDRESS );
    bsp_mb_propertyAddTag( DEV_TAG_GET_BOARD_SERIAL );
    bsp_mb_propertyProcess();
    // OK

    mp = bsp_mb_propertyGet( DEV_TAG_GET_BOARD_MODEL );

    device_printLine ( "Board Model: ");

    if( mp )
    	device_printNum ( mp->buffer[0], FMT_DEC );
    else
    	device_printLine ( "NULL" );

    device_printLine ("\r\n");

    mp = bsp_mb_propertyGet( DEV_TAG_GET_BOARD_REVISION );

    device_printLine ( "Board Revision: ");

    if( mp )
    	device_printNum ( mp->buffer[0], FMT_HEX32 );
    else
    	device_printLine ( "NULL" );

    device_printLine ("\r\n");

    mp = bsp_mb_propertyGet( DEV_TAG_GET_FIRMWARE_VERSION );

    device_printLine ( "Firmware Version: ");

    if( mp )
    	device_printNum ( mp->buffer[0], FMT_HEX32 );
    else
    	device_printLine ( "NULL" );

    device_printLine ("\r\n");

    mp = bsp_mb_propertyGet( DEV_TAG_GET_BOARD_MAC_ADDRESS );

    device_printLine ( "MAC Address: ");

    if( mp )
    {
    	device_printNum ( mp->buffer[0] & 0xFF, FMT_HEX8 );
    	device_printLine( ":" );
    	device_printNum ( (mp->buffer[0] >> 8) & 0xFF, FMT_HEX8 );
    	device_printLine( ":" );
    	device_printNum ( (mp->buffer[0] >> 16) & 0xFF, FMT_HEX8 );
    	device_printLine( ":" );
    	device_printNum ( (mp->buffer[0] >> 24) & 0xFF, FMT_HEX8 );
    	device_printLine( ":" );
    	device_printNum ( mp->buffer[1] & 0xFF, FMT_HEX8 );
    	device_printLine( ":" );
    	device_printNum ( (mp->buffer[2] >> 8) & 0xFF, FMT_HEX8 );
    }
    else
    	device_printLine ( "NULL" );

    device_printLine ("\r\n");

    mp = bsp_mb_propertyGet( DEV_TAG_GET_BOARD_SERIAL );

    device_printLine ( "Serial Number: ");

    if( mp )
    {
    	device_printNum ( mp->buffer[0], FMT_HEX32 );
    	device_printNum ( mp->buffer[1], FMT_HEX32 );
    }
    else
    	device_printLine ( "NULL" );

    device_printLine ("\r\n");
}
