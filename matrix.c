#include<stdio.h>
#include<stdlib.h>

#include "matrix.h"
#include "log.h"

static void dispose_cell(struct Cell *cell);
static struct Cell *get_cell(void);
static struct Colm *get_colhead(void);
static struct Row *get_rowhead(void);

static struct Row *add_row_bot(struct Matrix *mat,
		struct Row *rowabove, int value);
static struct Row *add_row_top(struct Matrix *mat,
		struct Row *rowbelow, int value);
static struct Row *add_row_mid(struct Matrix *mat,
		struct Row *rowabove, struct Row *rowbelow, int value);
static struct Row *add_initial_row(struct Matrix *mat,
		int ncols, int value);

static struct Colm *add_col_left(struct Matrix *mat,
		struct Colm *rightcol, int value);
static struct Colm *add_col_right(struct Matrix *mat,
		struct Colm *leftcol, int value);
static struct Colm *add_col_mid(struct Matrix *mat,
		struct Colm *leftcol, struct Colm *rightcol, int value);
static struct Colm *add_initial_col(struct Matrix *mat,
		int nrows, int value);

static void dispose_left_col(struct Row *currow, struct Colm *coltod);
static void dispose_right_col(struct Colm *leftcol, struct Colm *coltod);
static void dispose_mid_col(struct Colm *leftcol,
		struct Colm *rightcol, struct Colm *coltod);
static void dispose_last_col(struct Matrix *mat, struct Colm *coltod);

static void dispose_top_row(struct Colm *colstart, struct Row *rowtod);
static void dispose_bot_row(struct Row *rowabove, struct Row *rowtod);
static void dispose_mid_row(struct Row *rowabove,
		struct Row *rowbelow, struct Row *rowtod);
static void dispose_last_row(struct Matrix *mat, struct Row *rowtod);

static int Gnum = 0;

// guess ill go to mars;


void matrix_mvcc_right(struct Matrix *mat) {

	int newx;

	if (mat->curcol->right) {

		/* starting xcord of the right column */
		newx = mat->curcellxcord + mat->curcol->width + 3;

		mat->curcol = mat->curcol->right;
		mat->curcell = mat->curcell->right;

		/* checking whether the right column fits the screen */
		if (newx + mat->curcol->width + 3 > mat->xmax)
			newx = mat->curcellxcord;
			//newx = mat->xmax - mat->curcol->width - 3;

		mat->curcellxcord = newx;

	}

}

void matrix_mvcc_left(struct Matrix *mat) {

	int newx;

	if (mat->curcol->left) {

		/* starting xcord of the left column */
		newx = mat->curcellxcord;

		mat->curcol = mat->curcol->left;
		mat->curcell = mat->curcell->left;

		/* checking whether the left column fits the screen */
		newx = newx - mat->curcol->width - 3;
		if (newx < 0)
			newx = mat->curcellxcord;

		mat->curcellxcord = newx;

	}

}


void matrix_mvcc_below(struct Matrix *mat) {

	int newy;

	if (mat->currow->below) {

		/* starting ycord of the row below */
		newy = mat->curcellycord + mat->currow->height + 1;

		mat->currow = mat->currow->below;
		mat->curcell = mat->curcell->below;

		/* checking whether the row below fits the screen */
		if (newy + mat->currow->height + 1 > mat->ymax)
			newy = mat->curcellycord;
			//newy = mat->ymax - mat->currow->height - 1;

		mat->curcellycord = newy;

	}

}

void matrix_mvcc_above(struct Matrix *mat) {

	int newy;

	if (mat->currow->above) {

		newy = mat->curcellycord;

		mat->currow = mat->currow->above;
		mat->curcell = mat->curcell->above;

		newy = newy - mat->currow->height - 1;
		if (newy < 0)
			newy = mat->curcellycord;

		mat->curcellycord = newy;

	}

}

static struct Cell *get_cell(void) {

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

static struct Row *get_rowhead(void) {

	struct Row *rowhead = NULL;

	rowhead = malloc(sizeof(struct Row));

	if (!rowhead) {
		smm_log(ERROR, "couldn't allocate memory for row");
		exit(EXIT_FAILURE);
	}

