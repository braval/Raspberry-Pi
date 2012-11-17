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


void sd_init();
void sd_block_write(unsigned int,unsigned char*);
unsigned char*  sd_block_read(unsigned int);

