//
// main.cpp
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include "6502.h"
 //#include "6522.h"
#include "6821.h"
#include "utils.h"

#if SERVER_TELNET == 1
#include "hw_tty.h"
HW_TTY hw_tty;
#endif

#if SERVER_WEB == 1
#include "hw_web.h"
HW_WEB hw_web;
#endif


#ifdef _MSC_VER    
#include <windows.h>
#include <conio.h>
#endif


#ifdef __MINGW32__
#include <conio.h>
#endif

#if defined(__linux__) || defined(__MINGW32__)
#include <fcntl.h>
#include <pthread.h> 
#else
#include <mutex> 
#endif

#if defined(__linux__) || defined(__MINGW32__)
#include <time.h>
#else
#include <chrono>
using namespace std::chrono;

#endif

#include <queue>
using namespace std;

#ifdef _MSC_VER    
mutex mtx_out;
condition_variable cv_out;

#else
pthread_mutex_t mtx_out;
#endif
queue<unsigned char> keyboard_queue;


static uint8_t ramrom[0x10000];	/* Covers the banked card */

static uint8_t fast = 0;
static uint8_t iopage = 0xFE;
static uint16_t addrinvert = 0x0000;

static uint16_t tstate_steps = 200;	/* 4MHz */

/* Who is pulling on the interrupt line */

static uint8_t live_irq;


#define IRQ_VIA		3
#define IRQ_PIA		33

//static struct via6522 *via;
static struct m6821 *pia;


static volatile int done;

#define TRACE_MEM	1
#define TRACE_IO	2
#define TRACE_IRQ	4
#define TRACE_UNK	8
#define TRACE_512	32
#define TRACE_CPU	128
#define TRACE_VIA	4096
#define TRACE_PIA	8192

static int trace = 0;



char convert_ascii(char key) {

	if (key == '@') key = 0x80;

	else if (key == 'a' || key == 'A') key = 0xC1;
	else if (key == 'b' || key == 'B') key = 0xC2;
	else if (key == 'c' || key == 'C') key = 0xC3;
	else if (key == 'd' || key == 'D') key = 0xC4;
	else if (key == 'e' || key == 'E') key = 0xC5;
	else if (key == 'f' || key == 'F') key = 0xC6;
	else if (key == 'g' || key == 'G') key = 0xC7;
	else if (key == 'h' || key == 'H') key = 0xC8;
	else if (key == 'i' || key == 'I') key = 0xC9;
	else if (key == 'j' || key == 'J') key = 0xCA;
	else if (key == 'k' || key == 'K') key = 0xCB;
	else if (key == 'l' || key == 'L') key = 0xCC;
	else if (key == 'm' || key == 'M') key = 0xCD;
	else if (key == 'n' || key == 'N') key = 0xCE;
	else if (key == 'o' || key == 'O') key = 0xCF;
	else if (key == 'p' || key == 'P') key = 0xD0;
	else if (key == 'q' || key == 'Q') key = 0xD1;
	else if (key == 'r' || key == 'R') key = 0xD2;
	else if (key == 's' || key == 'S') key = 0xD3;
	else if (key == 't' || key == 'T') key = 0xD4;
	else if (key == 'u' || key == 'U') key = 0xD5;
	else if (key == 'v' || key == 'V') key = 0xD6;
	else if (key == 'w' || key == 'W') key = 0xD7;
	else if (key == 'x' || key == 'X') key = 0xD8;
	else if (key == 'y' || key == 'Y') key = 0xD9;
	else if (key == 'z' || key == 'Z') key = 0xDA;

	else if (key == '[') key = 0x9B;
	else if (key == '\\') key = 0x9C;
	else if (key == ']') key = 0x9D;
	else if (key == '^') key = 0x9E;
	else if (key == '_') key = 0x9F;
	else if (key == ' ') key = 0xA0;
	else if (key == '!') key = 0xA1;
	else if (key == '"') key = 0xA2;
	else if (key == '#') key = 0xA3;
	else if (key == '$') key = 0xA4;
	else if (key == '%') key = 0xA5;
	else if (key == '&') key = 0xA6;
	else if (key == '\'') key = 0xA7;
	else if (key == '(') key = 0xA8;
	else if (key == ')') key = 0xA9;
	else if (key == '*') key = 0xAA;
	else if (key == '+') key = 0xAB;
	else if (key == ',') key = 0xAC;
	else if (key == '-') key = 0xAD;
	else if (key == '.') key = 0xAE;
	else if (key == '/') key = 0xAF;
	else if (key == '0') key = 0xB0;
	else if (key == '1') key = 0xB1;
	else if (key == '2') key = 0xB2;
	else if (key == '3') key = 0xB3;
	else if (key == '4') key = 0xB4;
	else if (key == '5') key = 0xB5;
	else if (key == '6') key = 0xB6;
	else if (key == '7') key = 0xB7;
	else if (key == '8') key = 0xB8;
	else if (key == '9') key = 0xB9;
	else if (key == ':') key = 0xBA;
	else if (key == ';') key = 0xBB;
	else if (key == '<') key = 0xBC;
	else if (key == '=') key = 0xBD;
	else if (key == '>') key = 0xBE;
	else if (key == '?') key = 0xBF;



	return key;
}

