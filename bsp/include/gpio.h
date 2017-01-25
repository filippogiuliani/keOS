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
#ifndef BSP_INCLUDE_GPIO_H_
#define BSP_INCLUDE_GPIO_H_

/******************************************************************************
 ***** Include                                                            *****
 ******************************************************************************/

/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef enum { GPIO_OFF = 0, GPIO_ON = 1 } bsp_gpio_state_t;

/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/


/******************************************************************************
 ***** Public function declaration                                        *****
 ******************************************************************************/

sint32_t bsp_gpioSetFunction ( const uint32_t gpio, const uint32_t function );
sint32_t bsp_gpioSetValue ( const uint32_t gpio, const bsp_gpio_state_t state );

#endif /* BSP_INCLUDE_GPIO_H_ */
