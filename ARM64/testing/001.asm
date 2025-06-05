
.global _start
.section .text

_start:

 hello:
        mov w8, #64
        mov x0, #1
        ldr x1, =message
        ldr x2, =length
        mov w8, #64
        svc #0
 exit:
        
        mov x0, #0
        mov w8, #93
        svc #0
        
.section .data    
    message:
        .ascii "Hello Remote\n"
    
    length = . - message
