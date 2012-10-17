#include "raspi.h"
#include "define.h"
#include "timer.h"
#include "interrupts.h"
#include "serial.h"

__attribute__((no_instrument_function))  void not_main(void)
{
    
    unsigned int ra;
    serial_init();
    char *message = "\nTesting Serial Port - Mini UART Raspi \n\r";
    char *s;
    for(s=message;*s;++s) 
    {
        while((GET32(AUX_MU_LSR_REG)&0x20) == 0);
        PUT32(AUX_MU_IO_REG,*s);
    }


    PUT32(IRQ_DISABLE_BASIC,1);
    
    /* Selecting the GPIO Pin for OK LED and setting it to ouput */
    ra=GET32(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);
    PUT32(GPSET0,1<<16);
    timer_init(3);

    PUT32(IRQ_ENABLE_BASIC,1);
    enable_irq();
    while(1)continue;
    return;
} 

void delay()
{
      register int r0;
      unsigned int ra;
      for(ra=0;ra<0x100000;ra++)
          __asm("mov r4,r4");
}