	return rowhead;
}

static struct Colm *get_colhead(void) {

	struct Colm *colhead = NULL;

	colhead = malloc(sizeof(struct Colm));

	if (!colhead) {
		smm_log(ERROR, "couldn't allocate memory for column");
		exit(EXIT_FAILURE);
	}

	return colhead;
}

static struct Colm *add_col_left(struct Matrix *mat,
		struct Colm *rightcol, int value) {

	struct Cell *firstcell = NULL, *rightcell;
	struct Cell *precell = NULL, *curcell;
	struct Row *currowhead;
	struct Colm *newcol;

	newcol = get_colhead();
	newcol->width = DEFAULT_COL_WIDTH;
	newcol->widecell = NULL;

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
		curcell->value = value;

		precell = curcell;
	}

	mat->colstart = newcol;
	newcol->right = rightcol;
	rightcol->left = newcol;
	newcol->left = NULL;

	mat->ncols += 1;
	newcol->cellstart = firstcell;
	return newcol;

}

static struct Colm *add_col_right(struct Matrix *mat,
		struct Colm *leftcol, int value) {

	struct Cell *firstcell = NULL, *leftcell;
	struct Cell *precell = NULL, *curcell;
	struct Colm *newcol;

	newcol = get_colhead();
	newcol->width = DEFAULT_COL_WIDTH;
	newcol->widecell = NULL;

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
		curcell->value = value;

		precell = curcell;
	}

	newcol->left = leftcol;
	leftcol->right = newcol;
	newcol->right = NULL;

	mat->ncols = mat->ncols + 1;
	newcol->cellstart = firstcell;
	return newcol;

}

static struct Colm *add_col_mid(struct Matrix *mat,
		struct Colm *leftcol, struct Colm *rightcol, int value) {

	struct Cell *firstcell = NULL, *leftcell, *rightcell;
	struct Cell *precell = NULL, *curcell;
	struct Colm *newcol;

	newcol = get_colhead();
	newcol->width = DEFAULT_COL_WIDTH;
	newcol->widecell = NULL;

	leftcell = leftcol->cellstart;
	rightcell = rightcol->cellstart;

	/* leftcell and rightcell will always have NULL or a pointer
	 * at the same time, so we only need to check one
	 * */

	while (leftcell) {

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
		curcell->value = value;

		precell = curcell;
		leftcell = leftcell->below;
		rightcell = rightcell->below;
	}

	newcol->left = leftcol;
	leftcol->right = newcol;
	newcol->right = rightcol;
	rightcol->left = newcol;

	mat->ncols = mat->ncols + 1;
	newcol->cellstart = firstcell;
	return newcol;

}

static struct Colm *add_initial_col(struct Matrix *mat,
		int nrows, int value) {

	struct Cell *firstcell = NULL;
	struct Cell *precell = NULL, *curcell;
	struct Row *currowhead, *firstrowhead = NULL, *prerowhead = NULL;
	struct Colm *newcol;

	newcol = get_colhead();
	newcol->width = DEFAULT_COL_WIDTH;
	newcol->widecell = NULL;

	/*
	 * if nrows is negative this loop won't break
	 * */

	for (;nrows;nrows--) {

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
		curcell->value = value;

		currowhead->cellstart = curcell;

		precell = curcell;
		prerowhead = currowhead;
		mat->height += 1;
		mat->nrows += 1;

	}

	mat->colstart = newcol;
	mat->rowstart = firstrowhead;
	newcol->left = newcol->right = NULL;

	mat->ncols = mat->ncols + 1;
	newcol->cellstart = firstcell;
	return newcol;

}

struct Colm *matrix_add_col(struct Matrix *mat,
		struct Colm *leftcol, struct Colm *rightcol, int nrows, int value) {

	if (!mat)
		return NULL;

	if (leftcol && rightcol) {
		return add_col_mid(mat, leftcol, rightcol, value);
	} else if (leftcol) {
		return add_col_right(mat, leftcol, value);
	} else if (rightcol) {
		return add_col_left(mat, rightcol, value);
	} else {
		return add_initial_col(mat, nrows, value);
	}

}

