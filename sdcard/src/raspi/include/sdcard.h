#define ARG2          0x20300000
#define BLOCKSIZECNT  0x20300004
#define ARG1          0x20300008
#define CMDTM         0x2030000C
#define CONTROL0      0x20300028
#define CONTROL1      0x2030002C
#define CONTROL2      0x2030003C
#define IRPT_MASK     0x20300034
#define RESP0         0x20300010
#define RESP1         0x20300014
#define RESP2         0x20300018
#define RESP3         0x2030001c
#define DATA          0x20300020
#define STATUS        0x20300024
#define INTERRUPT     0x20300030


struct _FILE
{
    unsigned int reservedSectorCount;
    unsigned int sectorPerFAT;
    unsigned int hiddenSectors;
    unsigned int totalSectors_F32;
    unsigned char noFATs;
    unsigned char sectorPerCluster;
    unsigned int maxRootEntries;
    unsigned int file_location;
    unsigned int root_dir_add;
    unsigned int file_offset;
};
typedef struct _FILE FILE;


void sd_init();
void sd_block_write(unsigned int sector, unsigned char *buffer);
unsigned char*  sd_block_read(unsigned int sector);
int list_root_directory();
FILE* fopen(char *s);
int fread(FILE *file,unsigned char *buffer, int size);
int fwrite(FILE *file,unsigned char *buffer, int size);
int fdelete(FILE *file);

