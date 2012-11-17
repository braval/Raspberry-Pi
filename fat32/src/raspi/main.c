#include "raspi.h"
#include "define.h"
#include "timer.h"
#include "interrupts.h"
#include "serial.h"
#include "sdcard.h"
#include "malloc.h"
#include "fat_filelib.h"
#include "stdio.h"


unsigned int global_val = 10;

int media_read(unsigned long sector, unsigned char *buffer, unsigned long sector_count)
{
    unsigned long i;

    for (i=0;i<sector_count;i++)
    {
        // ...
        // Add platform specific sector (512 bytes) read code here
        unsigned char *temp = sd_block_read(sector);
        for(int o=0;o<512;o++)
            buffer[o] = temp[o];

        sector ++;
        buffer += 512;
    }

    return 1;
}

int media_write(unsigned long sector, unsigned char *buffer, unsigned long sector_count)
{
    unsigned long i;

    for (i=0;i<sector_count;i++)
    {
        // ...
        // Add platform specific sector (512 bytes) write code here
        sd_block_write(sector,buffer);
        sector ++;
        buffer += 512;
    }

    return 1;
}


void gpio_setup()
{
    unsigned int ra;
    PUT32(IRQ_DISABLE_BASIC,1);
    /* Selecting the GPIO Pin for OK LED and setting it to ouput */
    ra=GET32(GPFSEL1);
    ra&=~(7<<18);
    ra|=1<<18;
    PUT32(GPFSEL1,ra);
    PUT32(GPSET0,1<<16);
}
__attribute__((no_instrument_function))  void not_main(void)
{

    unsigned int ra;
    unsigned int local = 2;
    unsigned int local1;
    serial_init();
    print_s("Testing SD CARD INITIALIZATION\n\r");
    sd_init();
    FL_FILE *file;

    // Initialise File IO Library
    fl_init();

    // Attach media access functions to library
    if (fl_attach_media(media_read, media_write) != FAT_INIT_OK)
    {
        print_s("\n\rERROR: Media attach failed");
    }
    else
        print_s("\n\rMedia attached successfully");



    // Create File
    file = fl_fopen("/file.bin", "w");
    if (file)
    {
        print_s("\n\rFile Opened");
    }
    else
        print_s("\n\rFile Failed To Open");



    gpio_setup();
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


