.section sekcija
.global zezanje
.equ apsa, 200
.equ ten, 10
halt
call 0x100
int
znak:
beq %sp, %r0, 0x1234
halt
jmp 0x00
zezanje:

.section arith
ld zezanje, %r0
ld $zezanje, %r0
ld [%r0+ten], %r0
ld $0xff00, %r0
ld 0xff00, %r0
ld [%pc], %r1
ld [%r0+14], %r0
ld %r1, %r2

.section stores
st %r7,0xff00
st %r7,zezanje
st %r1 ,    [%pc]
st %r7, [%r9+14]
csrrd %status, %r11
csrwr %r3,%handler
csrrd %cause , %pc


.section neka
jmp apsa
call znak
#ret #nista ovo

