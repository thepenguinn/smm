#include<stdio.h>
#include<stdlib.h>

#include "matrix.h"
#include "log.h"

#define DEFAULT_VALUE 0
#define DEFAULT_ROW_HEIGHT 1
#define DEFAULT_COL_WIDTH 3

static cell *getcell();
static void disposecell(cell *cell);
static colm *getcolhead();
static row *getrowhead();

// guess ill go to mars;

static cell *getcell() {

	cell *ccell;
	ccell = malloc(sizeof(cell));

	if (ccell)
		return ccell;
	else {
		smm_log(ERROR, "couldn't allocate memory for cell");
		exit(EXIT_FAILURE);
	}

	return ccell;
}

static void disposecell(cell *cell) {

	if (!cell)
		return;

	free(cell);

}

static row *getrowhead() {

	row *rowhead = NULL;

	rowhead = malloc(sizeof(row));

	if (!rowhead) {
		smm_log(ERROR, "couldn't allocate memory for row");
		exit(EXIT_FAILURE);
	}

	return rowhead;
}

static colm *getcolhead() {

	colm *colhead = NULL;

	colhead = malloc(sizeof(colm));

	if (!colhead) {
		smm_log(ERROR, "couldn't allocate memory for column");
		exit(EXIT_FAILURE);
	}

	return colhead;
}

colm *addcol(matrix *mat, colm *leftcol, colm *rightcol) {

	cell *firstcell = NULL, *leftcell, *rightcell;
	cell *precell = NULL, *curcell;
	row *currowhead, *firstrowhead, *prerowhead = NULL;
	unsigned int i;

	if (!mat)
		return NULL;

	colm *newcol = getcolhead();
	newcol->width = DEFAULT_COL_WIDTH;
	newcol->widecell = NULL;

	if (leftcol && rightcol) {

		leftcell = leftcol->cellstart;
		rightcell = rightcol->cellstart;

		/* leftcell and rightcell will always have NULL or a pointer
		 * at the same time, so we only need to check one
		 * */

		for (;leftcell;) {
			curcell = getcell();

			curcell->above = precell;
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->below = curcell;
			}
			curcell->below = NULL;
			curcell->left = leftcell;
			curcell->right = rightcell;
			leftcell->right = curcell;
			rightcell->left = curcell;
			curcell->value = DEFAULT_VALUE;

			precell = curcell;
			leftcell = leftcell->below;
			rightcell = rightcell->below;
		}

		newcol->left = leftcol;
		leftcol->right = newcol;
		newcol->right = rightcol;
		rightcol->left = newcol;

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
			leftcell->right = curcell;
			curcell->value = DEFAULT_VALUE;

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
			curcell->value = DEFAULT_VALUE;

			precell = curcell;
		}

		mat->colstart = newcol;
		newcol->right = rightcol;
		rightcol->left = newcol;
		newcol->left = NULL;

	} else {

		for (i=mat->nrows;i;i--) {

			currowhead = getrowhead();
			curcell = getcell();

			currowhead->cellstart = curcell;
			if (!firstrowhead) {
				firstrowhead = currowhead;
			}
			currowhead->height = DEFAULT_ROW_HEIGHT;
			currowhead->highcell = NULL;
			currowhead->above = prerowhead;
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
			curcell->value = DEFAULT_VALUE;

			currowhead->cellstart = curcell;

			precell = curcell;
			prerowhead = currowhead;
			mat->height += 1;
			mat->nrows += 1;

		}

		mat->colstart = newcol;
		mat->rowstart = firstrowhead;
		newcol->left = newcol->right = NULL;

	}

	mat->ncols = mat->ncols + 1;
	newcol->cellstart = firstcell;
	return newcol;

}

void disposecol(matrix *mat, colm *curcol) {

	colm *leftcol, *rightcol;
	cell *curcell, *precell, *leftcell, *rightcell;
	row *currow, *prerow;

	leftcol = curcol->left;
	rightcol = curcol->right;

	if (leftcol && rightcol) {

		leftcell = leftcol->cellstart;
		rightcell = rightcol->cellstart;

		for (curcell = curcol->cellstart;curcell;) {
			leftcell->right = rightcell;
			rightcell->left = leftcell;

			precell = curcell;
			leftcell = leftcell->below;
			rightcell = rightcell->below;
			curcell = curcell->below;

			disposecell(precell);
		}
		leftcol->right = rightcol;
		rightcol->left = leftcol;

		free(curcol);

	} else if (leftcol) {

		leftcell = leftcol->cellstart;

		for (curcell=curcol->cellstart;curcell;) {
			precell = curcell;
			leftcell->right = NULL;
			leftcell = leftcell->below;
			curcell = curcell->below;
			disposecell(precell);
		}

		leftcol->right = NULL;
		free(curcol);

	} else if (rightcol) {

		for (currow=mat->rowstart;currow;) {
			curcell = currow->cellstart;
			rightcell = curcell->right;
			currow->cellstart = rightcell;
			rightcell->left = NULL;
			currow = currow->below;
			disposecell(curcell);
		}

		mat->colstart = curcol->right;
		free(curcol);

	} else {

		for (currow=mat->rowstart;currow;) {
			prerow = currow;
			curcell = currow->cellstart;
			currow = currow->below;
			disposecell(curcell);
			free(prerow);
		}

		free(curcol);
		mat->rowstart = mat->colstart = NULL;

	}

	mat->ncols = mat->ncols - 1;

}

