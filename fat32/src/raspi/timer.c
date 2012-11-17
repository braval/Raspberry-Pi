
#include "timer.h"

void timer_init(int seconds)
{
    PUT32(ARM_TIMER_CTL,0x003E0000);
    PUT32(ARM_TIMER_LOD,seconds*1000000-1);
    PUT32(ARM_TIMER_RLD,seconds*1000000-1);
    PUT32(ARM_TIMER_DIV,0x000000F9);
    PUT32(ARM_TIMER_CLI,0);
    PUT32(ARM_TIMER_CTL,0x003E00A2);
}
