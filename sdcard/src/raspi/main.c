#include "raspi.h"
#include "define.h"
#include "timer.h"
#include "interrupts.h"
#include "serial.h"
#include "sdcard.h"
#include "FAT32.h"
#include "malloc.h"

__attribute__((no_instrument_function))  void not_main(void)
{

    unsigned int ra;

    serial_init();
    print_s("Testing SD CARD INITIALIZATION\n\r");
    sd_init();

    FILE *fd;
    fd = fopen("TORNADO.JPG");

    unsigned char *test = (unsigned char*)malloc(sizeof(unsigned char)*6);
    fread(fd,test,6);
    for(int i=0;i<6;i++)
        print_ch(test[i]);


    free(test);
    free(fd);

    return;
}

void delay()
{
      register int r0;
      unsigned int ra;
      for(ra=0;ra<0x100000;ra++)
          __asm("mov r4,r4");
}



