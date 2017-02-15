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

extern uint32_t __sBss;
extern uint32_t __eBss;

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/

extern void kernel_main (void);

/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

void _cstartup ( void)
{
	uint32_t *pBss = &__sBss;

	// Reset the BSS section
	while (pBss < &__eBss)
		*pBss++ = 0;

	kernel_main ();
}

/******************************************************************************
 ***** Private function definitions                                        *****
 ******************************************************************************/
