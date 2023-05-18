#include<stdint.h>

typedef struct matrix matrix;
typedef struct cell cell;
typedef struct row row;
typedef struct colm colm;

struct colm {
	int width;
	cell *widecell;
	cell *cellstart;
	colm *right;
	colm *left;
};

struct row {
	int height;
	cell *highcell;
	cell *cellstart;
	row *above;
	row *below;
};

struct matrix {
	uint16_t nrows;
	uint16_t ncols;
	uint16_t width;
	uint16_t height;
	uint16_t curcellxcord;
	uint16_t curcellycord;
	float curcellxratio;
	float curcellyratio;
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

void disposerow(matrix *mat, row *currow);
void disposecol(matrix *mat, colm *curcol);
matrix *makematrix(unsigned int nrows, unsigned int ncols);
row *addrow(matrix *mat, row *rowabove, row *rowbelow);
colm *addcol(matrix *mat, colm *leftcol, colm *rightcol);

