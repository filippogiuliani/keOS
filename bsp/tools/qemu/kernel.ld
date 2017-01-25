/**                                                                            
 *    Linker Script for keOS                                                 
 *                                                                             
 *    This is the linker script to link keOS system. This file is auto       
 *    generate by kernel.py (invoked by the makefile).                         
 *                                                                             
 *                                                                             
 *    File:       <FILE>                                                       
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
                                                                               
                                                                               
OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm",                               
          "elf32-littlearm")                                                   
                                                                               
OUTPUT_ARCH(arm)                                                               
                                                                               
ENTRY(_start)                                                                  
                                                                               
SEARCH_DIR("=/usr/local/lib"); SEARCH_DIR("=/lib"); SEARCH_DIR("=/usr/lib");   
                                                                               
ENTRYPOINT = 0x10000;                                                           
                                                                               
SECTIONS {                                                                     
	/*                                                                     
	* First and formost we need the .text.startup section, containing the  
	* code to be run first. We allow room for the ATAGs and stack and      
	* conform to the bootloader's expectation by putting this code at     
	* 0x8000.                                                              
	*/                                                                     
	. = ENTRYPOINT;                                                        
	__sCode = .;                                                           
	.init :                                                                
	{                                                                      
		*(.init)                                                       
	}

	__sText = .;                                                  
	.text :                                                        
	{                                                                      

		__sKernelText = .;                        
		output/kernel/kernel.o(.text*);            
		. += __sKernelText == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eKernelText = .;                        

		__sBSPText = .;                        
		output/bsp/bsp.o(.text*);            
		. += __sBSPText == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eBSPText = .;                        

		__sUtilsText = .;                        
		output/utils/utils.o(.text*);            
		. += __sUtilsText == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eUtilsText = .;                        

	}                                                                      
	__eText = .;                                                  
	. = ALIGN (0x10);                                      

	__sData = .;                                                  
	.data :                                                        
	{                                                                      

		__sKernelData = .;                        
		output/kernel/kernel.o(.data);            
		. += __sKernelData == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eKernelData = .;                        

		__sBSPData = .;                        
		output/bsp/bsp.o(.data);            
		. += __sBSPData == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eBSPData = .;                        

		__sUtilsData = .;                        
		output/utils/utils.o(.data);            
		. += __sUtilsData == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eUtilsData = .;                        

	}                                                                      
	__eData = .;                                                  
	. = ALIGN (0x10);                                      

	__sRodata = .;                                                  
	.rodata :                                                        
	{                                                                      

		__sKernelRodata = .;                        
		output/kernel/kernel.o(.rodata*);            
		. += __sKernelRodata == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eKernelRodata = .;                        

		__sBSPRodata = .;                        
		output/bsp/bsp.o(.rodata*);            
		. += __sBSPRodata == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eBSPRodata = .;                        

		__sUtilsRodata = .;                        
		output/utils/utils.o(.rodata*);            
		. += __sUtilsRodata == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eUtilsRodata = .;                        

	}                                                                      
	__eRodata = .;                                                  
	. = ALIGN (0x10);                                      

	__sBss = .;                                                  
	.bss :                                                        
	{                                                                      

		__sKernelBss = .;                        
		output/kernel/kernel.o(.bss* COMMON);            
		. += __sKernelBss == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eKernelBss = .;                        

		__sBSPBss = .;                        
		output/bsp/bsp.o(.bss* COMMON);            
		. += __sBSPBss == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eBSPBss = .;                        

		__sUtilsBss = .;                        
		output/utils/utils.o(.bss* COMMON);            
		. += __sUtilsBss == . ? 0x10 : 0x0;
		. = ALIGN (0x10);                       
		__eUtilsBss = .;                        

	}                                                                      
	__eBss = .;                                                  
	. = ALIGN (0x10);                                      

	. = ALIGN(32 / 8);                                                     
	. = ALIGN(32 / 8);                                                     
                                                                               
	__eCode = .;                                                           
                                                                               
	.stack         0x10000 :                                               
	{                                                                      
		_stack = .;                                                    
		*(.stack)                                                      
	}                                                                      
                                                                               
	/*                                                                     
	* Finally comes everything else. A fun trick here is to put all other  
	* sections into this section, which will be discarded by default.      
	*/                                                                     
	/DISCARD/ : {                                                          
		*(*)                                                           
	}                                                                      
}                                                                              
