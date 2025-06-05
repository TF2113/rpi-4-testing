
.global _start
.section .text

_start:
        mov x15, #15
        
loop:
        cmp x15, #0
        beq exit
        
        bl hello
        
        sub x15, x15, #1
        b loop
        
hello:
        mov w8, #64
        mov x0, #1
        ldr x1, =message
        ldr x2, =length
        mov w8, #64
        svc #0
        ret

exit:
        mov x0, #0
        mov w8, #93
        svc #0
        
.section .data    
    message:
        .ascii "Hello Test\n"
    
    length = . - message
