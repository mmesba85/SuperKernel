#include <k/kstd.h>

#include "multiboot.h"
#include "io.h"

#include "gdt.h"


// build GDT
void init_gdtdesc(u32 base, u32 limite, u8 acces, u8 other, struct gdtdesc *d)
{
  d->lim0_15 = (limite & 0xffff);
  d->base0_15 = (base & 0xffff);
  d->base16_23 = (base & 0xff0000) >> 16;
  d->acces = acces;
  d->lim16_19 = (limite & 0xf0000) >> 16;
  d->other = (other & 0xf);
  d->base24_31 = (base & 0xff000000) >> 24;
}

struct gdt_r gdtr;
struct gdtdesc gdt[5];


// load GDT
void load(void)
{
  __asm__("cli\n");
  init_gdtdesc(0x0, 0x0, 0x0, 0x0, &gdt[0]);
  init_gdtdesc(0x0, 0xfffff, 0x9b, 0x0d, &gdt[1]); /* kernel code */
  init_gdtdesc(0x0, 0xfffff, 0x93, 0x0d, &gdt[2]); /* kernel data */
  init_gdtdesc(0x0, 0xfffff, 0xff, 0x0d, &gdt[3]);    /* user code */
  init_gdtdesc(0x0, 0xfffff, 0xF3, 0x0d, &gdt[4]);   /* user data */
  gdtr.base = (u32)gdt;
  gdtr.limit = sizeof(gdt) - 1;
    asm volatile("lgdt %0\n"
    : /* no output */
    : "m" (gdtr)
    : "memory");
}

void switch_to_protected(void)
{
  __asm__(
	    "mov %eax, %cr0\n"
	    "or %eax, 1\n"
	    "mov %cr0, %eax\n"
	    "movw $0x10, %ax\n"
	    "movw %ax, %ds\n"
	    "movw %ax, %fs\n"
	    "movw %ax, %gs\n"
	    "movw %ax, %ss\n"
      "movw %ax, %es\n"
            "pushl $0x08\n"
            "pushl $1f\n"
            "lret\n"
            "1:\n"
	  );
}