/* We do this in the 6502 loop instead. Provide a dummy for the device models */
void recalc_interrupts(void)
{
}

static void int_set(int src)
{
	live_irq |= (1 << src);
}

static void int_clear(int src)
{
	live_irq &= ~(1 << src);
}


/*
 *	6522 VIA support - we don't do anything with the pins on the VIA
 *	right now
 */

void via_recalc_outputs(struct via6522 *via)
{
}

void via_handshake_a(struct via6522 *via)
{
}

void via_handshake_b(struct via6522 *via)
{
}

void m6821_ctrl_change(struct m6821 *pia, uint8_t ctrl) {
}
uint8_t m6821_input(struct m6821 *pia, int port) {
	return 0;
}
void m6821_output(struct m6821 *pia, uint8_t data) {

	if (data > 0 && data < 0x7F && data != 0x1b) {
		if (data == 0xd)
			printf("%c", '\n');
		else
			printf("%c", data);


		if (data == 0xd) {
#if SERVER_TELNET == 1
			hw_tty.send('\r');
			hw_tty.send('\n');
#endif
#if SERVER_WEB == 1
			hw_web.new_char('\r');
			hw_web.new_char('\n');
#endif
		}
		else {
#if SERVER_TELNET == 1
			hw_tty.send(data);
#endif
#if SERVER_WEB == 1
			hw_web.new_char(data);
#endif
		}
	}
}
void m6821_strobe(struct m6821 *pia, int pin) {
}


uint8_t mmio_read_6502(uint16_t addr)
{
	if (trace & TRACE_IO)
		fprintf(stderr, "read %02x\n", addr);
	//if (addr >= 0x60 && addr <= 0x6F)
	//	return via_read(via, addr & 0x0F);

	if (addr >= 0xD000 && addr < 0xE000) {
		uint8_t val = m6821_read(pia, addr & 0x0F);
		//printf("R: %04X = %c (%02X)\n", addr, val, val);
		return val;
	}

	if (trace & TRACE_UNK)
		fprintf(stderr, "Unknown read from port %04X\n", addr);
	return 0xFF;
}

void mmio_write_6502(uint16_t addr, uint8_t val)
{
	if (trace & TRACE_IO)
		fprintf(stderr, "write %02x <- %02x\n", addr, val);
	//	else if (addr >= 0x60 && addr <= 0x6F)
	//		via_write(via, addr & 0x0F, val);

	if (addr >= 0xD000 && addr < 0xE000) {
		//printf("W: %04X = %c (%02X)\n", addr, val, val);
		m6821_write(pia, addr & 0x0F, val);
	}

	else if (addr == 0x00) {
		printf("trace set to %d\n", val);
		trace = val;
		if (trace & TRACE_CPU)
			log_6502 = 1;
		else
			log_6502 = 0;
	}
	else if (trace & TRACE_UNK)
		fprintf(stderr, "Unknown write to port %04X of %02X\n", addr, val);
}

/* Support emulating 32K/32K at some point */
uint8_t do_6502_read(uint16_t addr)
{
	//uint16_t xaddr = addr ^ addrinvert;
	//uint8_t ret = ramrom[xaddr & 0x3FFF];
	uint8_t val = ramrom[addr];

	/* When banking is off the entire 64K is occupied by repeats of ROM 0 */
	if (trace & TRACE_MEM)
		fprintf(stderr, "R %04X = %02X\n", addr, val);

	/*
		KBD.EQ     $D010           PIA.A keyboard input
		KBDCR.EQ     $D011           PIA.A keyboard control register
		DSP.EQ     $D012           PIA.B display output register
		DSPCR.EQ     $D013           PIA.B display control register

		; KBD b7..b0 are inputs, b6..b0 is ASCII input, b7 is constant high
		;     Programmed to respond to low to high KBD strobe
		; DSP b6..b0 are outputs, b7 is input
		;     CB2 goes low when data is written, returns high when CB1 goes high
		; Interrupts are enabled, though not used.KBD can be jumpered to IRQ,
		; whereas DSP can be jumpered to NMI.
	*/

	return val;
}

