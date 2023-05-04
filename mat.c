#include<stdio.h>
#include<stdlib.h>
//#include<ncurses.h>

typedef struct matrix matrix;
typedef struct cell cell;
typedef struct row row;
typedef struct colm colm;

struct colm {
	int colmaxwidth;
	cell *widecell;
	cell *cellstart;
	colm *right;
	colm *left;
};

struct row {
	int rowmaxheight;
	cell *highcell;
	cell *cellstart;
	row *above;
	row *below;
};

struct matrix {
	cell *curcell;
	row *rowstart;
	row *currow;
	colm *colstart;
	colm *curcol;
	matrix *left;
	matrix *right;
};

struct cell {
	long value;
	cell *left;
	cell *right;
	cell *above;
	cell *below;
};

cell *cellarray = NULL;

matrix *matrixhead = NULL;
unsigned int matrixlen = 4;
unsigned int exitnow = 0;

void disposecell(cell *cell);
cell *getcell();
matrix *makematrix();
void printmatrix(matrix *mat);
row *getrowhead();

// guess ill go to mars;

cell *getcell() {

	cell *ccell;
	ccell = malloc(sizeof(cell));

	if (ccell)
		return ccell;
	else
		exitnow = 0;

	//cell->left = cell->right = cell->below = cell->above = NULL;

	return ccell;
}

void disposecell(cell *cell) {

	if (!cell)
		return;

	free(cell);

}


row *getrowhead() {

	row *rowhead = NULL;

	rowhead = malloc(sizeof(row));

	if (!rowhead)
		exitnow = 1;

	return rowhead;
}

colm *getcolhead() {

	colm *colhead = NULL;

	colhead = malloc(sizeof(colm));

	if (!colhead)
		exitnow = 1;

	return colhead;
}

colm *addcol(matrix *mat, colm *leftcol, colm *rightcol) {
	/* */
	cell *firstcell = NULL, *leftcell, *rightcell;
	cell *precell = NULL, *curcell;
	row *currowhead, *firstrowhead, *prerowhead = NULL;
	int i;

	colm *newcol = getcolhead();
	newcol->colmaxwidth = 0;
	newcol->widecell = NULL;

	if (!leftcol && !rightcol) {

		for (i=matrixlen;i;i--) {

			currowhead = getrowhead();
			curcell = getcell();

			currowhead->cellstart = curcell;
			if (!firstrowhead) {
				firstrowhead = currowhead;
			}
			currowhead->rowmaxheight = 0;
			currowhead->highcell = NULL;
			currowhead->above = precolhead;
			if (prerowhead) {
				prerowhead->below = currowhead;
			}
			currowhead->below = NULL;

			curcell->above = precell;
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->below = curcell;
			}
			curcell->right = NULL;
			curcell->left = NULL;
			curcell->below = NULL;
			curcell->value = 0;
			precell = curcell;
		}

		mat->colstart = newcol;
		mat->rowstart = firstrowhead;
		newcol->left = newcol->right = NULL;

	} else if (leftcol) {

		 leftcell = leftcol->cellstart;

		for (;leftcell;leftcell = leftcell->below) {
			curcell = getcell();

			curcell->above = precell;
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->below = curcell;
			}
			curcell->right = NULL;
			curcell->left = leftcell;
			curcell->below = NULL;
			left->right = curcell;
			curcell->value = 0;

			precell = curcell;
		}

		newcol->left = leftcol;
		leftcol->right = newcol;
		newcol->right = NULL;

	} else if (rightcol) {

		rightcell = rightcol->cellstart;
		currowhead = mat->rowstart;

		for (;rightcell;rightcell = rightcell->below) {
			curcell = getcell();
			curcell->above = precell;
			if (currowhead) {
				currowhead->cellstart = curcell;
				currowhead = currowhead->below;
			}
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->below = curcell;
			}
			curcell->right = rightcell;
			curcell->left = NULL;
			curcell->below = NULL;
			rightcell->left = curcell;
			curcell->value = 0;

			precell = curcell;
		}

		mat->colstart = newcol;
		newcol->right = rightcol;
		rightcol->left = newcol;
		newcol->left = NULL;

	} else {

		leftcell = leftcol->cellstart;
		rightcell = rightcol->cellstart;

		/* leftcell and rightcell will always have NULL or a pointer
		 * at the same time, so we only need to check one
		 * */

		for (;leftcell;) {
			curcell = getcell();
			curcell->above = precell;
			if (precell) {
				precell->below = curcell;
			}
			curcell->below = NULL;
			curcell->left = leftcell;
			curcell->right = rightcell;
			leftcell->right = curcell;
			rightcell->left = curcell;
			curcell->value = 0;

			precell = curcell;
			leftcell = leftcell->below;
			rightcell = rightcell->below;
		}

		newcol->left = leftcol;
		leftcol->right = newcol;
		newcol->right = rightcol;
		rightcol->left = newcol;

	}

	newcol->cellstart = firstcell;
	return newcol;
}

