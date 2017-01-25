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

#define BSP_INTERRUPT_BASE    ( DEV_PERIPHERAL_BASE + 0xB200 )

/******************************************************************************
 ***** Type definitions                                                   *****
 ******************************************************************************/

typedef struct interrupt_controller_t
{
/*        struct IRQpendbase
        {
                DEF_CBIT  (ARMTimerIRQPending);
                DEF_CBIT  (ARMMailboxIRQPending);
                DEF_CBIT  (ARMDoorbell0IRQPending);
                DEF_CBIT  (ARMDoorbell1IRQPending);
                DEF_CBIT  (GPU0haltedIRQPending);
                DEF_CBIT  (GPU1haltedIRQPending);
                DEF_CBIT  (IllegalAccesstype1IRQPending);
                DEF_CBIT  (IllegalAccesstype2IRQPending);
                DEF_CBIT  (BitsSetPendingRegister1);
                DEF_CBIT  (BitsSetPendingRegister2);
                DEF_CBIT  (GPUIRQ7);
                DEF_CBIT  (GPUIRQ9);
                DEF_CBIT  (GPUIRQ10);
                DEF_CBIT  (GPUIRQ18);
                DEF_CBIT  (GPUIRQ19);
                DEF_CBIT  (GPUIRQ53);
                DEF_CBIT  (GPUIRQ54);
                DEF_CBIT  (GPUIRQ55);
                DEF_CBIT  (GPUIRQ56);
                DEF_CBIT  (GPUIRQ57);
                DEF_CBIT  (GPUIRQ62);
                DEF_CBITS (Unused,11);
        } IRQpendbase;*/

	DEF_CREG  (IRQpendbase);
	DEF_CREG  (IRQpend1);
	DEF_CREG  (IRQpend2);

        struct FIQ
        {
                DEF_BITS  (FIQSource,7);
                DEF_BIT   (FIQEnable);
                DEF_CBITS (Unused,24);
        } FIQ;

        DEF_CREG  (IRQenable1);
        DEF_CREG  (IRQenable2);

        struct IRQenable3
        {
                DEF_BIT   (ARMTimer);
                DEF_BIT   (ARMMailbox);
                DEF_BIT   (ARMDoorbell0);
                DEF_BIT   (ARMDoorbell1);
                DEF_BIT   (GPU0Halted);
                DEF_BIT   (GPU1Halted);
                DEF_BIT   (AccessErrorType1);
                DEF_BIT   (AccessErrorType0);
                DEF_CBITS (Unused,24);
        } IRQenable3;

        DEF_CREG  (IRQdisable1);
        DEF_CREG  (IRQdisable2);

        struct IRQdisable3
        {
                DEF_BIT   (ARMTimer);
                DEF_BIT   (ARMMailbox);
                DEF_BIT   (ARMDoorbell0);
                DEF_BIT   (ARMDoorbell1);
                DEF_BIT   (GPU0Halted);
                DEF_BIT   (GPU1Halted);
                DEF_BIT   (AccessErrorType1);
                DEF_BIT   (AccessErrorType0);
                DEF_CBITS (Unused,24);
        } IRQdisable3;

} interrupt_controller_t;

/******************************************************************************
 ***** Vars definitions                                                   *****
 ******************************************************************************/

void (*undefined_instruction_handler)(void) = NULL;
void (*software_interrupt_handler)(void) = NULL;
void (*prefetch_abort_handler)(void) = NULL;
void (*data_abort_handler)(void) = NULL;

struct interrupt_handler
{
    void (*generic)(void);
    void (*base[32])(void);
    void (*IRQ[64])(void);
} interrupt_handler = {NULL, };

void (*fast_interrupt_handler)(void) = NULL;

interrupt_controller_t * intCtrl = (interrupt_controller_t *)BSP_INTERRUPT_BASE;

/******************************************************************************
 ***** Private function declaration                                       *****
 ******************************************************************************/

/******************************************************************************
 ***** Public function definitions                                        *****
 ******************************************************************************/

void bsp_enable_interrupts ( void)
{
        asm ("mrs r0, cpsr");
        asm ("bic r0, r0, #0x80");
        asm ("msr cpsr_c, r0");
}

void bsp_register_interrupt_handler(bps_interrupt_t type, void (*handler)(void))
{
        switch (type)
        {
        case UNDEFINED_INSTRUCTION:
                undefined_instruction_handler = handler;
                break;
        case SOFTWARE_INTERRUPT:
                software_interrupt_handler = handler;
                break;
        case PREFETCH_ABORT:
                prefetch_abort_handler = handler;
                break;
        case DATA_ABORT:
                data_abort_handler = handler;
                break;
        case INTERRUPT_HANDLER:
                interrupt_handler.generic = handler;
                break;
        case FAST_INTERRUPT_HANDLER:
                fast_interrupt_handler = handler;
                break;
        default:
                break;
        }
}

