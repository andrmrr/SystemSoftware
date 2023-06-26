.global end_two, end_three 

.section prva
one:
.skip 10
end_one:
.section druga
.word end_two
.word end_three
.word end_one
two:
.word end_three
.word 0x1111
end_two:
.section treca
three:
st %r1, 0x100
.word 1919
end_three:
.word end_one
.end
