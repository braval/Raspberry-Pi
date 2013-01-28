// To initialize the SD Card/
#include "sdcard.h"
#include "serial.h"
#include "raspi.h"
#include "define.h"
#include "timer.h"
#include "malloc.h"
void wait()
{
    unsigned int ra;
    for(ra=0;ra<150000;ra++)
          dummy(ra);
}



unsigned char buffer[512];
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
    print_s("\n\rSelected SD Cardr ");
    hexstrings(GET32(RESP0));
    print_s("\n\r");
}


void sd_block_write(unsigned int sector,unsigned char *buffer)
{

    unsigned int ra, rb;

    PUT32(BLOCKSIZECNT, (1<<16)|512);

    PUT32(ARG1,sector);
    PUT32(CMDTM,0x18230000);

    PUT32(ARM_TIMER_CTL,0x00F90000);
    PUT32(ARM_TIMER_CTL,0x00F90200);
    rb = GET32(ARM_TIMER_CNT);

    while(1)
    {
        ra = GET32(ARM_TIMER_CNT);
        if((ra-rb)>=500000)
            break;
    }

    int num_written = 0;
    int check;
    while(num_written<512)
    {
        PUT32(DATA,buffer[num_written] +(int)(buffer[num_written + 1]<<8) +(int)(buffer[num_written + 2]<<16) + (int)(buffer[num_written+3]<<24));
        num_written = num_written + 4;
    }

    rb = GET32(ARM_TIMER_CNT);

    while(1)
    {
        ra = GET32(ARM_TIMER_CNT);
        if((ra-rb)>=500000)
            break;
    }
}

unsigned char*  sd_block_read(unsigned int sector)
{

    PUT32(BLOCKSIZECNT, (1<<16)|512);

    PUT32(ARG1,sector);
    PUT32(CMDTM,0x11230010);

    unsigned int ra,rb;
    PUT32(ARM_TIMER_CTL,0x00F90000);
    PUT32(ARM_TIMER_CTL,0x00F90200);
    rb = GET32(ARM_TIMER_CNT);

    while(1)
    {
        ra = GET32(ARM_TIMER_CNT);
        if((ra-rb)>=500000)
            break;
    }
   // unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char)*512);


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

    rb = GET32(ARM_TIMER_CNT);
    while(1)
    {
        ra = GET32(ARM_TIMER_CNT);
        if((ra-rb)>=500000)
            break;
    }

    return buffer;
}

int list_root_directory()
{
    FILE *file;
    file = (FILE *)malloc(sizeof(FILE));
    unsigned char *boot_sector_record = sd_block_read(0x2000);

    file->reservedSectorCount = (int)(boot_sector_record[0x0F] << 8) + boot_sector_record[0x0E];
    file->sectorPerFAT =  (int)(boot_sector_record[0x27] << 24) +(int)(boot_sector_record[0x26] << 16)+ (int)(boot_sector_record[0x25] <<8) +(int)boot_sector_record[0x24];
    file->hiddenSectors = (int)(boot_sector_record[0x1F] << 24) + (int)(boot_sector_record[0x1E]<<16) + (int) (boot_sector_record[0x1D]<<8) + (int)(boot_sector_record[0x1C]);
    file->totalSectors_F32 = (int)(boot_sector_record[0x14] << 8) + boot_sector_record[0x13];
    file->noFATs = boot_sector_record[0x10];
    file->sectorPerCluster = boot_sector_record[0x0D];
    file->maxRootEntries = (int)(boot_sector_record[0x12] << 8) + boot_sector_record[0x11];

    unsigned int root_dir_add = file->hiddenSectors + file->reservedSectorCount + (file->noFATs * file->sectorPerFAT );
    unsigned char *root_dir = sd_block_read(root_dir_add);

    print_s("Volume name : ");
    for(int i=0;i<11;i++)
        print_ch(root_dir[i]);
    print_s("\n\r");

    for(int j=32; j<=256; j+=32 )
    {
        for(int i=0;i<11;i++)
        {
            if(i==7)
                print_ch('.');
            else
            {
                if(root_dir[j+i] != 0x20)
                    print_ch(root_dir[j+i]);
            }
        }
        print_s("\n\r");
    }

    free(root_dir);


}

FILE* fopen(char *file_name)
{
    FILE *file = (FILE *)malloc(sizeof(FILE));
    unsigned char *boot_sector_record = sd_block_read(0x2000);

    file->reservedSectorCount = (int)(boot_sector_record[0x0F] << 8) + boot_sector_record[0x0E];
    file->sectorPerFAT =  (int)(boot_sector_record[0x27] << 24) +(int)(boot_sector_record[0x26] << 16)+ (int)(boot_sector_record[0x25] <<8) +(int)boot_sector_record[0x24];
    file->hiddenSectors = (int)(boot_sector_record[0x1F] << 24) + (int)(boot_sector_record[0x1E]<<16) + (int) (boot_sector_record[0x1D]<<8) + (int)(boot_sector_record[0x1C]);
    file->totalSectors_F32 = (int)(boot_sector_record[0x14] << 8) + boot_sector_record[0x13];
    file->noFATs = boot_sector_record[0x10];
    file->sectorPerCluster = boot_sector_record[0x0D];
    file->maxRootEntries = (int)(boot_sector_record[0x12] << 8) + boot_sector_record[0x11];

    file->root_dir_add = file->hiddenSectors + file->reservedSectorCount + (file->noFATs * file->sectorPerFAT );
    unsigned char *root_dir = sd_block_read(file->root_dir_add);

    int temp,i;
    for(int k=32;k<1024;k+=32)
    {
        temp = k;
        for(i=0;i<5;i++)
            if(root_dir[k+i]!=file_name[i])
                break;
        if(i==5)
            break;
    }

    file->file_offset = temp;
    print_s("Opened file : ");
    for(int i=0;i<11;i++)
    {
        if(i==7)
            print_ch('.');
        else
        {
            if(root_dir[temp+i] != 0x20)
                print_ch(root_dir[temp+i]);
        }
    }
    print_s("\n\r");
    unsigned int lower_cluster,higher_cluster;
    lower_cluster = (int)(root_dir[temp + 0x1B]<<8) + root_dir[temp + 0x1A] ;
    higher_cluster = root_dir[temp + 0x14];

    file->file_location = file->root_dir_add + ((lower_cluster - 2) * 8);
    print_s("\n\r");

    /*
    for(int i=0;i<512;i++)
    {
        if(i%16==0)
        {
            print_s("\n\r");
            hexstrings(i);
        }
        hexstrings(root_dir[i]);
    }
    */
    free(root_dir);
    return file;
}

int fread(FILE *file, unsigned char *out, int size)
{
    unsigned char *temp_read = sd_block_read(file->file_location);

    for(int i=0;i<size;i++)
    {
        out[i] = temp_read[i];
    }

    free(temp_read);


}

int fwrite(FILE *file, unsigned char *buffer, int size)
{
    unsigned char *root_dir_temp = sd_block_read(file->root_dir_add);
    root_dir_temp[file->file_offset +0x1C] = size;
    sd_block_write(file->root_dir_add,root_dir_temp);

    unsigned char *file_data = sd_block_read(file->file_location);
    for(int i=0;i<size;i++)
        file_data[i] = buffer[i];

    sd_block_write(file->file_location,file_data);

}
