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
#ifndef BSP_INCLUDE_PRIVATE_H_
#define BSP_INCLUDE_PRIVATE_H_

/******************************************************************************
 ***** Include                                                            *****
 ******************************************************************************/


/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

#define DEV_PERIPHERAL_BASE    (0x20000000U)

#define DEF_CBITS(x,y)    const volatile uint32_t x : y;
#define DEF_CBIT(x)       DEF_CBITS(x,1);
#define DEF_CREG(x)       DEF_CBITS(x,32);
#define DEF_BITS(x,y)     volatile uint32_t x : y;
#define DEF_BIT(x)        DEF_BITS(x,1);
#define DEF_REG(x)        DEF_BITS(x,32);

#define DEF_UNDEFREG(x)   struct x { DEF_CREG(UNDEF); } x;
#define DEF_NOREG(x)      struct x { DEF_CREG(NOREG); } x;


/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/



/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

#endif /* BSP_INCLUDE_PRIVATE_H_ */
