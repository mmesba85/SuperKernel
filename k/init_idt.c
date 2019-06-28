#include <k/kstd.h>
#include <stdio.h>

#include "multiboot.h"
#include "io.h"
#include "keyboard.h"
#include "idt.h"
#include "timer.h"

long tick = 0;

struct idt_r idtr;
struct idtdesc idt[IDTSIZE];

void idt_set_gate(int num, u32 base, u16 sel, u8 flags)
{
  idt[num].base_lo = base & 0xFFFF;
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].sel     = sel;
  idt[num].always0 = 0;
  idt[num].flags   = flags;
}

void init_idt(void)
{
    idtr.base = (u32)idt;
    idtr.limite = sizeof(idt) - 1;

    // isr entries
    idt_set_gate( 0, (u32)isr0 , 0x08, 0x8E);
    idt_set_gate( 1, (u32)isr1 , 0x08, 0x8E);
    idt_set_gate( 2, (u32)isr2, 0x08, 0x8E);
    idt_set_gate( 3, (u32)isr3, 0x08, 0x8E);
    idt_set_gate( 4, (u32)isr4, 0x08, 0x8E);
    idt_set_gate( 5, (u32)isr5, 0x08, 0x8E);
    idt_set_gate( 6, (u32)isr6, 0x08, 0x8E);
    idt_set_gate( 7, (u32)isr7, 0x08, 0x8E);
    idt_set_gate( 8, (u32)isr8, 0x08, 0x8E);
    idt_set_gate( 9, (u32)isr9, 0x08, 0x8E);
    idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32)isr31, 0x08, 0x8E);


    // IRQ entries
    idt_set_gate(32, (u32)irq0, 0x08, 0x8E);
    idt_set_gate(33, (u32)irq1, 0x08, 0x8E);
    idt_set_gate(34, (u32)irq2, 0x08, 0x8E);
    idt_set_gate(35, (u32)irq3, 0x08, 0x8E);
    idt_set_gate(36, (u32)irq4, 0x08, 0x8E);
    idt_set_gate(37, (u32)irq5, 0x08, 0x8E);
    idt_set_gate(38, (u32)irq6, 0x08, 0x8E);
    idt_set_gate(39, (u32)irq7, 0x08, 0x8E);
    idt_set_gate(40, (u32)irq8, 0x08, 0x8E);
    idt_set_gate(41, (u32)irq9, 0x08, 0x8E);
    idt_set_gate(42, (u32)irq10, 0x08, 0x8E);
    idt_set_gate(43, (u32)irq11, 0x08, 0x8E);
    idt_set_gate(44, (u32)irq12, 0x08, 0x8E);
    idt_set_gate(45, (u32)irq13, 0x08, 0x8E);
    idt_set_gate(46, (u32)irq14, 0x08, 0x8E);
    idt_set_gate(47, (u32)irq15, 0x08, 0x8E);

    //load
    __asm__ volatile("lidt %0\n"
      : /* no output */
      : "m" (idtr)
      : "memory");
}

int getkey(void) {
  unsigned char status;
	int keycode;
  outb(0x20, 0x20);
  status = inb(KEYBOARD_STATUS_PORT);
	if (status & 0x01) {
		keycode = inb(KEYBOARD_DATA_PORT);
		if(keycode < 0)
			return -1;
		char c = keyboard_map[keycode];
    printf("%c", c);
    return c;
	}
  return -1;
}

long gettick()
{
  return tick;
}

void generate_c_handler(registers_t regs)
{
    if(regs.int_no < 32)
        printf("Exception. Code: %d", regs.int_no);
    else {
      tick++;
      if (regs.int_no - 32 == 0)
        gettick();
      else if (regs.int_no - 32 == 1)
        getkey();
      outb(0x20, 0x20);
    }
}
