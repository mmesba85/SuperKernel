#include <k/kstd.h>

#include "multiboot.h"
#include "io.h"
#include "idt.h"

/*void init_pic(void)
{

    outb(0x20, 0x11);
    outb(0xA0, 0x11);


    outb(0x21, 0x20);
    outb(0xA1, 0x28);


    outb(0x21, 0x04);
    outb(0xA1, 0x02);


    outb(0x21, 0x01);
    outb(0xA1, 0x01);


    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);


    outb(0x20, 0x20);
    outb(0xA0, 0x20);

    __asm__("sti\n");
}*/

void init_pic(void)
{
  /* ICW1 - begin initialization */
	outb(0x20 , 0x11);
	outb(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	outb(0x21 , 0x20);
	outb(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	outb(0x21 , 0x00);
	outb(0xA1 , 0x00);

	/* ICW4 - environment info */
	outb(0x21 , 0x01);
	outb(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	outb(0x21 , 0xff);
	outb(0xA1 , 0xff);
  outb(0x21 , 0xFD);
  __asm__("sti\n");

}
