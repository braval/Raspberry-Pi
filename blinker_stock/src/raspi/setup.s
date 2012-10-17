.text

.global _start
.global exc_stack
.global supervisor_sp

_start:

   # initialize Stack pointer for exception modes
   mrs r4, cpsr
   bic r4, r4, #0x1f

   #FIQ Mode
   orr r3, r4, #0x11
   msr cpsr_c, r3
   ldr sp, =exc_stack

   #IRQ Mode
   orr r3, r4, #0x12
   msr cpsr_c, r3
   ldr sp, =irq

   #ABORT Mode
   orr r3, r4, #0x17
   msr cpsr_c, r3
   ldr sp, =exc_stack

   #UNDEFINED Mode
   orr r3, r4, #0x1b
   msr cpsr_c, r3
   ldr sp, =exc_stack

   # switch to supervisor mode
   orr r3, r4, #0x13
   msr cpsr_c, r3
   
   ldr sp, =temp_stack
   
   #
   # Create the first stack frame.
   #
   mov fp, #0
   mov ip, sp
   push {fp, ip, lr, pc}
   sub fp, ip, #4

   bl not_main
hang: b hang

.globl dummy
dummy:
    bx lr

   
   .space 0x100
temp_stack:

   .space 1024
exc_stack:
   .space 1024
supervisor_sp:
   .space 4
   
irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