static void dispose_left_col(struct Row *currow, struct Colm *coltod) {

	struct Cell *curcell, *rightcell;

	/*
	 * its easy to loop through the rows than through cols,
	 * so we don't need rightcol
	 * */

	while (currow) {

		curcell = currow->cellstart;
		rightcell = curcell->right;
		currow->cellstart = rightcell;
		rightcell->left = NULL;
		currow = currow->below;
		dispose_cell(curcell);

	}

	free(coltod);

}

static void dispose_right_col(struct Colm *leftcol, struct Colm *coltod) {

	struct Cell *curcell, *precell, *leftcell;

	leftcell = leftcol->cellstart;
	curcell = coltod->cellstart;

	while (curcell) {

		precell = curcell;
		leftcell->right = NULL;
		leftcell = leftcell->below;
		curcell = curcell->below;
		dispose_cell(precell);

	}

	leftcol->right = NULL;
	free(coltod);

}

static void dispose_mid_col(struct Colm *leftcol,
		struct Colm *rightcol, struct Colm *coltod) {

	struct Cell *curcell, *precell, *leftcell, *rightcell;

	leftcell = leftcol->cellstart;
	rightcell = rightcol->cellstart;
	curcell = coltod->cellstart;

	while (curcell) {

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

	free(coltod);

}

static void dispose_last_col(struct Matrix *mat, struct Colm *coltod) {

	struct Cell *curcell;
	struct Row *currow, *prerow;

	currow = mat->rowstart;

	while (currow) {

		prerow = currow;
		curcell = currow->cellstart;
		currow = currow->below;
		dispose_cell(curcell);
		free(prerow);

	}

	free(coltod);

	mat->rowstart = NULL;
	mat->colstart = NULL;

}


void matrix_dispose_col(struct Matrix *mat, struct Colm *coltod) {

	struct Colm *leftcol, *rightcol;

	if (!coltod) {
		smm_log(WARN, "matrix_dispose_col was called with a NULL coltod");
		return;
	}

	/*
	 * always move the curcol from the coltod before calling to dispose it
	 * ie, call matrix_mvcc* before disposing it
	 * */

	if (coltod == mat->curcol) {
		smm_log(WARN, "matrix_dispose_col was called with mat->curcol as coltod");
		return;
	}


	leftcol = coltod->left;
	rightcol = coltod->right;

	if (leftcol && rightcol) {
		dispose_mid_col(leftcol, rightcol, coltod);
	} else if (leftcol) {
		dispose_right_col(leftcol, coltod);
	} else if (rightcol) {
		mat->colstart = coltod->right;
		dispose_left_col(mat->rowstart, coltod);
	} else {
		dispose_last_col(mat, coltod);
	}

	mat->ncols -= 1;

}

static void dispose_top_row(struct Colm *curcol, struct Row *rowtod) {

	struct Cell *curcell, *cellbelow;

	while (curcol) {

		curcell = curcol->cellstart;
		cellbelow = curcell->below;
		curcol->cellstart = cellbelow;
		cellbelow->above = NULL;
		curcol = curcol->right;
		dispose_cell(curcell);

	}

	free(rowtod);

}

static void dispose_bot_row(struct Row *rowabove, struct Row *rowtod) {

	struct Cell *curcell, *precell, *cellabove;

	cellabove = rowabove->cellstart;
	curcell=rowtod->cellstart;

	while (curcell) {

		precell = curcell;
		cellabove->below = NULL;
		cellabove = cellabove->right;
		curcell = curcell->right;
		dispose_cell(precell);

	}

	rowabove->below = NULL;
	free(rowtod);

}

static void dispose_mid_row(struct Row *rowabove,
		struct Row *rowbelow, struct Row *rowtod) {

	struct Cell *curcell, *precell, *cellabove, *cellbelow;

	cellabove = rowabove->cellstart;
	cellbelow = rowbelow->cellstart;
	curcell = rowtod->cellstart;

	while (curcell) {

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

	free(rowtod);

}

static void dispose_last_row(struct Matrix *mat, struct Row *rowtod) {

	struct Cell *curcell;
	struct Colm *curcol, *precol;

	curcol = mat->colstart;

	while (curcol) {

		precol = curcol;
		curcell = curcol->cellstart;
		curcol = curcol->right;
		dispose_cell(curcell);
		free(precol);

	}

	free(rowtod);

	mat->rowstart = NULL;
	mat->colstart = NULL;

}


void matrix_dispose_row(struct Matrix *mat, struct Row *rowtod) {

	struct Row *rowbelow, *rowabove;

	if (!rowtod) {
		smm_log(WARN, "matrix_dispose_row was called with a NULL rowtod");
		return;
	}

	/*
	 * always move the currow from the rowtod before calling to dispose it
	 * ie, call matrix_mvcc* before disposing it
	 * */

	if (rowtod == mat->currow) {
		smm_log(WARN, "matrix_dispose_row was called with mat->currow as rowtod");
		return;
	}

	rowabove = rowtod->above;
	rowbelow = rowtod->below;

	if (rowabove && rowbelow) {
		dispose_mid_row(rowabove, rowbelow, rowtod);
	} else if (rowabove) {
		dispose_bot_row(rowabove, rowtod);
	} else if (rowbelow) {
		mat->rowstart = rowtod->below;
		dispose_top_row(mat->colstart, rowtod);
	} else {
		dispose_last_row(mat, rowtod);
	}

	mat->nrows -= 1;
}

static struct Row *add_row_bot(struct Matrix *mat,
		struct Row *rowabove, int value) {

	struct Cell *firstcell = NULL, *cellabove;
	struct Cell *precell = NULL, *curcell;
	struct Row *newrow;

	newrow = get_rowhead();
	newrow->height = DEFAULT_ROW_HEIGHT;
	newrow->highcell = NULL;

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
		curcell->value = value;

		precell = curcell;
	}

	newrow->above = rowabove;
	rowabove->below = newrow;
	newrow->below = NULL;

	mat->nrows += 1;
	newrow->cellstart = firstcell;
	return newrow;

}

static struct Row *add_row_top(struct Matrix *mat,
		struct Row *rowbelow, int value) {

	struct Cell *firstcell = NULL, *cellbelow;
	struct Cell *precell = NULL, *curcell;
	struct Colm *curcolhead;
	struct Row *newrow;

	newrow = get_rowhead();
	newrow->height = DEFAULT_ROW_HEIGHT;
	newrow->highcell = NULL;

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
		curcell->value = value;

		precell = curcell;
	}

	mat->rowstart = newrow;
	newrow->below = rowbelow;
	rowbelow->above = newrow;
	newrow->above = NULL;

	mat->nrows += 1;
	newrow->cellstart = firstcell;
	return newrow;

}

