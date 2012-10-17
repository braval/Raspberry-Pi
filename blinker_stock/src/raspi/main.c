#include "raspi.h"
#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CLI 0x2000B40C
#define ARM_TIMER_RIS 0x2000B410
#define ARM_TIMER_MIS 0x2000B414
#define ARM_TIMER_RLD 0x2000B418
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420

#define SYSTIMERCLO 0x20003004
#define GPFSEL1 0x20200004
#define GPSET0  0x2020001C
#define GPCLR0  0x20200028

#define IRQ_BASIC 0x2000B200
#define IRQ_PEND1 0x2000B204
#define IRQ_PEND2 0x2000B208
#define IRQ_FIQ_CONTROL 0x2000B210
#define IRQ_ENABLE_BASIC 0x2000B218
#define IRQ_DISABLE_BASIC 0x2000B224


extern void enable_irq ( void );
__attribute__((no_instrument_function))  void not_main(void)
{
   UINT32 sel = GET32(GPFSEL1);
   sel &= ~(0b111 << 18);
   sel |= (0b001 << 18);
   PUT32(GPFSEL1,sel);
  
   while(1) 
   {
      PUT32(GPCLR0, 1<<16);
      for (int i = 0; i < 0x100000; i++) ;
      PUT32(GPSET0, 1<<16);
      for (int i = 0; i < 0x100000; i++) ;
   }
} 
volatile unsigned int icount;

//-------------------------------------------------------------------------
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
//-------------------------------------------------------------------------
