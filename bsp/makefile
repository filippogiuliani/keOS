#
#    Brief comment
#    
#    Detailed comment.
#
#
#    File:       <FILENAME>
#    Author:     <AUTHORNAME>
#    Mail:       <AUTHORMAIL>
#    Date:       <COMMITTERDATEISO8601>
#    Ident:      <COMMITHASH>
#    Branch:     <BRANCH>
# 
#    <CHANGELOG:--reverse --grep "^tags.*relevant":-1:%an : %ai : %s>
# 
# 
#    (C) Copyright 2016 Filippo Giuliani <mail@filippogiuliani.it>
# 
#    This file is part of keOS.
# 
#    keOS is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
# 
#    keOS is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
# 
#    You should have received a copy of the GNU General Public License
#    along with keOS.  If not, see <http://www.gnu.org/licenses/>.
#

KEOS_ROOT := ..
include $(KEOS_ROOT)/def.mk

DEP_DIR = $(KEOS_ROOT)/$(OUTPUT_DIR)/bsp/

all : src asm
	$(HIDE)$(ARMGNU)-ld -r $(DEP_DIR)bsp_src.o $(DEP_DIR)bsp_asm.o -o $(DEP_DIR)bsp.o $(FOUT)

src asm : FORCE
	$(HIDE)$(MAKE) -C $@ $(MFLAGS) all $(FOUT)

clean : 
	$(HIDE)$(MAKE) -C src $(MFLAGS) clean $(FOUT)

FORCE: 
