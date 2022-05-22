//
// keyboard.cpp
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
#include "keyboard.h"
#include <assert.h>

static void keyboard_ensure_in_bounds(int key){
    assert(key >= 0 && key < TOTAL_KEYS);
}

void keyboard_set_map(struct keyboard* keyboard, const unsigned int* map)
{
    keyboard->keyboard_map = map;
    int i;
    for(i = 0; i < TOTAL_KEYS; i++)
        keyboard->keyboard[i] = false;
}

int keyboard_map(struct keyboard* keyboard, int key)
{
    
    int i;
    for(i = 0; i < TOTAL_KEYS; i++)
    {
        if(keyboard->keyboard_map[i] == key)
        {
            return i;
        }
    }

    return -1;
}

void keyboard_down(struct keyboard* keyboard, int key)
{
    keyboard->keyboard[key] = true;
}

void keyboard_up(struct keyboard* keyboard, int key)
{
    keyboard->keyboard[key] = false; 
}

bool keyboard_is_down(struct keyboard* keyboard, int key)
{
    return keyboard->keyboard[key];
}