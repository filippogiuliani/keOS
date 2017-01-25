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
#include <private.h>
#include <gpio.h>

/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

#define DEV_GPIO_BASE               ( DEV_PERIPHERAL_BASE + 0x00200000 )

/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/


/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/


/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

sint32_t bsp_gpioSetFunction ( const uint32_t gpio, const uint32_t function )
{
    uint32_t __gpio = gpio;
    vuint32_t* __address = (uint32_t*)DEV_GPIO_BASE;
    uint32_t __function = function;
    uint32_t __mask;

    /* Check the validity of the args */
    ASSERT ( gpio <= 53  && function <= 7 );

    /* Get the addresses */
    while (__gpio > 9)
    {
        __gpio -= 10U;
        __address++;
    }

    __gpio *= 3;

    /* compute the value to be written in the register to enable the requested
     * function */
    __function = __function << __gpio;

    /* Prepare a mask to zeroed the bit */
    __mask = 0x7 << __gpio;

    /* Write the computed value on the computed address */
    *__address = (*__address & ~__mask) | __function;

    /* Return 0 (ok status) */
    return 0;
}

sint32_t bsp_gpioSetValue ( const uint32_t gpio, const bsp_gpio_state_t state )
{
    ASSERT ( gpio <= 53 && ( state == GPIO_OFF || state == GPIO_ON ) );

    /* Get the address */
    vuint32_t* __address = (uint32_t*)DEV_GPIO_BASE + (gpio >> 5U);

    /* Compute the bit representing the gpio */
    uint32_t __bit = 1 << (gpio & 0x1F);

    /* Write the bit on the right register in order to clear or set the gpio */
    if (state == GPIO_OFF)
        *(__address + 10) = __bit;
    else
        *(__address + 7) = __bit;

    /* Return 0 (ok status) */
    return 0;
}

/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/
