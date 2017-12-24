README.txt

To compile and run:

bison -d test6.y

flex tlex4.lex

gcc test6.tab.c lex.yy.c -lfl

./a.out < input1.txt

./a.out < input2.txt