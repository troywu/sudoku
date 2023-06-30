Simple Sudoku Solver
=

Many, many years ago, I was seated on a plane next to someone struggling to do
the sudoku in the airline magazine. Not only was that irritating on its own, but
the dude was writing in the damn magazine. Dude, that's not cool. Anyway, it
irritated me enough to make me wonder if I could write a program to solve all
sudoku puzzles (or at least some very large subset of them) before:

1) we landed, and
2) before he could solve just the one.

He quit the puzzle (*dude*) before I finished, but he knew what I was doing, and
was irritated when he saw my program solved it. I asked the stewardess for some
more newspapers or magazines, and it solved those, too.

*\[I don't remember which flight or even which airline, but the puzzle below is
the one printed in that magazine.\]*

This is a simple heuristic sudoku solver. It can solve trivial sudoku
puzzles--that's probably most puzzles sold at newspaper stands marked 'easy'
through 'hard'--. If it can't solve the puzzle, it will fill in what it knows to
be true, and leaves the rest as an exercise to the reader.

Compiling
-
Simple enough:

	make

Details: your C compiler needs a c99 mode. I use GCC, and pass:

	-std=c99

in CFLAGS. That's it. You don't need to care about this, unless `make`
doesn't work for you.

Running
-
Just give it text input. Describe a sudoku row by row. Like this:

	[~/proj/sudoku] $ ./sudoku - - - 2 7 3 - - - 2 7 - - - 5 1 9 - - - 5 - - 6 - - 2 8 - - - - 9 3 1 - 7 1 - - 2 - - 5 8 - 6 4 8 - - - - 9 6 - - 5 - - 9 - - - 2 1 6 - - - 3 7 - - - 1 3 2 - - -

Yes. It's...a pain. But, it works.

Output
-
Small maps like this:

	-  -  -  2  7  3  -  -  -  
	2  7  -  -  -  5  1  9  -  
	-  -  5  -  -  6  -  -  2  
	8  -  -  -  -  9  3  1  -  
	7  1  -  -  2  -  -  5  8  
	-  6  4  8  -  -  -  -  9  
	6  -  -  5  -  -  9  -  -  
	-  2  1  6  -  -  -  3  7  
	-  -  -  1  3  2  -  -  -

Represent the known puzzle state.  '-'s are used for unknown values, and numbers
represent definitely known values.

Large maps like this:

	1 - -   - - -   - - -   - - -   - - -   - - -   - - -   - - -   - - -   
	4 - -   4 - -   - - 6   - - -   - - -   - - -   4 5 6   4 - 6   4 5 6   
	- - 9   - 8 9   - 8 9   - - -   - - -   - - -   - 8 -   - 8 -   - - -   
	
	- - -   - - -   - - 3   - - -   - - -   - - -   - - -   - - -   - - 3   
	- - -   - - -   - - 6   4 - -   4 - -   - - -   - - -   - - -   4 - 6   
	- - -   - - -   - 8 -   - - -   - 8 -   - - -   - - -   - - -   - - -   
	
	1 - 3   - - 3   - - -   - - -   1 - -   - - -   - - -   - - -   - - -   
	4 - -   4 - -   - - -   4 - -   4 - -   - - -   4 - -   4 - -   - - -   
	- - 9   - 8 9   - - -   - - 9   - 8 9   - - -   7 8 -   7 8 -   - - -   
	
	- - -   - - -   - 2 -   - - -   - - -   - - -   - - -   - - -   - - -   
	- - -   - 5 -   - - -   4 - -   4 5 6   - - -   - - -   - - -   4 - 6   
	- - -   - - -   - - -   7 - -   - - -   - - -   - - -   - - -   - - -   
	
	- - -   - - -   - - 3   - - 3   - - -   - - -   - - -   - - -   - - -   
	- - -   - - -   - - -   4 - -   - - -   4 - -   4 - 6   - - -   - - -   
	- - -   - - -   - - 9   - - -   - - -   - - -   - - -   - - -   - - -   
	
	- - 3   - - -   - - -   - - -   1 - -   1 - -   - 2 -   - 2 -   - - -   
	- 5 -   - - -   - - -   - - -   - 5 -   - - -   - - -   - - -   - - -   
	- - -   - - -   - - -   - - -   - - -   7 - -   7 - -   7 - -   - - -   
	
	- - -   - - 3   - - 3   - - -   - - -   - - -   - - -   - 2 -   1 - -   
	- - -   4 - -   - - -   - - -   4 - -   4 - -   - - -   4 - -   4 - -   
	- - -   - 8 -   7 8 -   - - -   - 8 -   7 8 -   - - -   - 8 -   - - -   
	
	- - -   - - -   - - -   - - -   - - -   - - -   - - -   - - -   - - -   
	4 5 -   - - -   - - -   - - -   4 - -   4 - -   4 5 -   - - -   - - -   
	- - 9   - - -   - - -   - - -   - 8 9   - 8 -   - 8 -   - - -   - - -   
	
	- - -   - - -   - - -   - - -   - - -   - - -   - - -   - - -   - - -   
	4 5 -   4 5 -   - - -   - - -   - - -   - - -   4 5 6   4 - 6   4 5 6   
	- - 9   - 8 9   7 8 9   - - -   - - -   - - -   - 8 -   - 8 -   - - -   

Describe the set of possible values at each square. Each super-small-block that
looks like this:

	1 - -
	4 - -
	- - 9

represents a single cell in the puzzle, and the possible values. In the example,
1, 4, and 9 are possible values for that square.

The process just iterates through its relatively simple heuristics until the
puzzle is solved--or no more progress is made.

That's it!