static struct Row *add_row_mid(struct Matrix *mat,
		struct Row *rowabove, struct Row *rowbelow, int value) {

	struct Cell *firstcell = NULL, *cellabove, *cellbelow;
	struct Cell *precell = NULL, *curcell;
	struct Row *newrow;

	newrow = get_rowhead();
	newrow->height = DEFAULT_ROW_HEIGHT;
	newrow->highcell = NULL;

	cellabove = rowabove->cellstart;
	cellbelow = rowbelow->cellstart;

	/* cellabove and cellbelow will always have NULL or a pointer
	 * at the same time, so we only need to check one
	 * */

	while (cellabove) {
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
		curcell->value = value;

		precell = curcell;
		cellabove = cellabove->right;
		cellbelow = cellbelow->right;
	}

	newrow->above = rowabove;
	rowabove->below = newrow;
	newrow->below = rowbelow;
	rowbelow->above = newrow;

	mat->nrows += 1;
	newrow->cellstart = firstcell;
	return newrow;

}

static struct Row *add_initial_row(struct Matrix *mat,
		int ncols, int value) {

	struct Cell *firstcell = NULL;
	struct Cell *precell = NULL, *curcell;
	struct Colm *curcolhead, *firstcolhead = NULL, *precolhead = NULL;
	struct Row *newrow;

	newrow = get_rowhead();
	newrow->height = DEFAULT_ROW_HEIGHT;
	newrow->highcell = NULL;

	/*
	 * if ncols is negative this loop won't break
	 * */

	for (;ncols;ncols--) {

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
		curcell->value = value;

		curcolhead->cellstart = curcell;

		precell = curcell;
		precolhead = curcolhead;
		mat->width += 1;
		mat->ncols += 1;
	}

	mat->rowstart = newrow;
	mat->colstart = firstcolhead;
	newrow->above = newrow->below = NULL;

	mat->nrows += 1;
	newrow->cellstart = firstcell;
	return newrow;

}

