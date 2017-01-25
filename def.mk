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

ARMGNU ?= arm-none-eabi
MAKE = /usr/bin/make

#GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)

CFLAGS = -Wall -O2 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
IFLAGS = -I$(KEOS_ROOT)/include
LFLAGS = -nostartfiles		\
         --no-undefined

CONFIG = -DVERSION=\"$(GIT_VERSION)\"

OUTPUT_DIR = output

MODULE_ALIGNMENT = 0x10
SECTION_ALIGNMENT = 0x10

ifndef target
	TARGET = qemu
else
	TARGET = target
endif

ifndef build
	BUILD = release
else
	BUILD = $(build)
	CFLAGS += -g -ggdb
	CONFIG += -DDEBUG
endif


ifndef verbose
	HIDE = @
	FOUT = > /dev/null
	MFLAGS = -s
else
	HIDE =
	FOUT =
	MFLAGS =
endif

.PHONY: all
