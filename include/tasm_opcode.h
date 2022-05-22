//
// tasm_opcode.h
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
#ifndef TASM_OPCODESOL1COMPUTER_H
#define SOL1COMPUTER_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Tasm_Opcode
{
public:
	string opcode;
	string desc;
	int size;

	Tasm_Opcode() {};
	Tasm_Opcode(string opcode, string desc, int size);
};

#endif