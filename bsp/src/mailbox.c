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

#include <stdarg.h>
#include <types.h>
#include <bsp.h>
#include <private.h>
#include <utils.h>

/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

#define BSP_MAILBOX0_BASE    ( DEV_PERIPHERAL_BASE + 0xB880 )


/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef enum
{
    MB0_POWER_MANAGEMENT = 0,
    MB0_FRAMEBUFFER,
    MB0_VIRTUAL_UART,
    MB0_VCHIQ,
    MB0_LEDS,
    MB0_BUTTONS,
    MB0_TOUCHSCREEN,
    MB0_UNUSED,
    MB0_TAGS_ARM_TO_VC,
    MB0_TAGS_VC_TO_ARM,
} mailbox0_channel_t;

typedef enum
{
    T_OIDENT = 0,
    T_OVALUE_SIZE = 1,
    T_ORESPONSE = 2,
    T_OVALUE = 3,
} tag_offset_t;

typedef struct mailbox_t
{
    const struct Read
    {
        DEF_BITS  (Channel,4);
        DEF_BITS  (Value,28);
    } Read;
    DEF_UNDEFREG(Undefined0x84);
    DEF_UNDEFREG(Undefined0x88);
    DEF_UNDEFREG(Undefined0x8C);
    DEF_CREG(Poll);
    DEF_CREG(Sender);
    struct Status
    {
        DEF_CBITS (RSRVD,30);
        DEF_CBIT  (ARM_MS_EMPTY);
        DEF_CBIT  (ARM_MS_FULL);
    } Status;
    DEF_CREG(Configuration);
    DEF_REG(Write);
} mailbox_t;


/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/


/* Mailbox 0 mapped to it's base address */
static mailbox_t* rpiMailbox0 = (mailbox_t*)BSP_MAILBOX0_BASE;

static struct properties_t
{
	/* Buffer allocation */
	sint32_t data[8192];
	/* Index declaration */
	sint32_t index;
} properties __attribute__((aligned(16)));


/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/

static void mailbox0Write( mailbox0_channel_t channel, uint32_t value );

static sint32_t mailbox0Read( mailbox0_channel_t channel );


/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

void bsp_mb_propertyInit( void )
{
    /* Fill in the size on-the-fly */
    properties.data[0] = 12;

    /* Process request (All other values are reserved!) */
    properties.data[1] = 0;

    /* First available data slot */
    properties.index = 2;

    /* NULL tag to terminate tag list */
    properties.data[properties.index] = 0;
}

/**
   @brief Add a property tag to the current tag list. Data can be included. All data is uint32_t
   @param tag
*/
void bsp_mb_propertyAddTag( bsp_mb_tag_t tag, ... )
{
    va_list vl;
    va_start( vl, tag );

    properties.data[properties.index++] = tag;

    switch( tag )
    {
    case DEV_TAG_GET_BOARD_MODEL:
    case DEV_TAG_GET_BOARD_REVISION:
    case DEV_TAG_GET_FIRMWARE_VERSION:
    case DEV_TAG_GET_DMA_CHANNELS:
    case DEV_TAG_GET_PITCH:
    case DEV_TAG_GET_DEPTH:
    case DEV_TAG_GET_ALPHA_MODE:
    case DEV_TAG_GET_PIXEL_ORDER:
        properties.data[properties.index++] = 4;
        properties.data[properties.index++] = 0; /**< Request with no payload*/
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
    break;
    case DEV_TAG_GET_BOARD_SERIAL:
    case DEV_TAG_GET_BOARD_MAC_ADDRESS:
    case DEV_TAG_GET_ARM_MEMORY:
    case DEV_TAG_GET_VC_MEMORY:
    case DEV_TAG_GET_PHYSICAL_SIZE:
    case DEV_TAG_GET_VIRTUAL_SIZE:
    case DEV_TAG_GET_VIRTUAL_OFFSET:
        properties.data[properties.index++] = 8;
        properties.data[properties.index++] = 0; /**< Request with no payload*/
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
    break;

    //    case DEV_TAG_GET_CLOCKS:           /* DA RIVEDERE */
    //    case DEV_TAG_GET_COMMAND_LINE:
    //    /* Provide a 256-byte buffer */
    //    dev_pt[dev_pt_index++] = 256;
    //    dev_pt[dev_pt_index++] = 0; /* Request */
    //    dev_pt_index += 256 >> 2;
    //    break;

    case DEV_TAG_ALLOCATE_BUFFER:
        properties.data[properties.index++] = 8;
        properties.data[properties.index++] = 4;    /**< Request with 4 bytes
                                                     *   of payload          */
        properties.data[properties.index++] = 0x10; /* Payload               */
        properties.data[properties.index++] = 0;    /**< Provide space for
                                                     *   the response        */
    break;

    case DEV_TAG_SET_PHYSICAL_SIZE:
    case DEV_TAG_SET_VIRTUAL_SIZE:
    case DEV_TAG_SET_VIRTUAL_OFFSET:
    case DEV_TAG_TEST_PHYSICAL_SIZE:
    case DEV_TAG_TEST_VIRTUAL_SIZE:
        properties.data[properties.index++] = 8;
        properties.data[properties.index++] = 8;    /**< Request with 8 bytes
                                                     *   of payload          */
        properties.data[properties.index++] = va_arg( vl, int ); /**< Width  */
        properties.data[properties.index++] = va_arg( vl, int ); /**< Height */
    break;

    case DEV_TAG_SET_DEPTH:
    case DEV_TAG_SET_ALPHA_MODE:
    case DEV_TAG_SET_PIXEL_ORDER:
        properties.data[properties.index++] = 4;
        properties.data[properties.index++] = 4;    /**< Request with 4 bytes
                                                     *   of payload          */
        /* Colour Depth, bits-per-pixel \ Pixel Order State */
        properties.data[properties.index++] = va_arg( vl, int );
    break;

    case DEV_TAG_GET_OVERSCAN: /* DA RIVEDERE */
        properties.data[properties.index++] = 0;
        properties.data[properties.index++] = 16;/**< Request with 16 bytes
                                                  *   of payload             */
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
        properties.data[properties.index++] = 0; /**< Provide space for the
                                                  *   response               */
        break;

    case DEV_TAG_SET_OVERSCAN:
        properties.data[properties.index++] = 16;
        properties.data[properties.index++] = 16;/**< Request with 16 bytes
                                                  *   of payload             */
        properties.data[properties.index++] = va_arg( vl, int ); /* Top pixels */
        properties.data[properties.index++] = va_arg( vl, int ); /* Bottom pixels */
        properties.data[properties.index++] = va_arg( vl, int ); /* Left pixels */
        properties.data[properties.index++] = va_arg( vl, int ); /* Right pixels */
        break;

    default:
        /* Unsupported tags, just remove the tag from the list */
        properties.index--;
        break;
    }

    /* Make sure the tags are 0 terminated to end the list and update the buffer size */
    /* dev_pt[dev_pt_index] = 0; */

    va_end( vl );
}


