#include "raspi.h"
#include "define.h"
#include "timer.h"
#include "interrupts.h"
#include "serial.h"
#include "stdlib.h"
#include "stdio.h"
#include "malloc.h"

__attribute__((no_instrument_function)) int not_main(void)
{
    
    unsigned int ra;
    serial_init();
    printf("\n\rTesting UART and Timer INT\n\r");
    
    int size;
    printf("Enter the size of malloc to be allocated \n\r ");
    scanf("%d",&size);

    unsigned int *ptr = (unsigned int*)malloc(size);
    if(ptr == NULL)
        printf("Malloc allocation failed\n\r");
    printf("Address of malloc if size %d  allocated 0x%x \n\r", size,ptr); 

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
    printf("See the OK LED blinking\n\r");
    while(1)continue;
    return(0);
} 

void delay()
{
      register int r0;
      unsigned int ra;
      for(ra=0;ra<0x100000;ra++)
          __asm("mov r4,r4");
}



