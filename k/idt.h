#ifndef IDT_H
#define IDT_H

#define IDTSIZE 256

typedef struct registers
{
    u32 ds;                            
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
} registers_t;


struct idtdesc {
  u16 base_lo;
  u16 sel;
  u8  always0;
  u8  flags;
  u16 base_hi;

} __attribute__ ((packed));

struct idt_r {
    u16 limite;
    u32 base;
} __attribute__ ((packed));

void init_idt(void);
void init_pic(void);

#endif
