#include "ARMv7AR.h"
#include "MemoryMap.h"

.text
    .code 32
    
    .global vector_start
    .global vector_end

    vector_start:
        LDR     PC, reset_handler_addr
        LDR     PC, undef_handler_addr
        LDR     PC, svc_handler_addr
        LDR     PC, prefetch_handler_addr
        LDR     PC, data_handler_addr
        B       .
        LDR     PC, irq_handler_addr
        LDR     PC, fiq_handler_addr

        reset_handler_addr:     .word reset_handler
        undef_handler_addr:     .word dummy_handler
        svc_handler_addr:       .word dummy_handler
        prefetch_handler_addr:  .word dummy_handler
        data_handler_addr:      .word dummy_handler
        irq_handler_addr:       .word Irq_Handler
        fiq_handler_addr:       .word Fiq_Handler
    vector_end:


    reset_handler:
        // Initialize SVC mode
        MRS     R0, CPSR
        BIC     R1, R0, #0x1F
        ORR     R1, R1, #ARM_MODE_BIT_SVC
        MSR     CPSR, R1
        LDR     SP, =SVC_STACK_TOP
        // Initialize IRQ mode
        MRS     R0, CPSR
        BIC     R1, R0, #0x1F
        ORR     R1, R1, #ARM_MODE_BIT_IRQ
        MSR     CPSR, R1
        LDR     SP, =IRQ_STACK_TOP
        // Initialize FIQ mode
        MRS     R0, CPSR
        BIC     R1, R0, #0x1F
        ORR     R1, R1, #ARM_MODE_BIT_FIQ
        MSR     CPSR, R1
        LDR     SP, =FIQ_STACK_TOP
        // Initialize ABT mode
        MRS     R0, CPSR
        BIC     R1, R0, #0x1F
        ORR     R1, R1, #ARM_MODE_BIT_ABT
        MSR     CPSR, R1
        LDR     SP, =ABT_STACK_TOP
        // Initialize UND mode
        MRS     R0, CPSR
        BIC     R1, R0, #0x1F
        ORR     R1, R1, #ARM_MODE_BIT_UND
        MSR     CPSR, R1
        LDR     SP, =UND_STACK_TOP
        // Initialize USRSYS mode
        MRS     R0, CPSR
        BIC     R1, R0, #0x1F
        ORR     R1, R1, #ARM_MODE_BIT_SYS
        MSR     CPSR, R1
        LDR     SP, =USRSYS_STACK_TOP
        // Go to main function
        BL      main
    
    dummy_handler:
        B       .

.end
