/*
 *  sudoku.c
 *  sudoku
 *
 *  Created on 8/19/09.
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "sudoku.h"


void loadMatrix ( const char *argv[] ) {
	int k = 1;
	for ( int row = 0; row < 9; ++row ) {
		for ( int col = 0; col < 9; ++col ) {
			struct entry *e = G(row, col);
			e->row = row;
			e->col = col;
			
			char sym = argv[k++][0];
			int val;
			
			switch ( sym ) {
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					val = (sym - '0');
					e->is = val;
					memset(e->vals, 0, 9);
					e->vals[val-1] = 1;
					break;
				case '-':
					/*
					 * We only an unknown spot...
					 */
					e->is = 0;
					break;
					
				default:
					printf("WTF is [%c] doing in the puzzle?\n", sym);
					exit(1);
			}
		}
	}
}

void snapshot() {
	memcpy(copy, mat, matsize);
}

int compare() {
	return memcmp(copy, mat, matsize);
}

void set ( struct entry *e, int val ) {
	e->is = val;
	memset(e->vals, 0, 9);
	
	struct entry **r = allocRow(e->row);
	struct entry **c = allocCol(e->col);
	struct entry **n = allocNeighbors(e->row, e->col);
	
	for ( int i = 0; i < 9; ++i ) {
		N(r[i], val);
		N(c[i], val);
		N(n[i], val);
	}
}

struct entry **alloc9() {
	return malloc(sizeof(struct entry *) * 9);
}

struct entry **allocRow ( int row ) {
	struct entry **r = alloc9();
	for ( int col = 0; col < 9; ++col ) {
		r[col] = G(row, col);
	}
	
	return r;
}

struct entry **allocCol ( int col ) {
	struct entry **c = alloc9();
	for ( int row = 0; row < 9; ++row ) {
		c[row] = G(row, col);
	}
	
	return c;
}

struct entry **allocNeighbors ( int row, int col ) {
	struct entry **n = alloc9();
	int r = 0;
	int c = 0;
	switch ( row ) {
		case 0:
		case 1:
		case 2:
			r = 0;
			break;
			
		case 3:
		case 4:
		case 5:
			r = 3;
			break;
			
		case 6:
		case 7:
		case 8:
			r = 6;
			break;
			
		default:
			printf("WTF is row [%d]?\n", row);
			exit(1);
			break;
	}
	
	switch ( col ) {
		case 0:
		case 1:
		case 2:
			c = 0;
			break;
			
		case 3:
		case 4:
		case 5:
			c = 3;
			break;
			
		case 6:
		case 7:
		case 8:
			c = 6;
			break;
			
		default:
			printf("WTF is col [%d]?\n", col);
			exit(1);
			break;
	}
	
	int lastrow = r+3;
	int lastcol = c+3;
	
	for ( int j = r; j < lastrow; ++j ) {
		for ( int i = c; i < lastcol; ++i ) {
			n[((j-r) * 3) + (i-c)] = (mat + ((j * 9) + i));
		}
	}
	
	return n;
}

void printNeighbors ( struct entry **n ) {
	for ( int row = 0; row < 3; ++row ) {
		for ( int col = 0; col < 3; ++col ) {
			printf("%d  ", n[row * 3 + col]->is);
		}
		puts("");
	}
}

void printRow ( struct entry **r ) {
	for ( int col = 0; col < 9; ++col ) {
		printf("%d  ", r[col]->is);
	}
}

void printCol ( struct entry **c ) {
	for ( int row = 0; row < 9; ++row ) {
		printf("%d\n", c[row]->is);
	}
}

void printFull() {
	for ( int row = 0; row < 9; ++row ) {
		for ( int subrow = 0; subrow < 3; ++subrow ) {
			for ( int col = 0; col < 9; ++col ) {
				struct entry *e = G(row, col);
				int off = subrow * 3;
				char a = (e->vals[off + 0]) ? ('1' + off) : '-';
				char b = (e->vals[off + 1]) ? ('2' + off) : '-';
				char c = (e->vals[off + 2]) ? ('3' + off) : '-';
				printf("%c %c %c   ", a, b, c);
			}
			puts("");
		}
		puts("");
	}
}

void printMatrix() {
	for ( int row = 0; row < 9; ++row ) {
		for ( int col = 0; col < 9; ++col ) {
			char val = (G(row, col)->is) ? (G(row, col)->is + '0') : '-';
			printf("%c  ", val);
		}
		puts("");
	}
}

int check() {
	int filledCount = 0;
	
	for ( int row = 0; row < 9; ++row ) {
		for ( int col = 0; col < 9; ++col ) {
			struct entry *e = G(row, col);
			if ( e->is ) {
				++filledCount;
			}
		}
	}
	
	return 81 - filledCount;
}

int doublecheck() {
	int sum = 0;
	
	for ( int row = 0; row < 9; ++row ) {
		for ( int col = 0; col < 9; ++col ) {
			struct entry *e = G(row, col);
			for ( int val = 0; val < 9; ++val ) {
				sum += e->vals[val];
			}
		}
	}
	
	return sum;
}

