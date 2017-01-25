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
#ifndef INCLUDE_TYPES_H_
#define INCLUDE_TYPES_H_

/******************************************************************************
 ***** Include                                                            *****
 ******************************************************************************/


/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/

#define ASSERT(cond)            \
    do                          \
    {                           \
        if (!(cond))            \
            return -1;          \
    } while (0);


#define NULL      ((void*)(0x0))

/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef enum { FALSE = 0, TRUE = 1 } bool_t;

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef signed int sint32_t;
typedef signed short sint16_t;
typedef signed char sint8_t;

typedef volatile unsigned int vuint32_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned char vuint8_t;

typedef volatile signed int vsint32_t;
typedef volatile signed short vsint16_t;
typedef volatile signed char vsint8_t;

typedef enum format_t
{
    FMT_DEC = 0,
    FMT_0xHEX32 = 1,
    FMT_0xHEX16 = 2,
    FMT_0xHEX8 = 3,
    FMT_HEX32 = 4,
    FMT_HEX16 = 5,
    FMT_HEX8 = 6,
    FMT_NULL = 7
} format_t;

typedef uint32_t size_t;

#endif /* INCLUDE_TYPES_H_ */
