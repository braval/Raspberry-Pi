#include "raspi.h"
#include "define.h"
#include "timer.h"
#include "interrupts.h"
#include "serial.h"
#include "sdcard.h"
#define NEWLINE  print_s("\n\r")

unsigned int global_val = 10;

__attribute__((no_instrument_function))  void not_main(void)
{

    unsigned int ra;
    unsigned int local = 2;
    unsigned int local1;
    serial_init();
    print_s("Testing SD CARD INITIALIZATION\n\r");

    sd_init();

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


