#include <k/kstd.h>

#include "multiboot.h"
#include "io.h"
#include "idt.h"

void init_pic(void)
{
    /* Initialization of ICW1 */
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    /* Initialization of ICW2 */
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    /* Initialization of ICW3 */
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    /* Initialization of ICW4 */
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* Keyboard */
    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);

    /* mask interrupts */
    outb(0x20, 0x20);
    outb(0xA0, 0x20);

    __asm__("sti\n");
}
