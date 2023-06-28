.section sekcija
halt
int
znak:
beq %r1, %r2, 0xdd78ff8a
bne %r7, %r8, 0xdd78ff8a
bgt %r10, %r11, 0xdd78ff8a
halt
iret
push %r3
pop %sp
ret
call 0x100
halt
jmp znak
call znak
#ret #nista ovo

