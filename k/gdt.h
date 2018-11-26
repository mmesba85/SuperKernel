#ifndef GDT_H
#define GDT_H

struct gdtdesc {
    u16 lim0_15;
    u16 base0_15;
    u8 base16_23;
    u8 acces;
    u8 lim16_19:4;
    u8 other:4;
    u8 base24_31;
} __attribute__ ((packed));

struct gdt_r {
    u16 limit;
    u32 base;
}__attribute__((packed));

void load(void);
void switch_to_protected(void);

#endif
