// To initialize the SD Card/
#include "sdcard.h"
#include "serial.h"
#include "raspi.h"
#include "define.h"
#include "malloc.h"
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


void sd_block_write(unsigned int sector,unsigned char *buffer)
{

    PUT32(BLOCKSIZECNT, (1<<16)|512);

    PUT32(ARG1,sector);
    PUT32(CMDTM,0x18230000);
    for(int a =1; a<4; a++)
        wait();
    print_s("\n\rWriting single block ");
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));

    int num_written = 0;
    while(num_written<512)
    {
        PUT32(DATA,buffer[num_written]);
        num_written++;
    }

    print_s("\n\rDone writing ");
    for(int a =1; a<2; a++)
        wait();
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
}

unsigned char*  sd_block_read(unsigned int sector)
{

    PUT32(BLOCKSIZECNT, (1<<16)|512);

    PUT32(ARG1,sector);
    PUT32(CMDTM,0x11230010);
    for(int a =1; a<4; a++)
        wait();

    unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char)*512);

    /*
    print_s("\n\rReading single block ");
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
    */

    int num_read = 0;
    int new_data = 0;
    int temp;

    while(num_read<512)
    {
        new_data = GET32(DATA);
        buffer[num_read + 3 ]     = (new_data >>24) & 0xFF;
        buffer[num_read + 2] = (new_data >>16) & 0xFF;
        buffer[num_read + 1] = (new_data >> 8)  & 0xFF;
        buffer[num_read] = new_data & 0xFF;
        num_read = num_read + 4;
    }


    /*
    print_s("\n\rDone reading ");
    for(int a =1; a<2; a++)
        wait();
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
    */
    return buffer;
}
