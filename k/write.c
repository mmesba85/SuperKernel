#include <k/kstd.h>
#include <stdio.h>

#include "multiboot.h"
#include "io.h"
#include "write.h"


#define COM1 0x3f8

void init_serial()
{
	//disable all interrupts
	outb(COM1 + 1, 0x00);

	// no paritys
	outb(COM1 + 3, 0x80);

	//set divisor to 3
	outb(COM1 + 0, 0x03);
	outb(COM1 + 1, 0x00);

	// 8 bits length
   	outb(COM1 + 3, 0x03);

	// enable fifo, clear receive fifo, clear transmit fifo
	outb(COM1 + 2, 0xC7);
}

int write(const char *buf, size_t count)
{
	for(int i =0; buf[i]; i++)
		outb(COM1, buf[i]);
	return count;
}
