
#include "define.h"
#include "raspi.h"

#define ARM_TIMER_LOD 0x2000B400
#define ARM_TIMER_VAL 0x2000B404
#define ARM_TIMER_CTL 0x2000B408
#define ARM_TIMER_CLI 0x2000B40C
#define ARM_TIMER_RIS 0x2000B410
#define ARM_TIMER_MIS 0x2000B414
#define ARM_TIMER_RLD 0x2000B418
#define ARM_TIMER_DIV 0x2000B41C
#define ARM_TIMER_CNT 0x2000B420
#define DEADLINE 4000000

void timer_init(int seconds);
