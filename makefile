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

include def.mk

IMAGE = $(OUTPUT_DIR)/kernel.img
LINKER = bsp/tools/$(TARGET)/kernel.ld

all: $(OUTPUT_DIR)/kernel.elf
install: $(IMAGE)


$(IMAGE): $(OUTPUT_DIR)/kernel.elf
	@echo Creating $(IMAGE)..
	$(HIDE)$(ARMGNU)-objcopy $(OUTPUT_DIR)/kernel.elf -O binary $(IMAGE) $(FOUT)
	
$(OUTPUT_DIR)/kernel.elf : bsp kernel utils
	@echo Creating kernel.elf..
	$(HIDE)$(ARMGNU)-ld $(LFLAGS) -o $@ -T $(LINKER) $(FOUT)

bsp kernel utils : FORCE
	@echo Compiling the $@
	$(HIDE)$(MAKE) -C $@ all $(MFLAGS)
	
	
FORCE : 