struct Row *matrix_add_row(struct Matrix *mat,
		struct Row *rowabove, struct Row *rowbelow, int ncols, int value) {

	if (!mat) {
		smm_log(WARN, "matrix_add_row was called with NULL mat");
		return NULL;
	}

	if (rowabove && rowbelow) {
		return add_row_mid(mat, rowabove, rowbelow, value);
	} else if (rowabove) {
		return add_row_bot(mat, rowabove, value);
	} else if (rowbelow) {
		return add_row_top(mat, rowbelow, value);
	} else {
		return add_initial_row(mat, ncols, value);
	}

}

struct Matrix *matrix_multiply(const struct Matrix *leftmat,
		const struct Matrix *rightmat) {

	struct Matrix *result;
	struct Row *resrow;
	struct Cell *rescell;
	struct Row *lmatrow;
	struct Colm *rmatcol;
	struct Cell *lmatcell;
	struct Cell *rmatcell;

	if (!leftmat || !rightmat)
		return NULL;

	if (leftmat->ncols != rightmat->nrows)
		return NULL;

	result = matrix_create(rightmat->nrows, leftmat->ncols, 0);

	resrow = result->rowstart;
	lmatrow = leftmat->rowstart;

	while (resrow) {

		rescell = resrow->cellstart;
		rmatcol = rightmat->colstart;

		while (rescell) {

			rmatcell = rmatcol->cellstart;
			lmatcell = lmatrow->cellstart;

			while (rmatcell) {

				rescell->value += lmatcell->value * rmatcell->value;

				lmatcell = lmatcell->right;
				rmatcell = rmatcell->below;
			}

			rmatcol = rmatcol->right;
			rescell = rescell->right;
		}

		resrow = resrow->below;
		lmatrow = lmatrow->below;

	}

	return result;

}

void matrix_dispose(struct Matrix *mat) {

	struct Row *rowtod;
	struct Cell *curcell, *precell;

	if (!mat) {
		smm_log(WARN,"matrix_dispose was called with a NULL mat");
		return;
	}

	/*
	 * make sure to detach the matrix form matrix list before the
	 * call to matrix_dispose
	 * */

	if (mat->left || mat->right) {
		smm_log(WARN, "seems like matrix_dispose was called with a non-detached mat");
	}

	rowtod = mat->rowstart->below;

	dispose_last_row(mat, mat->rowstart);

	while (rowtod) {

		curcell=rowtod->cellstart;

		while (curcell) {

			precell = curcell;
			curcell = curcell->right;
			dispose_cell(precell);

		}

		free(rowtod);

	}

	free(mat);

}

struct Matrix *matrix_create(int nrows, int ncols, int value) {

	struct Matrix *mat = malloc(sizeof(struct Matrix));
	struct Row *prerowhead;
	int i;

	if (nrows <= 0 || ncols <= 0) {
		smm_log(WARN, "matrix_create was called with nrows: %d and ncols: %d", nrows, ncols);
		return NULL;
	}

	if (mat) {

		mat->width = 0;
		mat->ncols = 0;
		mat->nrows = 0;
		mat->right = mat->left = NULL;

		prerowhead = matrix_add_row(mat, NULL, NULL, ncols, value);
		for(i=1;i<nrows;i++) {
			prerowhead = matrix_add_row(mat, prerowhead, NULL, 0, value);
		}

		mat->curcol = mat->colstart;
		mat->currow = mat->rowstart;
		mat->curcell = mat->rowstart->cellstart;
		mat->curcellxcord = mat->curcellycord = 0;
		mat->xmax = mat->ymax = 0;

		mat->selected = 0;
		sprintf(mat->name, "Matrix %0*d", 3, Gnum);
		Gnum++;

	} else {
		exit(EXIT_FAILURE);
	}

	return mat;
}
