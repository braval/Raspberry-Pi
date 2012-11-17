.text

.global _init
.global exc_stack
.global supervisor_sp

_init:
	b _reset
    ldr	pc, undef_handler_address
	ldr	pc, swi_handler_address
	ldr	pc, prefetch_abort_handler_address
	ldr	pc, data_abort_handler_address
    b	.
    ldr pc, irq_handler_address
	ldr	pc, fiq_handler_address



undef_handler_address:	        	.word	hang
swi_handler_address:	        	.word	hang
prefetch_abort_handler_address:   	.word	hang
data_abort_handler_address:     	.word	hang
irq_handler_address:	        	.word	_irq_handler
fiq_handler_address:	        	.word	hang


text_size_x: .word text_size
soffset: .word _reset - _init +8

_reset:  
    mov r0,pc
    ldr r1,soffset
    sub r0,r1
    cmp r0,#0
    beq skip_copy
    mov r1,#0;
    ldr r2,text_size_x
    add r2,r2,#0xFF
    add r2,r2,#15
    mov r2,r2,lsr #4
copy_loop:
    ldmia r0!,{r4,r5,r6,r7}
    stmia r1!,{r4,r5,r6,r7}
    subs r2,r2,#1
    bne copy_loop
    mov r0,#0
    bx r0

skip_copy:

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
   ldr sp, =exc_stack

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

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr
   
   .space 0x100

.ltorg
temp_stack:
   .space 4096

exc_stack:

supervisor_sp:
   .space 4

_irq_handler:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4




