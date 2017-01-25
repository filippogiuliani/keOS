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
#ifndef INCLUDE_BSP_H_
#define INCLUDE_BSP_H_

/******************************************************************************
 ***** Include                                                            *****
 ******************************************************************************/


/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/


/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef enum
{
    /* Videocore */
    DEV_TAG_GET_FIRMWARE_VERSION = 0x1,

    /* Hardware */
    DEV_TAG_GET_BOARD_MODEL = 0x10001,
    DEV_TAG_GET_BOARD_REVISION,
    DEV_TAG_GET_BOARD_MAC_ADDRESS,
    DEV_TAG_GET_BOARD_SERIAL,
    DEV_TAG_GET_ARM_MEMORY,
    DEV_TAG_GET_VC_MEMORY,
    DEV_TAG_GET_CLOCKS,

    /* Config */
    DEV_TAG_GET_COMMAND_LINE = 0x50001,

    /* Shared resource management */
    DEV_TAG_GET_DMA_CHANNELS = 0x60001,

    /* Power */
    DEV_TAG_GET_POWER_STATE = 0x20001,
    DEV_TAG_GET_TIMING,
    DEV_TAG_SET_POWER_STATE = 0x28001,

    /* Clocks */
    DEV_TAG_GET_CLOCK_STATE = 0x30001,
    DEV_TAG_SET_CLOCK_STATE = 0x38001,
    DEV_TAG_GET_CLOCK_RATE = 0x30002,
    DEV_TAG_SET_CLOCK_RATE = 0x38002,
    DEV_TAG_GET_MAX_CLOCK_RATE = 0x30004,
    DEV_TAG_GET_MIN_CLOCK_RATE = 0x30007,
    DEV_TAG_GET_TURBO = 0x30009,
    DEV_TAG_SET_TURBO = 0x38009,

    /* Voltage */
    DEV_TAG_GET_VOLTAGE = 0x30003,
    DEV_TAG_SET_VOLTAGE = 0x38003,
    DEV_TAG_GET_MAX_VOLTAGE = 0x30005,
    DEV_TAG_GET_MIN_VOLTAGE = 0x30008,
    DEV_TAG_GET_TEMPERATURE = 0x30006,
    DEV_TAG_GET_MAX_TEMPERATURE = 0x3000A,
    DEV_TAG_ALLOCATE_MEMORY = 0x3000C,
    DEV_TAG_LOCK_MEMORY = 0x3000D,
    DEV_TAG_UNLOCK_MEMORY = 0x3000E,
    DEV_TAG_RELEASE_MEMORY = 0x3000F,
    DEV_TAG_EXECUTE_CODE = 0x30010,
    DEV_TAG_GET_DISPMANX_MEM_HANDLE = 0x30014,
    DEV_TAG_GET_EDID_BLOCK = 0x30020,

    /* Framebuffer */
    DEV_TAG_ALLOCATE_BUFFER = 0x40001,
    DEV_TAG_RELEASE_BUFFER = 0x48001,
    DEV_TAG_BLANK_SCREEN = 0x40002,
    DEV_TAG_GET_PHYSICAL_SIZE = 0x40003,
    DEV_TAG_TEST_PHYSICAL_SIZE = 0x44003,
    DEV_TAG_SET_PHYSICAL_SIZE = 0x48003,
    DEV_TAG_GET_VIRTUAL_SIZE = 0x40004,
    DEV_TAG_TEST_VIRTUAL_SIZE = 0x44004,
    DEV_TAG_SET_VIRTUAL_SIZE = 0x48004,
    DEV_TAG_GET_DEPTH = 0x40005,
    DEV_TAG_TEST_DEPTH = 0x44005,
    DEV_TAG_SET_DEPTH = 0x48005,
    DEV_TAG_GET_PIXEL_ORDER = 0x40006,
    DEV_TAG_TEST_PIXEL_ORDER = 0x44006,
    DEV_TAG_SET_PIXEL_ORDER = 0x48006,
    DEV_TAG_GET_ALPHA_MODE = 0x40007,
    DEV_TAG_TEST_ALPHA_MODE = 0x44007,
    DEV_TAG_SET_ALPHA_MODE = 0x48007,
    DEV_TAG_GET_PITCH = 0x40008,
    DEV_TAG_GET_VIRTUAL_OFFSET = 0x40009,
    DEV_TAG_TEST_VIRTUAL_OFFSET = 0x44009,
    DEV_TAG_SET_VIRTUAL_OFFSET = 0x48009,
    DEV_TAG_GET_OVERSCAN = 0x4000A,
    DEV_TAG_TEST_OVERSCAN = 0x4400A,
    DEV_TAG_SET_OVERSCAN = 0x4800A,
    DEV_TAG_GET_PALETTE = 0x4000B,
    DEV_TAG_TEST_PALETTE = 0x4400B,
    DEV_TAG_SET_PALETTE = 0x4800B,
    DEV_TAG_SET_CURSOR_INFO = 0x8010,
    DEV_TAG_SET_CURSOR_STATE = 0x8011
} bsp_mb_tag_t;

typedef struct
{
    sint32_t tag;
    sint32_t bufferLength;
    sint32_t valueLength;
    sint32_t indicator;
    uint32_t buffer[256];
} bsp_mb_property_t;

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint32_t depth;
} monitor_preference_t;

typedef struct
{
	uint32_t fgColor;
	uint32_t fontColor;
} console_preference_t;

typedef enum
{
	UNDEFINED_INSTRUCTION,
	SOFTWARE_INTERRUPT,
	PREFETCH_ABORT,
	DATA_ABORT,
	INTERRUPT_HANDLER,
	FAST_INTERRUPT_HANDLER
} bps_interrupt_t;

typedef enum
{
	IRQARMTimer,
	IRQARMMailbox,
	IRQARMDoorbell0,
	IRQARMDoorbell1,
	IRQGPU0Halted,
	IRQGPU1Halted,
	IRQAccessErrorType1,
	IRQAccessErrorType0
} interrupt_irq_t;

typedef enum
{
	BSP_ARMTIMER_NOPRESCALE,
	BSP_ARMTIMER_PRESCALE_16,
	BSP_ARMTIMER_PRESCALE_256
} bsp_armtimer_prescaler_t;

/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

/** Monitor routines */
void device_clearMonitor ( void);
void device_initializeMonitor ( const console_preference_t console, const monitor_preference_t monitor );
void device_drawChar ( char character);
void device_printLine ( char * string );
void device_printNum ( uint32_t num, format_t format);

/** Mailbox routines */
void bsp_mb_propertyInit( void );
void bsp_mb_propertyAddTag( bsp_mb_tag_t tag, ... );
sint32_t bsp_mb_propertyProcess( void );
bsp_mb_property_t* bsp_mb_propertyGet( bsp_mb_tag_t tag );

/** Interrupt routines */
void bsp_enable_interrupts ( void);
void bsp_register_interrupt_handler(bps_interrupt_t type, void (*handler)(void));
void bsp_register_irq_handler(interrupt_irq_t type, void (*handler)(void));
void bsp_enable_irq (interrupt_irq_t type);

/** Timer routines */
void bsp_armtimer_load ( uint32_t value );
void bsp_armtimer_setPrescale (bsp_armtimer_prescaler_t value);
void bsp_armtimer_enable ( void);
void bsp_armtimer_int_enable ( void);
void bsp_armtimer_irqclear (void);

#endif /* INCLUDE_BSP_H_ */
