#drugo testiranje drugog prolaza
.global dd, ff
.global d
.global b
.equ a, 0x100
.equ b, 0x111 - a
.section prva
not %pc
csrwr %pc, %status
csrrd %handler, %r1
csrrd %cause, %r0
pp: 
.section druga
ld [%r1 + a], %r2
ld [%r5], %r5
ld $d, %r0
ld 0xf10, %r5
ld %r1, %pc
ret
ff:halt

dd: int
.section trecaa
st %r6, [%r5]
st %r5 , ff
st %r1, %pc
st %r2, [%r1 + 0x1f]
ret
beq %r1, %r15, pp

.section skokovi
jmp 0x0000f0000


.section cetvrtaa
.skip 6
ret
.ascii "zezanje"
halt
halt
.word ff, 0x00ff0000, 19, b
.end
.skip 11


halt
