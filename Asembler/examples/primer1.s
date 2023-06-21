#drugo testiranje drugog prolaza
.global d, f
.global bb
.equ aa, 0x100
.equ bb, 0x111 - aa
.section prva
.word 0x1
not %pc
csrwr %pc, %status
csrrd %handler, %r1
csrrd %cause, %r0
p: 
.section druga
.word 0x2
ld [%r1 + aa], %r2
ld [%r5], %r5
ld $f, %r0
ld 0xf10, %r5
ld %r1, %pc
ret
f:halt

d: int
.section trecaa
.word 0x3
st %r6, [%r5]
st %r5 , f
st %r1, %pc
st %r2, [%r1 + 0x1f]
ret
beq %r1, %r15, p

.section skokovi
.word 5
jmp 0x0000f0000



halt
