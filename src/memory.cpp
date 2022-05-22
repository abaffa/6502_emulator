//
// memory.cpp
//
////// BEGIN LICENSE NOTICE//////
//
//6502 Emulator 
//
//Copyright(C) 2022 Augusto Baffa, (baffa-6502.baffasoft.com.br)
//
//This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301, USA.
//
////// END LICENSE NOTICE//////
//
#include "memory.h"
#include <assert.h>
#include <stdio.h>

static void is_memory_in_bounds(int index)
{
	assert(index >= 0 && index < MEMORY_SIZE);
}

void memory_set(struct memory* memory, int index, unsigned char val)
{
	is_memory_in_bounds(index);
	memory->memory[index] = val;
}

unsigned char memory_get(struct memory* memory, int index)
{
	is_memory_in_bounds(index);
	return memory->memory[index];
}

unsigned short memory_get_short(struct memory* memory, int index)
{
	unsigned char byte1 = memory_get(memory, index);
	unsigned char byte2 = memory_get(memory, index + 1);

	return byte1 << 8 | byte2;
}

