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

#define BSP_ARMTIMER_BASE    ( DEV_PERIPHERAL_BASE + 0xB400 )


/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef struct
{
	DEF_REG  (LOAD);
	DEF_CREG (VALUE);

	struct CONTROL
	{
		DEF_CBIT  (NotUsed0);
		DEF_BIT   (CounterLength);
		DEF_BITS  (Prescale,2);
		DEF_CBIT  (NotUsed1);
		DEF_BIT   (IntEnable);
		DEF_CBIT  (NotUsed2);
		DEF_BIT   (Enable);
		DEF_BIT   (Halt);
		DEF_BIT   (FreeRunning);
		DEF_CBITS (NotUsed3,6);
		DEF_BITS  (FreeRunningPrescale,8);
		DEF_CBITS (NotUsed4,8);
	} CONTROL;

	DEF_REG  (IRQCLEAR);

	struct RAW
	{
		DEF_CBIT  (InterruptPending);
		DEF_CBITS (Unused, 31);
	} RAW;

	struct MASKED
	{
		DEF_CBIT  (InterruptMasked);
		DEF_CBITS (Unused, 31);
	} MASKED;

	DEF_REG  (RELOAD);

	struct PREDIVIDER
	{
		DEF_BITS  (Value, 10);
		DEF_CBITS (Unused, 22);
	} PREDIVIDER;

	DEF_CREG (FREERUNNINGCOUNTER);

} arm_timer_t;

/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

arm_timer_t * ARMTimerCtrl = (arm_timer_t *)BSP_ARMTIMER_BASE;

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/

/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/



void bsp_armtimer_setPrescale (bsp_armtimer_prescaler_t value)
{
	switch (value)
	{
	case BSP_ARMTIMER_NOPRESCALE:
		ARMTimerCtrl->CONTROL.Prescale = 0;
		break;
	case BSP_ARMTIMER_PRESCALE_16:
		ARMTimerCtrl->CONTROL.Prescale = 1;
		break;
	case BSP_ARMTIMER_PRESCALE_256:
		ARMTimerCtrl->CONTROL.Prescale = 2;
		break;
	default:
		break;
	}
}

void bsp_armtimer_enable ( void)
{
	ARMTimerCtrl->CONTROL.Enable = 1;
}

void bsp_armtimer_int_enable ( void)
{
	ARMTimerCtrl->CONTROL.IntEnable = 1;
}

void bsp_armtimer_load ( uint32_t value )
{
	if (value > 0xFFFF)
		ARMTimerCtrl->CONTROL.CounterLength = 1;
	else
		ARMTimerCtrl->CONTROL.CounterLength = 0;

	ARMTimerCtrl->LOAD = value;
}

void bsp_armtimer_irqclear (void)
{
	ARMTimerCtrl->IRQCLEAR = 1;
}

/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/
