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
    ra&=~(7<<15); //gpio15
    ra|=2<<15;    //alt5

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




void hexstrings ( unsigned int d )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        print_ch(rc);
        if(rb==0) break;
    }
    print_ch(0x20);
}


void hexstring ( unsigned int d )
{
    hexstrings(d);
    print_ch(0x0D);
    print_ch(0x0A);
}
