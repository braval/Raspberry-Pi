// To initialize the SD Card/
#include "sdcard.h"
#include "serial.h"
#include "raspi.h"
#include "define.h"
void wait()
{
    unsigned int ra;
    for(ra=0;ra<150000;ra++)
          dummy(ra);
}

void sd_init()
{
    unsigned int m;
    unsigned int test;
    PUT32(CONTROL0,0x00100000);
    PUT32(CONTROL1,0x070F0F27);
    PUT32(CONTROL2,0x00000000);

    do
    {
        m = GET32(CONTROL1);
        test = m & 0x10000000;
    }while(test!=0);

    for(int a =1; a<5; a++)
        wait();

    PUT32(CONTROL0,0x00100000);
    //PUT32(CONTROL1,0x000F0F27);
    PUT32(CONTROL1,0x00071007);
    PUT32(CONTROL2,0x00000000);
    PUT32(IRPT_MASK,0xFFFFFFFF);
    wait();
    wait();
    do
    {
        m = GET32(CONTROL1);
        test = m &  0x00000002;
    }while(test!=2);


    //GO_IDLE_STATE
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x00000010);
    for(int a =1; a<5; a++)
        wait();

    //SD_SEND_IF_COND
    PUT32(ARG1,0x000001AA);
    PUT32(CMDTM,0x08030010);
    for(int a =1; a<5; a++)
        wait();

    unsigned int check;
    do
    {
    //APP_CMD
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x37020000);
    for(int a =1; a<10; a++)
        wait();


        //SD_SEND_OP_COND (repeat until Powerup bit is set) stops here, always 0xff80000 returned
        PUT32(ARG1,0x40FF0000);
        PUT32(CMDTM,0x29020000);
        for(int a =1; a<10; a++)
        wait();
        check = GET32(RESP0);
        check = check & 0x80000000;
        for(int a =1; a<10; a++)
        wait();
    }while(check == 0);

    print_s("\n\rResponse from ACMD41 ");
    hexstrings(GET32(RESP0));
    print_s("\n\rCard Identification number ");
    //ALL_SEND_CID
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x02020000);
    for(int a =1; a<5; a++)
        wait();
    hexstrings(GET32(RESP0));

    print_s("\n\rRelative address ");
    //SEND_RELATIVE_ADDRESS
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x03020000);
    for(int a =1; a<5; a++)
        wait();
    check = GET32(RESP0);
    hexstrings(check);

    //SEND_CSD
    PUT32(ARG1,check);
    PUT32(CMDTM,0x09020000);
    for(int a =1; a<4; a++)
        wait();
    print_s("\n\rCard Specific Data ");
    hexstrings(GET32(RESP0));

    //SELCT_CARD
    PUT32(ARG1,check);
    PUT32(CMDTM,0x07020000);
    for(int a =1; a<4; a++)
        wait();
    print_s("\n\rSelected SD Card ");
    hexstrings(GET32(RESP0));

    /*
    //SET_BLOCKLEN
    PUT32(ARG1,0x00000200);
    PUT32(CMDTM,0x10020000);
    for(int a =1; a<4; a++)
        wait();
    print_s("\n\rSetting block length ");
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
    */

}


void sd_block_write(unsigned int address)
{

    PUT32(BLOCKSIZECNT, (1<<16)|512);

    PUT32(ARG1,0x00000005);
    PUT32(CMDTM,0x18230000);
    for(int a =1; a<4; a++)
        wait();
    print_s("\n\rWriting single block ");
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));

    int num_written = 0;
    unsigned int data = 0x00000044;
    while(num_written<128)
    {
        PUT32(DATA,data);
        num_written++;
        data++;
    }

    print_s("\n\rDone writing ");
    for(int a =1; a<2; a++)
        wait();
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
}

unsigned int sd_block_read(unsigned int address)
{


    PUT32(ARG1,0x00000005);
    PUT32(CMDTM,0x11230010);
    for(int a =1; a<4; a++)
        wait();

    print_s("\n\rReading single block ");
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));

    int num_read = 0;
    while(num_read<128)
    {
        print_s("\n\r");
        hexstrings(GET32(DATA));
        num_read ++;
    }


    print_s("\n\rDone reading ");
    for(int a =1; a<2; a++)
        wait();
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));

}
