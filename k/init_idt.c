#include <k/kstd.h>
#include <stdio.h>

#include "multiboot.h"
#include "io.h"

#include "idt.h"

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0 (void);
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();


// IRQ Handlers
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

struct idtdesc idt[IDTSIZE];
struct idt_r idtr;



void idt_set_gate(int num, u32 base, u16 sel, u8 flags)
{
  idt[num].base_lo = base & 0xFFFF;
  idt[num].base_hi = (base >> 16) & 0xFFFF;
  idt[num].sel     = sel;
  idt[num].always0 = 0;
// We must uncomment the OR below when we get to using user-mode.
// It sets the interrupt gate's privilege level to 3.
  idt[num].flags   = flags /* | 0x60 */;
}

void init_idt(void)
{
    idtr.base = (u32)idt; /* idt base address */
    idtr.limite = sizeof(idt) - 1; /* idt size - 1 */

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

    __asm__ volatile("lidt %0\n"
      : /* no output */
      : "m" (idtr)
      : "memory");
}

void generate_c_handler(registers_t regs)
{
    if(regs.err_code < 32)
    {
        printf("Exception. Code: %d", regs.err_code);
    }
}

void irq_handler(registers_t regs)
{
    //If int_no >= 40, we must reset the slave as well as the master
    if(regs.int_no >= 40)
    {
        printf("helloooooooooo");
    }
}
