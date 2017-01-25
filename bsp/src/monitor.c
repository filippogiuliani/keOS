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
#include <private.h>
#include <bsp.h>
#include <font.h>


/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/


/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/



/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

static struct preference_t
{
	uint8_t* frameBufferAddr;
	console_preference_t console;
	monitor_preference_t monitor;
} preference;

static struct position_t
{
	uint32_t cursorX;
	uint32_t cursorY;
} position;

static const uint8_t dev_font[0x80][0x10] = FONT_DEF;

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/

static void drawPixel ( uint16_t color, uint32_t x, uint32_t y);
static void drawHex (uint8_t hex);

/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

void device_clearMonitor ( void)
{
    uint16_t* framebuffer = NULL;
    uint32_t x,y;

    framebuffer = (uint16_t *)preference.frameBufferAddr;

    for (y = 0U; y < 1050U; y++)
    {
    	for (x = 0U; x < 1680U; x++)
    	{
    		*framebuffer++ = preference.console.fgColor;
    	}
    }

    position.cursorX = 0U;
    position.cursorY = 0U;
}

void device_initializeMonitor ( console_preference_t console, monitor_preference_t monitor )
{
    bsp_mb_property_t* mp;

    bsp_mb_propertyInit();
    bsp_mb_propertyAddTag( DEV_TAG_ALLOCATE_BUFFER );
    bsp_mb_propertyAddTag( DEV_TAG_SET_PHYSICAL_SIZE, monitor.width, monitor.height );
    bsp_mb_propertyAddTag( DEV_TAG_SET_VIRTUAL_SIZE, monitor.width, monitor.height );
    bsp_mb_propertyAddTag( DEV_TAG_SET_DEPTH, monitor.depth );
    bsp_mb_propertyAddTag( DEV_TAG_GET_PITCH );
    bsp_mb_propertyAddTag( DEV_TAG_GET_PHYSICAL_SIZE );
    bsp_mb_propertyAddTag( DEV_TAG_GET_DEPTH );
    bsp_mb_propertyProcess();

    mp = bsp_mb_propertyGet( DEV_TAG_ALLOCATE_BUFFER );

    preference.frameBufferAddr = (uint8_t*)(mp->buffer[0]);
    preference.monitor = monitor;
    preference.console = console;

    device_clearMonitor ();
}

void device_drawChar ( char character)
{
	if (character > 0 && character <= 0x1F )
	{
		switch (character)
		{
		case 0x8:
			if (position.cursorX == 0)
			{
				position.cursorX = preference.monitor.width;
				position.cursorY -= 16;
			}

			position.cursorX -= 8;
			device_drawChar (0);

			if (position.cursorX == 0)
			{
				position.cursorX = preference.monitor.width;
				position.cursorY -= 16;
			}

			position.cursorX -= 8;
			break;

		case 0x9:
			device_drawChar (0);
			device_drawChar (0);
			device_drawChar (0);
			device_drawChar (0);
			break;

		case 0xA: // LF
			position.cursorX = 0;
			position.cursorY += 16;
			break;

		case 0xD:
			position.cursorX = 0;
			break;

		default:
			break;
		}
	}
	else if (character < 0x7F)
	{
		uint8_t i, j, line;

		for (i = 0U; i < 16U; i++)
		{
			line = dev_font[(uint8_t)character][i];

			for (j = 0U; j < 8U; j++)
			{
				if (line & 0x1)
					drawPixel (preference.console.fontColor, position.cursorX + j, position.cursorY + i);
				else
					drawPixel (preference.console.fgColor, position.cursorX + j, position.cursorY + i);

				line = line >> 1;
			}
		}

		position.cursorX += 8;

		if (position.cursorX + 8 >= 1680)
		{
			position.cursorX = 0;
			position.cursorY += 16;
		}
	}
}

void device_printLine ( char * string )
{
	while (*string != '\0')
	device_drawChar (*(string++));
}

void device_printNum ( uint32_t num, format_t format)
{
	switch (format)
	{
	case FMT_DEC:
		if (num >= 10)
		{
			device_printNum (num / 10, FMT_DEC);
		}
		device_drawChar ((num % 10) + 0x30);
	break;

	case FMT_0xHEX32:
		device_printLine ("0x");
		device_printNum (num, FMT_HEX32);
		break;

	case FMT_0xHEX16:
		device_printLine ("0x");
		device_printNum (num, FMT_HEX16);
		break;

	case FMT_0xHEX8:
		device_printLine ("0x");
		device_printNum (num, FMT_HEX8);
		break;

	case FMT_HEX32:
		drawHex ((num >> 28) & 0xF);
		drawHex ((num >> 24) & 0xF);
		drawHex ((num >> 20) & 0xF);
		drawHex ((num >> 16) & 0xF);
		drawHex ((num >> 12) & 0xF);
		drawHex ((num >> 8) & 0xF);
		drawHex ((num >> 4) & 0xF);
		drawHex (num & 0xF);
		break;

	case FMT_HEX16:
		drawHex ((num >> 12) & 0xF);
		drawHex ((num >> 8) & 0xF);
		drawHex ((num >> 4) & 0xF);
		drawHex (num & 0xF);
		break;

	case FMT_HEX8:
		drawHex ((num >> 4) & 0xF);
		drawHex (num & 0xF);
		break;

	case FMT_NULL:
		break;
	}
}


/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/

static void drawHex (uint8_t hex)
{
	if (hex >= 10)
		device_drawChar (hex + 0x37);
	else
		device_drawChar (hex + 0x30);
}

static void drawPixel ( uint16_t color, uint32_t x, uint32_t y)
{
	// FixMe!: Gestire la profondita di colore
	// FixMe!: Gestire il numero di colonne
	((uint16_t *)(preference.frameBufferAddr))[x + y * preference.monitor.width] = color;
}
