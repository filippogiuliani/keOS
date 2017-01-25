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

/* This is the init section (will be placed on the entry point by the linker
 * using the linker script */
.section .init
.globl _start

_start:
	ldr   pc, [pc, #24] //=_reset_handler
	ldr   pc, [pc, #24] //=_undefined_instruction_handler
	ldr   pc, [pc, #24] //=_software_interrupt_handler
	ldr   pc, [pc, #24] //=_prefetch_abort_handler
	ldr   pc, [pc, #24] //=_data_abort_handler
	ldr   pc, [pc, #4] //=_reset_handler
	ldr   pc, [pc, #20] //=_interrupt_handler
	ldr   pc, [pc, #20] //=_fast_interrupt_handler

_reset_handler_addr:                    .word   _reset_handler
_undefined_instruction_handler_addr:	.word   _undefined_instruction_handler
_software_interrupt_handler_addr:       .word   _software_interrupt_handler
_prefetch_abort_handler_addr:           .word   _prefetch_abort_handler
_data_abort_handler_addr:               .word   _data_abort_handler
_interrupt_handler_addr:                .word   _interrupt_handler
_fast_interrupt_handler_addr:           .word   _fast_interrupt_handler

_reset_handler:
	/* The program will jump here since first instruction on _start is to move
	 * the pc here */

	/* Load the vector table from init section to actual vector table */
	ldr   	r0,=_start
	mov   	r1,#0x0000
	ldmia 	r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia 	r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia 	r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia 	r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

	/* Set the stack pointer, which progresses downwards through memory */
	ldr		sp, =_stack

	/* This function should not return (entry point of the kernel) */
	b		_cstartup

	/* If the _cstartup function will ever return (that should be impossible)
	 * loop forever */
loop$:
	b     loop$
