#include "define.h"
#include "raspi.h"

#define IRQ_BASIC 0x2000B200
#define IRQ_PEND1 0x2000B204
#define IRQ_PEND2 0x2000B208
#define IRQ_FIQ_CONTROL 0x2000B210
#define IRQ_ENABLE_BASIC 0x2000B218
#define IRQ_DISABLE_BASIC 0x2000B224

volatile unsigned int icount;
extern void enable_irq ( void );
void c_irq_handler ( void );