void bsp_register_irq_handler(interrupt_irq_t type, void (*handler)(void))
{
        switch (type)
        {
        case IRQARMTimer:
                interrupt_handler.base[0]= handler;
                break;
        case IRQARMMailbox:
                interrupt_handler.base[1] = handler;
                break;
        case IRQARMDoorbell0:
                interrupt_handler.base[2] = handler;
                break;
        case IRQARMDoorbell1:
                interrupt_handler.base[3] = handler;
                break;
        case IRQGPU0Halted:
                interrupt_handler.base[4] = handler;
                break;
        case IRQGPU1Halted:
                interrupt_handler.base[5] = handler;
                break;
        case IRQAccessErrorType1:
                interrupt_handler.base[6] = handler;
                break;
        case IRQAccessErrorType0:
                interrupt_handler.base[7] = handler;
                break;
        default:
                break;
        }
}

void bsp_enable_irq (interrupt_irq_t type)
{
        switch (type)
        {
        case IRQARMTimer:
                intCtrl->IRQenable3.ARMTimer = 1;
                break;
        case IRQARMMailbox:
                intCtrl->IRQenable3.ARMMailbox = 1;
                break;
        case IRQARMDoorbell0:
                intCtrl->IRQenable3.ARMDoorbell0 = 1;
                break;
        case IRQARMDoorbell1:
                intCtrl->IRQenable3.ARMDoorbell1 = 1;
                break;
        case IRQGPU0Halted:
                intCtrl->IRQenable3.GPU0Halted = 1;
                break;
        case IRQGPU1Halted:
                intCtrl->IRQenable3.GPU1Halted = 1;
                break;
        case IRQAccessErrorType1:
                intCtrl->IRQenable3.AccessErrorType1 = 1;
                break;
        case IRQAccessErrorType0:
                intCtrl->IRQenable3.AccessErrorType0 = 1;
                break;
        default:
                break;
        }
}

/******************************************************************************
 ***** Private function definitions                                       *****
 ******************************************************************************/

void __attribute__((interrupt("UNDEF"))) _undefined_instruction_handler(void)
{
        if (undefined_instruction_handler != NULL)
                undefined_instruction_handler();
        else
            device_printLine ( "Unhandled UNDEF interrupt\n");
}

void __attribute__((interrupt("SWI"))) _software_interrupt_handler(void)
{
        if (software_interrupt_handler != NULL)
                software_interrupt_handler();
        else
            device_printLine ( "Unhandled SWI interrupt\n");
}

void __attribute__((interrupt("ABORT"))) _prefetch_abort_handler(void)
{
        if (prefetch_abort_handler != NULL)
                prefetch_abort_handler();
        else
            device_printLine ( "Unhandled ABORT interrupt\n");
}

void __attribute__((interrupt("ABORT"))) _data_abort_handler(void)
{
        if (data_abort_handler != NULL)
                data_abort_handler();
        else
            device_printLine ( "Unhandled ABORT interrupt\n");
}

void __attribute__((interrupt("IRQ"))) _interrupt_handler(void)
{
    uint32_t ring = 0x1U;
    uint32_t counter = 0U;

    while (intCtrl->IRQpendbase != 0x0)
    {
        if ((intCtrl->IRQpendbase & ring) != 0)
        {
            if (counter == 8)
            {
                uint32_t i;
                uint32_t IRQpending = intCtrl->IRQpend1;
                for (i = 0U; i < 32; i++)
                {
                    if (((IRQpending & 0x1) == 0x1) &&
                        (interrupt_handler.IRQ[i] != NULL))
                        interrupt_handler.IRQ[i]();

                    IRQpending = IRQpending >> 1;
                }
            }
            else if (counter == 9)
            {
                uint32_t i;
                uint32_t IRQpending = intCtrl->IRQpend2;
                for (i = 32U; i < 64; i++)
                {
                    if ((IRQpending & 0x1) == 0x1 &&
                        interrupt_handler.IRQ[i] != NULL)
                        interrupt_handler.IRQ[i]();

                    IRQpending = IRQpending >> 1;
                }
            }
            else if (interrupt_handler.base[counter] != NULL)
            {
                interrupt_handler.base[counter]();
            }
        }

        if (counter < 31)
        {
            counter++;
            ring = ring << 1;
        }
        else
        {
            counter = 0U;
            ring = 0x1U;
        }
        
    }

    if (interrupt_handler.generic != NULL)
        interrupt_handler.generic();

}

void __attribute__((interrupt("FIQ"))) _fast_interrupt_handler(void)
{
        if (fast_interrupt_handler != NULL)
                fast_interrupt_handler();
        else
            device_printLine ( "Unhandled FIQ interrupt\n");
}
