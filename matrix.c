#include<stdio.h>
#include<stdlib.h>

#include "matrix.h"
#include "log.h"

static void dispose_cell(struct Cell *cell);
static struct Cell *get_cell();
static struct Colm *get_colhead();
static struct Row *get_rowhead();

// guess ill go to mars;

static struct Cell *get_cell() {

	struct Cell *ccell;
	ccell = malloc(sizeof(struct Cell));

	if (ccell)
		return ccell;
	else {
		smm_log(ERROR, "couldn't allocate memory for cell");
		exit(EXIT_FAILURE);
	}

	return ccell;
}

static void dispose_cell(struct Cell *cell) {

	if (!cell)
		return;

	free(cell);

}

static struct Row *get_rowhead() {

	struct Row *rowhead = NULL;

	rowhead = malloc(sizeof(struct Row));

	if (!rowhead) {
		smm_log(ERROR, "couldn't allocate memory for row");
		exit(EXIT_FAILURE);
	}

	return rowhead;
}

static struct Colm *get_colhead() {

	struct Colm *colhead = NULL;

	colhead = malloc(sizeof(struct Colm));

	if (!colhead) {
		smm_log(ERROR, "couldn't allocate memory for column");
		exit(EXIT_FAILURE);
	}

	return colhead;
}

struct Colm *add_col(struct Matrix *mat, struct Colm *leftcol, struct Colm *rightcol) {

	struct Cell *firstcell = NULL, *leftcell, *rightcell;
	struct Cell *precell = NULL, *curcell;
	struct Row *currowhead, *firstrowhead, *prerowhead = NULL;
	unsigned int i;

	if (!mat)
		return NULL;

	struct Colm *newcol = get_colhead();
	newcol->width = DEFAULT_COL_WIDTH;
	newcol->widecell = NULL;

	if (leftcol && rightcol) {

		leftcell = leftcol->cellstart;
		rightcell = rightcol->cellstart;

		/* leftcell and rightcell will always have NULL or a pointer
		 * at the same time, so we only need to check one
		 * */

		for (;leftcell;) {
			curcell = get_cell();

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

			curcell = get_cell();

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
			curcell = get_cell();
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

			currowhead = get_rowhead();
			curcell = get_cell();

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

void dispose_col(struct Matrix *mat, struct Colm *curcol) {

	struct Colm *leftcol, *rightcol;
	struct Cell *curcell, *precell, *leftcell, *rightcell;
	struct Row *currow, *prerow;

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

			dispose_cell(precell);
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
			dispose_cell(precell);
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
			dispose_cell(curcell);
		}

		mat->colstart = curcol->right;
		free(curcol);

	} else {

		for (currow=mat->rowstart;currow;) {
			prerow = currow;
			curcell = currow->cellstart;
			currow = currow->below;
			dispose_cell(curcell);
			free(prerow);
		}

		free(curcol);
		mat->rowstart = mat->colstart = NULL;

	}

	mat->ncols = mat->ncols - 1;

}

void dispose_row(struct Matrix *mat, struct Row *currow) {

	struct Row *rowbelow, *rowabove;
	struct Cell *curcell, *precell, *cellabove, *cellbelow;
	struct Colm *curcol, *precol;

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

			dispose_cell(precell);
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
			dispose_cell(precell);
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
			dispose_cell(curcell);
		}

		mat->rowstart = currow->below;
		free(currow);

	} else {

		for (curcol=mat->colstart;curcol;) {
			precol = curcol;
			curcell = curcol->cellstart;
			curcol = curcol->right;
			dispose_cell(curcell);
			free(precol);
		}

		free(currow);
		mat->rowstart = mat->colstart = NULL;

	}

}

struct Row *add_row(struct Matrix *mat, struct Row *rowabove, struct Row *rowbelow) {
	/* */
	struct Cell *firstcell = NULL, *cellabove, *cellbelow;
	struct Cell *precell = NULL, *curcell;
	struct Colm *curcolhead, *firstcolhead, *precolhead = NULL;
	unsigned int i;

	// if (!mat)
	// 	return NULL;

	struct Row *newrow = get_rowhead();
	newrow->height = DEFAULT_ROW_HEIGHT;
	newrow->highcell = NULL;

	if (rowabove && rowbelow) {

		cellabove = rowabove->cellstart;
		cellbelow = rowbelow->cellstart;

		/* cellabove and cellbelow will always have NULL or a pointer
		 * at the same time, so we only need to check one
		 * */

		for (;cellabove;) {
			curcell = get_cell();
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
			curcell = get_cell();
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
			curcell = get_cell();
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

			curcolhead = get_colhead();
			curcell = get_cell();

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

struct Matrix *make_matrix(unsigned int nrows, unsigned int ncols) {

	struct Matrix *mat = malloc(sizeof(struct Matrix));
	struct Matrix *last;
	struct Row *prerowhead;
	int i;

	if (mat) {
		mat->width = 0;
		mat->ncols = ncols;
		mat->nrows = 0;
		mat->right = mat->left = NULL;
		prerowhead = add_row(mat, NULL, NULL);
		for(i=1;i<nrows;i++) {
			prerowhead = add_row(mat, prerowhead, NULL);
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