void neighRowDetection ( struct entry **n ) {
	for ( int v = 0; v < 9; ++v ) {
		for ( int idx = 0; idx < 8; ++idx ) {
			struct entry *e = n[idx];
			printf("      (e.row, e.col, v:e.vals[v]): (%d, %d, %d:%d)\n", e->row, e->col, (v+1), e->vals[v]);
			if ( ! e->vals[v] ) continue;
			
			int found = 0;
			int row = e->row;
			
			for ( int jdx = idx + 1; jdx < 9; ++jdx ) {
				if ( n[jdx]->row != row ) {
					printf("        (row, col, vals[v]): (%d, %d, %d)\n", n[jdx]->row, n[jdx]->col, n[jdx]->vals[v]);
					if ( n[jdx]->vals[v] ) {
						found = 1;
						break;
					}
				}
			}
			
			if ( found ) continue;
			
			printf("  @@ detected a constrained row in this neighborhood for value %d...\n", (v+1));
			
			struct entry **r = allocRow(row);
			for ( int i = 0; i < 9; ++i ) {
				int matches = 0;
				for ( int j = 0; j < 9; ++j ) {
					if ( r[i] == n[j] ) {
						matches = 1;
						break;
					}
				}
				if ( ! matches ) {
					N(r[i], v+1);
				}
			}
		}
	}
}

void singletonDetection ( struct entry **n ) {
	for ( int v = 0; v < 9; ++v ) {
		int numFound = 0;
		int idx = -1;
		for ( int k = 0; k < 9; ++k ) {
			if ( n[k]->vals[v] ) {
				++numFound;
				idx = k;
			}
		}
		
		if ( 1 == numFound ) {
			set(n[idx], v+1);
		}
	}
}

void similarityReduction ( struct entry **n ) {
	char sims[9];
	
	for ( int idx = 0; idx < 8; ++idx ) {
		int degree = S(n[idx]);
		if ( 2 > degree ) continue;
		
		char *vals = n[idx]->vals;
		
		memset(sims, 0, 9);
		sims[idx] = 1;
		int numSimilar = 1;
		
		for ( int jdx = idx+1; jdx < 9; ++jdx ) {
			int deg = S(n[jdx]);
			
			if ( deg != degree ) continue;
			
			char *wals = n[jdx]->vals;
			if ( ! memcmp(vals, wals, 9) ) {
				++numSimilar;
				sims[jdx] = 1;
			}
		}
		
		if ( numSimilar != degree )	continue;
		
		for ( int jdx = 0; jdx < 9; ++jdx ) {
			if ( sims[jdx] ) continue;
			
			for ( int k = 0; k < 9; ++k ) {
				if ( vals[k] ) {
					N(n[jdx], k+1);
				}
			}
		}
	}
}

void printSingleEntry ( struct entry *e ) {
    printf("is: %d\n", e->is);
    printf("(row, col): (%d, %d)\n", e->row, e->col);
    printf("%d %d %d\n", e->vals[0], e->vals[1], e->vals[2]);
    printf("%d %d %d\n", e->vals[3], e->vals[4], e->vals[5]);
    printf("%d %d %d\n", e->vals[6], e->vals[7], e->vals[8]);
}

void checkIfEntrySolved ( struct entry *e ) {
    //printSingleEntry(e);
    
    int targetIndex = -1;
    int legalValuesLeft = 0;
    if ( 1 == S(e) ) {
        int idx = -1;
        for ( idx = 0; idx < 9; ++idx ) {
            if ( e->vals[idx] ) break;
        }
        set(e, 1+idx);
    }
}

int verifySolution() {
	for ( int idx = 0; idx < 9; ++idx ) {
		struct entry **r = allocRow(idx);
		struct entry **c = allocCol(idx);
		
		for ( int i = 0; i < 8; ++i ) {
			int ris = r[i]->is;
			int cis = c[i]->is;
			for ( int j = i+1; j < 9; ++j ) {
				if ( r[j]->is == ris ) {
					printf("\n  Solution does not verify (row[%d] violation).\n\n", idx);
					return 1;
				}
				if ( c[j]->is == cis ) {
					printf("\n  Solution does not verify (col[%d] violation).\n\n", idx);
					return 1;
				}
			}
		}
	}
	
	for ( int row = 0; row < 9; row += 3 ) {
		for ( int col = 0; col < 9; col += 3 ) {
			struct entry **n = allocNeighbors(row, col);
			for ( int i = 0; i < 8; ++i ) {
				int nis = n[i]->is;
				for ( int j = i+1; j < 9; ++j ) {
					if ( n[j]->is == nis ) {
						printf("  Solution does not verify (neighborhood[%d,%d] violation).\n", row, col);
						return 1;
					}
				}
			}
		}
	}
	
	puts("\n  Solution is verified.\n");
	return 0;
}