row *addrow(matrix *mat, row *rowabove, row *rowbelow) {
	/* */
	cell *firstcell = NULL, *cellabove, *cellbelow;
	cell *precell = NULL, *curcell;
	colm *curcolhead, *firstcolhead, *precolhead = NULL;
	int i;

	row *newrow = getrowhead();
	newrow->rowmaxheight = 0;
	newrow->highcell = NULL;

	if (!rowabove && !rowbelow) {

		for (i=matrixlen;i;i--) {

			curcolhead = getcolhead();
			curcell = getcell();

			curcolhead->cellstart = curcell;
			if (!firstcolhead) {
				firstcolhead = curcolhead;
			}
			curcolhead->colmaxwidth = 0;
			curcolhead->widecell = NULL;
			curcolhead->left = precolhead;
			if (precolhead) {
				precolhead->right = curcolhead;
			}
			curcolhead->right = NULL;

			curcell->left = precell;
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->right = curcell;
			}
			curcell->right = NULL;
			curcell->above = NULL;
			curcell->below = NULL;
			curcell->value = 0;
			precell = curcell;
		}

		mat->rowstart = newrow;
		mat->colstart = firstcolhead;
		newrow->above = newrow->below = NULL;

	} else if (rowabove) {

		cellabove = rowabove->cellstart;

		for (;cellabove;cellabove = cellabove->right) {
			curcell = getcell();
			curcell->left = precell;
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->right = curcell;
			}
			curcell->right = NULL;
			curcell->below = NULL;
			curcell->above = cellabove;
			cellabove->below = curcell;
			curcell->value = 0;

			precell = curcell;
		}

		newrow->above = rowabove;
		rowabove->below = newrow;
		newrow->below = NULL;

	} else if (rowbelow) {

		cellbelow = rowbelow->cellstart;
		curcolhead = mat->colstart;

		for (;cellbelow;cellbelow = cellbelow->right) {
			curcell = getcell();
			curcell->left = precell;
			if (curcolhead) {
				curcolhead->cellstart = curcell;
				curcolhead = curcolhead->right;
			}
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->right = curcell;
			}
			curcell->right = NULL;
			curcell->above = NULL;
			curcell->below = cellbelow;
			cellbelow->above = curcell;
			curcell->value = 0;

			precell = curcell;
		}

		mat->rowstart = newrow;
		newrow->below = rowbelow;
		rowbelow->above = newrow;
		newrow->above = NULL;

	} else {

		cellabove = rowabove->cellstart;
		cellbelow = rowbelow->cellstart;

		/* cellabove and cellbelow will always have NULL or a pointer
		 * at the same time, so we only need to check one
		 * */

		for (;cellabove;) {
			curcell = getcell();
			curcell->left = precell;
			if (precell) {
				precell->right = curcell;
			}
			curcell->right = NULL;
			curcell->below = cellbelow;
			curcell->above = cellabove;
			cellabove->below = curcell;
			cellbelow->above = curcell;
			curcell->value = 0;

			precell = curcell;
			cellabove = cellabove->right;
			cellbelow = cellbelow->right;
		}

		newrow->above = rowabove;
		rowabove->below = newrow;
		newrow->below = rowbelow;
		rowbelow->above = newrow;

	}

	newrow->cellstart = firstcell;
	return newrow;
}


matrix *makematrix() {

	matrix *mat = malloc(sizeof(matrix));
	matrix *last;
	row *prerowhead;
	int i;

	if (mat) {
		for (last = matrixhead; last && last->right;last = last->right);

		mat->left = last;

		if (matrixhead) 
			last->right = mat;
		else
			matrixhead = mat;

		prerowhead = addrow(mat, NULL, NULL);
		for(i=1;i<matrixlen;i++) {
			prerowhead = addrow(mat, prerowhead, NULL);
		}

	} else {
		// get the fuck outta here!!
		exitnow = 1;
	}
	
	return mat;
}


/*
char char_top_l[]  = "┏" ;
char char_top_r[]  = "┓" ;
char char_bot_l[]  = "┗" ;
char char_bot_r[]  = "┛" ;
char char_minus[]  = "━" ;
char char_pipe[]   = "┃" ;
char char_space    = ' ' ;
*/

char char_top_l[]  = "┌" ;
char char_top_r[]  = "┐" ;
char char_bot_l[]  = "└" ;
char char_bot_r[]  = "┘" ;
char char_minus[]  = "─" ;
char char_pipe[]   = "│" ;
char char_space    = ' ' ;

int max_len = 1;
int max_num = 10;
int mat_size = 3;