uint8_t read6502(uint16_t addr)
{
	uint8_t r;

	//if (addr >> 8 == iopage)
	if (addr >= 0xD000 && addr < 0xE000) {
		return mmio_read_6502(addr);
	}

	r = do_6502_read(addr);
	return r;
}

uint8_t read6502_debug(uint16_t addr)
{
	/* Avoid side effects for debug */
	if (addr >> 8 == iopage)
		return 0xFF;

	return do_6502_read(addr);
}


void write6502(uint16_t addr, uint8_t val)
{
	uint16_t xaddr = addr ^ addrinvert;

	if (addr >= 0xD000 && addr < 0xE000) {
		mmio_write_6502(addr, val);
		return;
	}

	else {
		if (trace & TRACE_MEM)
			fprintf(stderr, "W: %04X = %02X\n", addr, val);
		else if (trace & TRACE_MEM)
			fprintf(stderr, "[Discarded: ROM]\n");

		ramrom[addr] = val;

	}
}

static void poll_irq_event(void)
{
	/*
	if (via_irq_pending(via))
		int_set(IRQ_VIA);
	else
		int_clear(IRQ_VIA);
	*/

	if (m6821_irq_pending(pia))
		int_set(IRQ_PIA);
	else
		int_clear(IRQ_PIA);
}

static void irqnotify(void)
{
	if (live_irq)
		irq6502();
}




int main(int argc, char *argv[])
{
	char *filename = "apple1.rom";
	//printf("The filename to load is: %s\n", filename);

	FILE* f = fopen(filename, "rb");
	if (!f)
	{
		printf("Failed to open the file\n");
		printf("Press any key to continue");
		return -1;
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* buf = (char*)malloc(size * sizeof(char));

	int res = fread(buf, size, 1, f);
	if (res != 1)
	{
		printf("Failed to read from file");
		return -1;
	}


	///////////////////////////////////////////////////////////////////////////
/*
	int j = 0;
	for (j = 0; j < ZX80_MEMORY_SIZE; j++) {
		zx80_memory.memory[j] = 0x07;
	}
*/

//memcpy(z80->memory.memory, buf, size);
	memcpy(&ramrom[0xE000], buf, size);
	///////////////////////////////////////////////////////////////////////////

#if SERVER_TELNET == 1
	hw_tty.start_server(&keyboard_queue);
#endif

#if SERVER_WEB == 1
	hw_web.start_server(&keyboard_queue);
#endif



	if (trace & TRACE_CPU)
		log_6502 = 1;

	/*
	via = via_create();
	via_trace(via, trace & TRACE_VIA);
	*/

	pia = m6821_create();
	m6821_trace(pia, trace & TRACE_PIA);

	init6502();
	reset6502();
	hookexternal(irqnotify);

	/* This is the wrong way to do it but it's easier for the moment. We
	   should track how much real time has occurred and try to keep cycle
	   matched with that. The scheme here works fine except when the host
	   is loaded though */

	   /* We run 4000000 t-states per second */
	   /* We run 200 cycles per I/O check, do that 100 times then poll the
		  slow stuff and nap for 5ms. */
	while (!done) {
		int i;
		/* 36400 T states for base RC2014 - varies for others */
		for (i = 0; i < 100; i++) {
			/* FIXME: should check return and keep adjusting */
			exec6502(tstate_steps);

			//via_tick(via, tstate_steps);

		}

		if (kbhit()) {
			int key = getch();

#ifdef _MSC_VER    
			std::unique_lock<std::mutex> lock(mtx_out);
#else
			pthread_mutex_lock(&mtx_out);
#endif

			keyboard_queue.push(key);

#ifdef _MSC_VER    
			cv_out.notify_all();
#else
			pthread_mutex_unlock(&mtx_out);
#endif

			if (key == 0xe0)
				getch();
		}

		if (!keyboard_queue.empty()) {

			int key = keyboard_queue.front();
			keyboard_queue.pop();

			if (key == 0xe0 || key == 0x12)
			{
				reset6502();
				
				std::queue<unsigned char> empty;
				std::swap(keyboard_queue, empty);

				continue;
			}
			if (key == 0x1b)
				key = 0x9B;
			else if (key == 0xd)
				key = 0x8D;
			else if (key == 0x8)
				key = 0xDF;
			else
				key = convert_ascii(key);

			m6821_write(pia, 1, 0);
			m6821_write(pia, 0, key);
			m6821_write(pia, 1, 0xA4);
			m6821_write(pia, 0, key);

		}

		poll_irq_event();
	}
	exit(0);
}