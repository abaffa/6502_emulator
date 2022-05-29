//
// config.h
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
#ifndef CONFIG_H
#define CONFIG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EMULATOR_WINDOW_TITLE "6502 Emulator"

#define SERVER_TELNET 0
#define SERVER_WEB 1

#define DEBUG_OPCODE 0
#define DEBUG_LOG 0


#define MEMORY_SIZE 0xFFFF //0x100

#define WIDTH 256 //32 x 8
#define HEIGHT 200 // 25 x 8
#define BORDER 40
#define WINDOW_MULTIPLIER 2


#define TOTAL_KEYS 41

#endif


