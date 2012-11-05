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

    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    print_s("Done resetting\n\r");

    PUT32(CONTROL0,0x00100000);
    PUT32(CONTROL1,0x000F0F27);
    PUT32(CONTROL2,0x00000000);
    PUT32(IRPT_MASK,0xFFFFFFFF);
    wait();
    wait();
    do
    {
        m = GET32(CONTROL1);
        test = m &  0x00000002;
    }while(test!=2);

    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    wait();
    print_s("Done clock setting\n\r");


    //GO_IDLE_STATE
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x00000010);
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    print_s("GO_IDLE_STATE DONE\n\r");


    //SD_SEND_IF_COND
    PUT32(ARG1,0x000001AA);
    PUT32(CMDTM,0x08030010);
    wait();
    wait();
    wait();
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
    print_s("SD_SEND_IF_COND DONE\n\r");


    unsigned int check;
    do
    {
    //APP_CMD
    print_s("\n\rCMD55  \n\r");
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x37020000);
    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    for(int a =1; a<10; a++)
        wait();
    check = GET32(RESP0);
    hexstrings(check);

    hexstrings(GET32(RESP1));
    hexstrings(GET32(RESP2));
    hexstrings(GET32(RESP3));



        print_s("\n\rCMD41\n\r");
        //SD_SEND_OP_COND (repeat until Powerup bit is set) stops here, always 0xff80000 returned
        PUT32(ARG1,0x40FF0000);
        PUT32(CMDTM,0x29020000);
        for(int a =1; a<10; a++)
        wait();
        hexstrings(GET32(STATUS));
        hexstrings(GET32(INTERRUPT));
        check = GET32(RESP0);
        hexstrings(check);
        hexstrings(GET32(RESP1));
        hexstrings(GET32(RESP2));
        hexstrings(GET32(RESP2));
        check = check & 0x80000000;
        for(int a =1; a<10; a++)
        wait();
    }while(check == 0);

    print_s("CID\n\r");

    //ALL_SEND_CID
    PUT32(ARG1,0x00000000);
    PUT32(CMDTM,0x02020000);
        for(int a =1; a<10; a++)
        wait();

    hexstrings(GET32(STATUS));
    hexstrings(GET32(INTERRUPT));
    hexstrings(GET32(RESP0));
    hexstrings(GET32(RESP1));
    hexstrings(GET32(RESP2));
    hexstrings(GET32(RESP3));


}
