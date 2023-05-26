lex flex.l
gcc lex.yy.c -o tokenizer -lfl
./tokenizer