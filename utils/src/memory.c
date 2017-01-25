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
#include <utils.h>

/******************************************************************************
 ***** Constant definitions                                               *****
 ******************************************************************************/


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

void * u_memcpy ( void * destination, const void * source, size_t num )
{
	int i;
	uint8_t * src = (uint8_t *)source;
	uint8_t * dst = (uint8_t *)destination;

	for (i = 0; i < num; i++)
	{
		*dst = *src;
		dst++;
		src++;
	}

	return destination;
}

/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/
