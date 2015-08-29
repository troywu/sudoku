#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sudoku.h"

int main (int argc, const char * argv[]) {
	if ( argc < 2 ) {
		printf("Usage: %s <0,0><0,1>...<m,n>\n", argv[0]);
		return 1;
	}
	
	matsize = sizeof(struct entry) * 81;
	
	mat = malloc(matsize);
	copy = malloc(matsize);
	
	for ( int i = 0; i < 81; ++i ) {
		memset((mat + i)->vals, 1, 9);
	}

	loadMatrix(argv);
	
	puts("\n  Starting state...\n");
	printMatrix();
	
	puts("\n\n  Before any analysis...\n");
	printFull();
	printMatrix();
	
	for ( int row = 0; row < 9; ++row ) {
		for ( int col = 0; col < 9; ++col ) {
			struct entry *e = mat + (row * 9 + col);
			if ( e->is ) {
				set(e, e->is);
			}
		}
	}
	
	puts("\n\n  After set()-exclusion...\n");
	printFull();
	printMatrix();
	
	/*
	 * Track the number of unsolved spots.
	 */
	int empties = 0;
	
	for ( int iter = 1; ; ++iter ) {
		snapshot();
		
		for ( int row = 0; row < 9; ++row ) {
			struct entry **r = allocRow(row);
			similarityReduction(r);
		}
		
		puts("\n\n  After row-similarity-reduction...\n");
		printFull();
		printMatrix();
		
		for ( int col = 0; col < 9; ++col ) {
			struct entry **c = allocCol(col);
			similarityReduction(c);
		}
		
		puts("\n\n  After col-similarity-reduction...\n");
		printFull();
		printMatrix();
		
		for ( int col = 0; col < 9; col += 3 ) {
			for ( int row = 0; row < 8; row += 3 ) {
				struct entry **n = allocNeighbors(row, col);
				similarityReduction(n);
			}
		}
		
		puts("\n\n  After neighborhood-similarity-reduction...\n");
		printFull();
		printMatrix();
		
		for ( int row = 0; row < 9; ++row ) {
			struct entry **r = allocRow(row);
			singletonDetection(r);
			free(r);
		}
		
		puts("\n\n  After row-singleton-detection...\n");
		printFull();
		printMatrix();

		for ( int col = 0; col < 9; ++col ) {
			struct entry **c = allocCol(col);
			singletonDetection(c);
			free(c);
		}
		
		puts("\n\n  After col-singleton-detection...\n");
		printFull();
		printMatrix();
        
        for ( int row = 0; row < 9; ++row ) {
            for ( int col = 0; col < 9; ++col ) {
                struct entry *e = G(row, col);
                checkIfEntrySolved(e);
            }
        }
        
        puts("\n\n  After entry-singleton-detection...\n");
        printFull();
        printMatrix();
		
//		for ( int row = 0; row < 9; row += 3 ) {
//			for ( int col = 0; col < 9; col += 3 ) {
//				struct entry **n = allocNeighbors(row, col);
//				singletonDetection(n);
//				free(n);
//			}
//		}
//		
//		puts("\n\n  After neighborhood-singleton-detection...\n");
//		printFull();
//		printMatrix();
//		
//		/*
//		 * If a number can only be in 1 row (or col) of neighborhood,
//		 * then that row (or col) MUST contain that number.  Outside
//		 * of that neighborhood but still in that row (or col), that
//		 * number cannot be.
//		 *
//		 * Get N
//		 * Starting with 0th entry...for each possible, check
//		 *   in other rows (in neigh).  If not there, then we're
//		 *   neigh-row constrained, and all other non-neigh-row
//		 *   entries cannot have possible.
//		 * Starting with 0th entry...for each possible, check
//		 *   in other cols (in neigh).  If not there, then we're
//		 *   neigh-col constrained, and all other non-neigh-col
//		 *   entries cannot have possible.
//		 */
//		/*
//		for ( int row = 0; row < 9; row += 3 ) {
//			for ( int col = 0; col < 9; col += 3 ) {
//				struct entry **n = allocNeighbors(row, col);
//
//				printf("@@ Looking at neighborhood around (%d, %d)...\n", row, col);
//				neighRowDetection(n);
//
//				free(n);
//			}
//		}
//		
//		puts("\n\n  After neighborhood-row-detection...\n");
//		printFull();
//		printMatrix();
//		 */
//		
//		for ( int row = 0; row < 9; ++row ) {
//			for ( int col = 0; col < 9; ++col ) {
//				if ( 1 == S(G(row, col)) ) {
//					struct entry *e = G(row, col);
//					for ( int v = 0; v < 9; ++v ) {
//						if ( 0 < e->vals[v] ) {
//							set(e, v+1);
//							break;
//						}
//					}
//				}
//			}
//		}
//		
//		puts("\n\n  After singleton-reduction...\n");
//		printFull();
//		printMatrix();
		
		/*
		 * Display state of the matrix.
		 */
		
		empties = check();
		printf("\n  ==> %d empties after %d iterations...\n\n", empties, iter);
		
		if ( 0 == empties ) {
			printFull();
			printMatrix();
			break;
		} else if ( ! compare() ) {
			puts("    FAILURE...cannot make any more progress.");
			printFull();
			printMatrix();
			break;
		}
	}
	
	free(mat);
	
	return verifySolution();
}