/*
─ ━ │ ┃ ┄ ┅ ┆ ┇ ┈ ┉ ┊ ┋ ┌ ┍ ┎ ┏
U+251x    ┐ ┑ ┒ ┓ └ ┕ ┖ ┗ ┘ ┙ ┚ ┛ ├ ┝ ┞ ┟
U+252x    ┠ ┡ ┢ ┣ ┤ ┥ ┦ ┧ ┨ ┩ ┪ ┫ ┬ ┭ ┮ ┯
U+253x    ┰ ┱ ┲ ┳ ┴ ┵ ┶ ┷ ┸ ┹ ┺ ┻ ┼ ┽ ┾ ┿
U+254x    ╀ ╁ ╂ ╃ ╄ ╅ ╆ ╇ ╈ ╉ ╊ ╋ ╌ ╍ ╎ ╏
U+255x    ═ ║ ╒ ╓ ╔ ╕ ╖ ╗ ╘ ╙ ╚ ╛ ╜ ╝ ╞ ╟
U+256x    ╠ ╡ ╢ ╣ ╤ ╥ ╦ ╧ ╨ ╩ ╪ ╫ ╬ ╭ ╮ ╯
U+257x    ╰ ╱ ╲ ╳ ╴ ╵ ╶ ╷ ╸ ╹ ╺ ╻ ╼ ╽ ╾ ╿
Notes
*/


void getvalue (int mat[mat_size][mat_size], int mnum);
void multmatrix (int mat1[mat_size][mat_size], int mat2[mat_size][mat_size], int mat3[mat_size][mat_size]);
int getlength (int num);
void getwidth (int mat[mat_size][mat_size]);

void getwidth (int mat[mat_size][mat_size]) {

	int i, j;
	max_len = 1;
	max_num = 10;

	for (i=0; i < mat_size;i++) {
		for (j=0; j < mat_size;j++) {
			if (mat[i][j] > max_num)
				max_len = getlength(mat[i][j]);
		}
	}

}

int getlength (int num) {

	if (num >= 1000000000) {
		max_num = 1000000000;
		return 10;
	}
	if (num >= 100000000) {
		max_num = 100000000;
		return 9;
	}
	if (num >= 10000000) {
		max_num = 10000000;
		return 8;
	}
	if (num >= 1000000) {
		max_num = 1000000;
		return 7;
	}
	if (num >= 100000) {
		max_num = 100000;
		return 6;
	}
	if (num >= 10000) {
		max_num = 10000;
		return 5;
	}
	if (num >= 1000) {
		max_num = 1000;
		return 4;
	}
	if (num >= 100) {
		max_num = 100;
		return 3;
	}
	if (num >= 10) {
		max_num = 10;
		return 2;
	}

	return 1;

}

void getvalue (int mat[mat_size][mat_size], int mnum) {

	int i, j;

	printf("Enter the matrix %d\n", mnum);
	for (i=0; i < mat_size;i++) {
		for (j=0; j < mat_size;j++) {
			printf("Enter the value for (%d,%d): ", i, j);
			scanf("%d", &mat[i][j]);
		}
	}
	printf("\n");
}

void printmatrix(matrix *mat) {

	int i, j;
	cell *currowhead, *curcell;

	if (!mat)
		return;

	max_len = 2;
	//getwidth (mat);
	max_len++;

	printf("%s%s", char_top_l, char_minus);
	printf("%*c", max_len * matrixlen - 1, char_space);
	printf("%s%s\n", char_minus, char_top_r);

	for (currowhead = mat->rowstart->cellstart;currowhead;currowhead = currowhead->below) {
		printf("%s", char_pipe);
		for (curcell = currowhead;curcell;curcell = curcell->right) {
			printf("% *ld", max_len, curcell->value);
		}
		printf(" %s\n", char_pipe);
	}

	printf("%s%s", char_bot_l, char_minus);
	printf("%*c", max_len * matrixlen - 1, char_space);
	printf("%s%s\n", char_minus, char_bot_r);

}

void multmatrix (int mat1[mat_size][mat_size], int mat2[mat_size][mat_size], int mat3[mat_size][mat_size]) {

	int i, j, k;

	for (i=0; i < mat_size; i++){
		for (j=0; j < mat_size; j++){
			mat3[i][j] = 0;
		}
	}

	// TODO : figure out this shit
	for (i=0; i < mat_size; i++){
		for (j=0; j < mat_size; j++){
			for (k=0; k < mat_size; k++){
				mat3[i][j] = mat3[i][j] + (mat1[i][k] * mat2[k][j]);
			}
		}
	}

}

int main () {

	
	// int mat1[mat_size][mat_size];
	// int mat2[mat_size][mat_size];
	// int mat3[mat_size][mat_size];

	// getvalue(mat1, 1);
	// getvalue(mat2, 2);

	// multmatrix(mat1, mat2, mat3);

	// printmatrix(mat1);
	// printmatrix(mat2);
	// printmatrix(mat3);
	
	// initscr();
	// clear();
	// noecho();
	// keypad(stdscr, TRUE);

	// while ()
	printmatrix(makematrix());

}

