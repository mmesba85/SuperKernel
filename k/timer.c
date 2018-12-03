#include "io.h"
#include "timer.h"

void init_timer()
{
    u32 divisor = FREQ / 100;

    outb(CONTROL_REG, 0x36);

    u8 low = (u8)(divisor & 0xFF);
    u8 high = (u8)((divisor >> 8) & 0xFF);

    outb(COUNTER0, low);
    outb(COUNTER0, high);
    outb(0x20, 0x20);
}
