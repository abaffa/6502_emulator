//
// screen.cpp
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
#include "screen.h"
#include <string.h>
#include <assert.h>


static void screen_check_bounds(int x, int y)
{
    assert(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

void screen_set(struct screen* screen, int x, int y)
{
    screen_check_bounds(x, y);
    screen->pixels[y][x] = true;
}

void screen_clear(struct screen* screen)
{
    memset(screen->pixels, 0, sizeof(screen->pixels));
}

bool screen_is_set(struct screen* screen, int x, int y)
{
    screen_check_bounds(x, y);
    return screen->pixels[y][x];
}

bool screen_draw_sprite(struct screen* screen, int x, int y, const char* sprite, int num)
{
    bool pixel_collision = false;
    int lx, ly;
    for(ly = 0; ly < num; ly++){
        char c = sprite[ly];
        for(lx = 0; lx < 8; lx++)
        {
            if((c & (0b10000000 >> lx)) == 0)
                continue;

            if(screen->pixels[(ly+y) % HEIGHT][(lx+x) % WIDTH])
            {
                pixel_collision = true;
            }

            screen->pixels[(ly+y) % HEIGHT][(lx+x) % WIDTH] ^= true;
        }
    }
    return pixel_collision;
}