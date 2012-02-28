CFLAGS="-std=c99"

.PHONY:		all
all:		sudoku
sudoku:		main.o
main.o:		sudoku.o main.c
sudoku.o:	sudoku.c sudoku.h


.PHONY:		clean
clean:
	rm -f *.o
	rm -f sudoku
	rm -rf a.out
