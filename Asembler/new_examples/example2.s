.section prva_sekcija
label0: .skip 0xa
label1:
.section druga_sekcija
.global prva
.extern prva, druga, treca
.word 0x3f42, prva, druga, 33444
.word 0x3f42
label2: label3: .ascii "asfassdfws45df"
.skip 1
.equ zez, 0x44
.end