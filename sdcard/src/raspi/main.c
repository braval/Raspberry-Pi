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
    print_s("\n\rTesting SD CARD INITIALIZATION\n\r");
    sd_init();


   // list_root_directory();
    FILE *fd;
    fd = fopen("TORNADO.JPG");

    fwrite(fd,"Yaaayie",10);
    unsigned char *test = (unsigned char*)malloc(sizeof(unsigned char)*6);
    fread(fd,test,10);
    print_s("\n\rContents: ");
    for(int i=0;i<10;i++)
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



