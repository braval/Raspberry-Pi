
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
extern unsigned int GET32 ( unsigned int );
extern void PUT32 ( unsigned int, unsigned int );
// For using the GPIO's
#define GPFSEL1 0x20200004   // Corresponds to the set of register which controls function of GPIO's where OK LED belongs (GPIO 16)
#define GPSET0  0x2020001C   // Sets the OK LED
#define GPCLR0  0x20200028   // Clears the OK LED

// For using the ARM timer
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CNT 0x2000B420

#define DEADLINE 4000000
//-------------------------------------------------------------------------



int notmain ( void )
{

    unsigned int ra;
    unsigned int rb;
    ra=GET32(GPFSEL1);    
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);       // Sets the OK LED GPIO to output

    PUT32(ARM_TIMER_CTL,0x00F90000);
    PUT32(ARM_TIMER_CTL,0x00F90200); 
    
    rb = GET32(ARM_TIMER_CNT);
    while(1)
    {
        PUT32(GPSET0,1<<16);   // Sets the LED
        while(1)
        {
            ra = GET32(ARM_TIMER_CNT);
            if((ra-rb)>=DEADLINE)
                break;
        }
        rb = rb + DEADLINE;
        PUT32(GPCLR0,1<<16);   // Clears the LED
        while(1)
        {
            ra = GET32(ARM_TIMER_CNT);
            if((ra-rb)>=DEADLINE)
                break;
        }
        rb = rb + DEADLINE;
    }

    return(0);
}


