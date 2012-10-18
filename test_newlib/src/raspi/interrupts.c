#include "interrupts.h"
#include "timer.h"



void c_irq_handler ( void )
{
        icount++;
            if(icount&1)
                    {
                                PUT32(GPCLR0,1<<16);
                                    }
                else
                        {
                                    PUT32(GPSET0,1<<16);
                                         }
                    PUT32(ARM_TIMER_CLI,0);
}

