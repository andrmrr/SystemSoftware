./linker -hex \
-place=my_code@0x40000000 -place=math@0xF0000000 \
-o program.hex \
test_a/handler.o test_a/math.o test_a/main.o test_a/isr_terminal.o test_a/isr_timer.o test_a/isr_software.o