void disposerow(matrix *mat, row *currow) {

	row *rowbelow, *rowabove;
	cell *curcell, *precell, *cellabove, *cellbelow;
	colm *curcol, *precol;

	rowabove = currow->above;
	rowbelow = currow->below;

	if (rowabove && rowbelow) {

		cellabove = rowabove->cellstart;
		cellbelow = rowbelow->cellstart;

		for (curcell = currow->cellstart;curcell;) {
			cellabove->below = cellbelow;
			cellbelow->above = cellabove;

			precell = curcell;
			cellabove = cellabove->right;
			cellbelow = cellbelow->right;
			curcell = curcell->right;

			disposecell(precell);
		}
		rowabove->below = rowbelow;
		rowbelow->above = rowabove;

		free(currow);

	} else if (rowabove) {

		cellabove = rowabove->cellstart;

		for (curcell=currow->cellstart;curcell;) {
			precell = curcell;
			cellabove->below = NULL;
			cellabove = cellabove->right;
			curcell = curcell->right;
			disposecell(precell);
		}

		rowabove->below = NULL;
		free(currow);

	} else if (rowbelow) {

		for (curcol=mat->colstart;curcol;) {
			curcell = curcol->cellstart;
			cellbelow = curcell->below;
			curcol->cellstart = cellbelow;
			cellbelow->above = NULL;
			curcol = curcol->right;
			disposecell(curcell);
		}

		mat->rowstart = currow->below;
		free(currow);

	} else {

		for (curcol=mat->colstart;curcol;) {
			precol = curcol;
			curcell = curcol->cellstart;
			curcol = curcol->right;
			disposecell(curcell);
			free(precol);
		}

		free(currow);
		mat->rowstart = mat->colstart = NULL;

	}

}

row *addrow(matrix *mat, row *rowabove, row *rowbelow) {
	/* */
	cell *firstcell = NULL, *cellabove, *cellbelow;
	cell *precell = NULL, *curcell;
	colm *curcolhead, *firstcolhead, *precolhead = NULL;
	unsigned int i;

	// if (!mat)
	// 	return NULL;

	row *newrow = getrowhead();
	newrow->height = DEFAULT_ROW_HEIGHT;
	newrow->highcell = NULL;

	if (rowabove && rowbelow) {

		cellabove = rowabove->cellstart;
		cellbelow = rowbelow->cellstart;

		/* cellabove and cellbelow will always have NULL or a pointer
		 * at the same time, so we only need to check one
		 * */

		for (;cellabove;) {
			curcell = getcell();
			curcell->left = precell;
			if (!firstcell) {
				firstcell = curcell;
			}
			if (precell) {
				precell->right = curcell;
			}
			curcell->right = NULL;
			curcell->below = cellbelow;
			curcell->above = cellabove;
			cellabove->below = curcell;
			cellbelow->above = curcell;
			curcell->value = DEFAULT_VALUE;

			precell = curcell;
			cellabove = cellabove->right;
			cellbelow = cellbelow->right;
		}


		newrow->above = rowabove;
		rowabove->below = newrow;
		newrow->below = rowbelow;
		rowbelow->above = newrow;

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
			curcell->value = DEFAULT_VALUE;

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
			curcell->value = DEFAULT_VALUE;

			precell = curcell;
		}

		mat->rowstart = newrow;
		newrow->below = rowbelow;
		rowbelow->above = newrow;
		newrow->above = NULL;

	} else {

		for (i=mat->ncols;i;i--) {

			curcolhead = getcolhead();
			curcell = getcell();

			curcolhead->cellstart = curcell;
			if (!firstcolhead) {
				firstcolhead = curcolhead;
			}
			curcolhead->width = DEFAULT_COL_WIDTH;
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
			curcell->value = DEFAULT_VALUE;

			curcolhead->cellstart = curcell;

			precell = curcell;
			precolhead = curcolhead;
			mat->width += 1;
			mat->ncols += 1;
		}

		mat->rowstart = newrow;
		mat->colstart = firstcolhead;
		newrow->above = newrow->below = NULL;

	}

	mat->nrows = mat->nrows + 1;
	newrow->cellstart = firstcell;
	return newrow;
}

matrix *makematrix(unsigned int nrows, unsigned int ncols) {

	matrix *mat = malloc(sizeof(matrix));
	matrix *last;
	row *prerowhead;
	int i;

	if (mat) {
		mat->width = 0;
		mat->ncols = ncols;
		mat->nrows = 0;
		mat->right = mat->left = NULL;
		prerowhead = addrow(mat, NULL, NULL);
		for(i=1;i<nrows;i++) {
			prerowhead = addrow(mat, prerowhead, NULL);
		}

		mat->curcol = mat->colstart;
		mat->currow = mat->rowstart;
		mat->curcell = mat->rowstart->cellstart;
		mat->curcellxratio = mat->curcellyratio = 0;
	} else {
		exit(EXIT_FAILURE);
	}
	
	return mat;
}