sint32_t bsp_mb_propertyProcess( void )
{
    sint32_t result;

    /* Fill in the size of the buffer */
    properties.data[properties.index] = 0x0;
    properties.data[0] = ( properties.index + 1 ) << 2;
    properties.data[1] = 0;

    mailbox0Write( MB0_TAGS_ARM_TO_VC, (uint32_t)&(properties.data[0]) + 0x40000000U );

    result = mailbox0Read( MB0_TAGS_ARM_TO_VC );

    return result;
}


bsp_mb_property_t* bsp_mb_propertyGet( bsp_mb_tag_t tag )
{
    static bsp_mb_property_t property;
    sint32_t* tag_buffer = NULL;

    property.tag = tag;

    /* Get the tag from the buffer. Start at the first tag position  */
    sint32_t index = 2;

    while( index < ( properties.data[0] >> 2 ) )
    {
        /* printf( "Test Tag: [%d] %8.8X\r\n", index, pt[index] ); */
        if( properties.data[index] == tag )
        {
            tag_buffer = &properties.data[index];
            break;
        }

        /* Progress to the next tag if we haven't yet discovered the tag */
        index += ( properties.data[index + 1] >> 2 ) + 3;
    }

    /* Return NULL of the property tag cannot be found in the buffer */
    if( tag_buffer == NULL )
        return NULL;

    /* Return the required data */
    property.bufferLength = tag_buffer[T_OVALUE_SIZE];
    property.valueLength = tag_buffer[T_ORESPONSE] & 0x7FFFFFFF;
    property.indicator = tag_buffer[T_ORESPONSE] & 0x80000000;

    u_memcpy( (void*)&property.buffer[0], (void*)&tag_buffer[T_OVALUE], property.bufferLength );

    return &property;
}

/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/


static void mailbox0Write
(
    mailbox0_channel_t channel,
    uint32_t value
    )
{
    /* Add the channel number into the lower 4 bits */
    value &= ~(0xF);
    value |= channel;

    /* Wait until the mailbox becomes available and then write to the mailbox
       channel */
    while( rpiMailbox0->Status.ARM_MS_FULL != 0 ) { }

    /* Write the modified value + channel number into the write register */
    rpiMailbox0->Write = value;
}


static sint32_t mailbox0Read
(
    mailbox0_channel_t channel
    )
{
    struct Read read = {-1, 0};

    /* Keep reading the register until the desired channel gives us a value */
    while( read.Channel != channel )
    {
        /* Wait while the mailbox is empty */
        while( rpiMailbox0->Status.ARM_MS_EMPTY ) { }

        /* Extract the value from the Read register of the mailbox. */
        read = rpiMailbox0->Read;
    }

    /* Return just the value */
    return read.Value;
}
