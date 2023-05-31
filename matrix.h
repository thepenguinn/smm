#include<stdint.h>

// #define DEFAULT_VALUE 0x100
#define DEFAULT_VALUE 1
#define DEFAULT_ROW_HEIGHT 1
#define DEFAULT_COL_WIDTH 3

struct Cell;
struct Colm;
struct Row;
struct Matrix;

struct Cell {
	long value;
	struct Cell *left;
	struct Cell *right;
	struct Cell *above;
	struct Cell *below;
};

struct Colm {
	int width;
	struct Cell *widecell;
	struct Cell *cellstart;
	struct Colm *right;
	struct Colm *left;
};

struct Row {
	int height;
	struct Cell *highcell;
	struct Cell *cellstart;
	struct Row *above;
	struct Row *below;
};

struct Matrix {
	uint16_t nrows;
	uint16_t ncols;
	uint16_t width;
	uint16_t height;

	uint16_t curcellxcord;
	uint16_t curcellycord;
	uint16_t xmax;
	uint16_t ymax;

	float curcellxratio;
	float curcellyratio;
	struct Cell *curcell;
	struct Row *rowstart;
	struct Row *currow;
	struct Colm *colstart;
	struct Colm *curcol;
	struct Matrix *left;
	struct Matrix *right;
};

void dispose_row(struct Matrix *mat, struct Row *currow);
void dispose_col(struct Matrix *mat, struct Colm *curcol);
void mvccleft(struct Matrix *mat);
void mvccright(struct Matrix *mat);
void mvccabove(struct Matrix *mat);
void mvccbelow(struct Matrix *mat);
struct Matrix *make_matrix(int nrows, int ncols, int value);
struct Matrix *matrix_multiply(const struct Matrix *leftmat,
		const struct Matrix *rightmat);
struct Row *add_row(struct Matrix *mat,
		struct Row *rowabove, struct Row *rowbelow, int ncols, int value);
struct Colm *add_col(struct Matrix *mat,
		struct Colm *leftcol, struct Colm *rightcol, int nrows, int value);
