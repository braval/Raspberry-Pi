#include "serial.h"
#include "define.h"
#include "raspi.h"
void serial_init()
{
    unsigned int ra;

    PUT32(AUX_ENABLES,1);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_CNTL_REG,0);
    PUT32(AUX_MU_LCR_REG,3);
    PUT32(AUX_MU_MCR_REG,0);
    PUT32(AUX_MU_IER_REG,0);
    PUT32(AUX_MU_IIR_REG,0xC6);
    PUT32(AUX_MU_BAUD_REG,270);

    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=2<<12;    //alt5
    PUT32(GPFSEL1,ra);
    PUT32(GPPUD,0);

    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++) dummy(ra);
    PUT32(GPPUDCLK0,0);

    PUT32(AUX_MU_CNTL_REG,3);
}

void print_s(char *message)
{
    char *s;
    for(s=message;*s;++s) 
    {
        print_ch(*s);
    }
}

void print_ch(char ch)
{
    while((GET32(AUX_MU_LSR_REG)&0x20) == 0);
    PUT32(AUX_MU_IO_REG,ch);
}

char get_char()
{
    unsigned int ra;
    while(1)
    {
        if(GET32(AUX_MU_LSR_REG)&0x01) break;
    }
    ra=GET32(AUX_MU_IO_REG);
    PUT32(AUX_MU_IO_REG,ra);
    return ra;
}


