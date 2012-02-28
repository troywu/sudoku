/*
 *  sudoku.h
 *  sudoku
 *
 *  Created on 8/19/09.
 */
 
struct entry {
	char vals[9];
	int is;
	int row;
	int col;
};

#define G(row, col)		(mat + ((((row) << 3) + (row)) + (col)))
#define N(e, val)		(e)->vals[(val)-1] = 0
#define S(e)			(	(e)->vals[0] + \
							(e)->vals[1] + \
							(e)->vals[2] + \
							(e)->vals[3] + \
							(e)->vals[4] + \
							(e)->vals[5] + \
							(e)->vals[6] + \
							(e)->vals[7] + \
							(e)->vals[8] )

size_t matsize;
struct entry *mat;
struct entry *copy;

void loadMatrix ( const char * [] );
void snapshot(void);
int compare(void);

void set ( struct entry *, int );
struct entry **alloc9(void);
struct entry **allocRow ( int );
struct entry **allocCol ( int );
struct entry **allocNeighbors ( int, int );

void printRow ( struct entry ** );
void printCol ( struct entry ** );
void printNeighbors ( struct entry ** );
void printMatrix(void);
void printFull(void);

int check(void);
int doublecheck(void);
int verifySolution(void);

void neighRowDetection ( struct entry ** );
void singletonDetection ( struct entry ** );
void similarityReduction ( struct entry ** );
