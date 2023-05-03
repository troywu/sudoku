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


.PHONY:		test
test:		all
	./sudoku - - - 2 7 3 - - - 2 7 - - - 5 1 9 - - - 5 - - 6 - - 2 8 - - - - 9 3 1 - 7 1 - - 2 - - 5 8 - 6 4 8 - - - - 9 6 - - 5 - - 9 - - - 2 1 6 - - - 3 7 - - - 1 3 2 - - -
