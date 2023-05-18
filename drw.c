#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "drw.h"

/*
char char_top_l[]  = "┏" ;
char char_top_r[]  = "┓" ;
char char_bot_l[]  = "┗" ;
char char_bot_r[]  = "┛" ;
char char_minus[]  = "━" ;
char char_pipe[]   = "┃" ;
char char_space    = ' ' ;
*/

#define CHAR_CORNER_TOPLEFT	  0
#define CHAR_CORNER_TOPRIGHT  1
#define CHAR_CORNER_BOTRIGHT  2
#define CHAR_CORNER_BOTLEFT   3
#define CHAR_EDGE_TOP         4
#define CHAR_EDGE_RIGHT       5
#define CHAR_EDGE_BOT         6
#define CHAR_EDGE_LEFT        7
#define CHAR_MINUS            8
#define CHAR_PIPE             9
#define CHAR_CROSS           10

#define TOPLEFT_CORNER        0
#define TOPRIGHT_CORNER       1
#define BOTRIGHT_CORNER       2
#define BOTLEFT_CORNER        3
#define TOP_EDGE              4
#define RIGHT_EDGE            5
#define BOT_EDGE              6
#define LEFT_EDGE             7

char *char_symbols[] = { 
	[CHAR_CORNER_TOPLEFT]   = "┌",
	[CHAR_CORNER_TOPRIGHT]  = "┐",
	[CHAR_CORNER_BOTRIGHT]  = "┘",
	[CHAR_CORNER_BOTLEFT]   = "└",
	[CHAR_EDGE_TOP]         = "┬",
	[CHAR_EDGE_RIGHT]       = "┤",
	[CHAR_EDGE_BOT]         = "┴",
	[CHAR_EDGE_LEFT]        = "├",
	[CHAR_MINUS]            = "─",
	[CHAR_PIPE]             = "│",
	[CHAR_CROSS]            = "┼"
};

char char_top_l[]  = "┌" ;
char char_top_r[]  = "┐" ;
char char_bot_l[]  = "└" ;
char char_bot_r[]  = "┘" ;
char char_minus[]  = "─" ;
char char_pipe[]   = "│" ;
char char_cross[]  = "┼" ;
char char_space    = ' ' ;

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
  │
──┼────
  │
  */

void drwcell(WINDOW *win, matrix *mat) {



}

static void actualdrwcell() {

}

void drw_row_seperator(WINDOW *win, colm *colstart, colm *colend, const char *widesymbol) {

	int i;

	for (i = 0;i < colstart->width + 2;i++)
		wprintw(win, "%s", char_symbols[CHAR_MINUS]);
	colstart = colstart->right;

	while (colstart && colstart->left != colend) {
		wprintw(win, "%s", widesymbol);
		for (i = 0;i < colstart->width + 2;i++)
			wprintw(win, "%s", char_symbols[CHAR_MINUS]);
		colstart = colstart->right;
	}

}

static void set_symbols(row *rowstart, row *rowend, colm *colstart, colm *colend, char *symbols[]) {

	/* 
	 * I know this is probably inefficient but I think this is more readable I guess.... :)
	 * */
	
	if (rowstart->above && colstart->left) {
		symbols[TOP_EDGE] = symbols[LEFT_EDGE] = symbols[TOPLEFT_CORNER] = char_symbols[CHAR_CROSS];
	} else if (rowstart->above) {
		symbols[LEFT_EDGE] = symbols[TOPLEFT_CORNER] = char_symbols[CHAR_EDGE_LEFT];
		symbols[TOP_EDGE] = char_symbols[CHAR_CROSS];
	} else if (colstart->left) {
		symbols[TOP_EDGE] = symbols[TOPLEFT_CORNER] = char_symbols[CHAR_EDGE_TOP];
		symbols[LEFT_EDGE] = char_symbols[CHAR_CROSS];
	} else {
		symbols[TOPLEFT_CORNER] = char_symbols[CHAR_CORNER_TOPLEFT];
		symbols[TOP_EDGE] = char_symbols[CHAR_EDGE_TOP];
		symbols[LEFT_EDGE] = char_symbols[CHAR_EDGE_LEFT];
	}

	if (rowend->below && colend->right) {
		symbols[RIGHT_EDGE] = symbols[BOT_EDGE] = symbols[BOTRIGHT_CORNER] = char_symbols[CHAR_CROSS];
	} else if (rowend->below) {
		symbols[RIGHT_EDGE] = symbols[BOTRIGHT_CORNER] = char_symbols[CHAR_EDGE_RIGHT];
		symbols[BOT_EDGE] = char_symbols[CHAR_CROSS];
	} else if (colend->right) {
		symbols[BOT_EDGE] = symbols[BOTRIGHT_CORNER] = char_symbols[CHAR_EDGE_BOT];
		symbols[RIGHT_EDGE] = char_symbols[CHAR_CROSS];
	} else {
		symbols[BOTRIGHT_CORNER] = char_symbols[CHAR_CORNER_BOTRIGHT];
		symbols[BOT_EDGE] = char_symbols[CHAR_EDGE_BOT];
		symbols[RIGHT_EDGE] = char_symbols[CHAR_EDGE_RIGHT];
	}
	
	if (rowstart->above && colend->right) {
		symbols[TOPRIGHT_CORNER] = char_symbols[CHAR_CROSS];
	} else if (rowstart->above) {
		symbols[TOPRIGHT_CORNER] = char_symbols[CHAR_EDGE_RIGHT];
	} else if (colend->right) {
		symbols[TOPRIGHT_CORNER] = char_symbols[CHAR_EDGE_TOP];
	} else {
		symbols[TOPRIGHT_CORNER] = char_symbols[CHAR_CORNER_TOPRIGHT];
	}

	if (rowend->below && colstart->left) {
		symbols[BOTLEFT_CORNER] = char_symbols[CHAR_CROSS];
	} else if (rowend->below) {
		symbols[BOTLEFT_CORNER] = char_symbols[CHAR_EDGE_LEFT];
	} else if (colstart->left) {
		symbols[BOTLEFT_CORNER] = char_symbols[CHAR_EDGE_BOT];
	} else {
		symbols[BOTLEFT_CORNER] = char_symbols[CHAR_CORNER_BOTLEFT];
	}

}

void drw_row(WINDOW *win, colm *colstart, colm *colend, cell *cellstart) {


	if (colstart && colstart->left != colend) {

		wattron(win, COLOR_PAIR(5));
		wprintw(win, " %*ld ", colstart->width, cellstart->value);
		wattron(win, COLOR_PAIR(4));

		colstart = colstart->right;
		cellstart = cellstart->right;
	}

	while (colstart && colstart->left != colend) {
		wprintw(win, "%s", char_symbols[CHAR_PIPE]);

		wattron(win, COLOR_PAIR(5));
		wprintw(win, " %*ld ", colstart->width, cellstart->value);
		wattron(win, COLOR_PAIR(4));

		colstart = colstart->right;
		cellstart = cellstart->right;
	}

}

// void drwmatrix(WINDOW *win, matrix *mat, colm *coltostart, colm *coltostop) {
void drwmatrix(WINDOW *win, matrix *mat) {

	unsigned int xmax, ymax;
	unsigned int begx, begy;
	unsigned int endx, endy;
	unsigned int curx, cury;
	unsigned int xstart, ystart;
	unsigned int mwidth, mheight;
	struct cell *curcell;
	struct cell *cellstart;
	struct colm *colstart, *colend;
	struct row *rowstart, *rowend;

	char *symbols[8];

	if (!mat)
		return;

	getmaxyx(win, ymax, xmax);

	wattron(win, COLOR_PAIR(4));

	/* its the number of cols and rows but cords starts with 0 */
	xmax--;
	ymax--;

	endx = begx = mat->curcellxratio * xmax;
	endy = begy = mat->curcellyratio * ymax;

	/*
	 * Ensuring that we will always have the space to draw the curcell
	 * */

	if (begx + mat->curcol->width + 3 > xmax)
		endx = begx = xmax - mat->curcol->width - 3;

	if (begy + mat->currow->height + 1 > ymax)
		endy = begy = ymax - mat->currow->height - 1;

	mheight = mwidth = 0;

	cellstart = mat->curcell;

	//if (coltostart && coltostart == curcol)

	for (colstart = mat->curcol->left;colstart;colstart = colstart->left) {
		if (begx - colstart->width - 3 >= 0) {
			begx -= colstart->width + 3;
			mwidth += colstart->width + 3;
			cellstart = cellstart->left;
		} else {
			colstart = colstart->right;
			break;
		}
	}

	if (!colstart)
		colstart = mat->curcol;

	for (rowstart = mat->currow->above;rowstart;rowstart = rowstart->above) {
		if (begy - rowstart->height - 1 >= 0) {
			begy -= rowstart->height + 1;
			mheight += rowstart->height + 1;
			cellstart = cellstart->above;
		} else {
			rowstart = rowstart->below;
			break;
		}
	}

	if (!rowstart)
		rowstart = mat->currow;

	for (colend = mat->curcol;colend;colend = colend->right) {
		if (endx + colend->width + 3 <= xmax) {
			endx  += colend->width + 3;
			mwidth += colend->width + 3;
		} else {
			colend = colend->left;
			break;
		}
		if (!(colend->right))
			break;
	}

	for (rowend = mat->currow;rowend;rowend = rowend->below) {
		if (endy + rowend->height + 1 <= ymax) {
			endy += rowend->height + 1;
			mheight += rowend->height + 1;
		} else {
			rowend = rowend->above;
			break;
		}
		if (!(rowend->below))
			break;
	}
	
	fprintf(stderr, "rowstart -> %p\n", rowstart);
	fprintf(stderr, "rowend -> %p\n", rowend);
	fprintf(stderr, "colstart -> %p\n", colstart);
	fprintf(stderr, "colend -> %p\n", colend);
	begx = (xmax - mwidth) / 2;
	begy = (ymax - mheight) / 2;

	mat->curcellxcord = begx;
	mat->curcellycord = begy;

	// mat->curcellxratio = (double)(begx / (xmax + 1));
	// mat->curcellyratio = (double)(begy / (ymax + 1));

	werase(win);
	wmove(win, begy, begx);

	set_symbols(rowstart, rowend, colstart, colend, symbols);

	wprintw(win, "%s", symbols[TOPLEFT_CORNER]);
	drw_row_seperator(win, colstart, colend, symbols[TOP_EDGE]);
	wprintw(win, "%s", symbols[TOPRIGHT_CORNER]);

	wmove(win, ++begy, begx);
	wprintw(win, "%s", char_symbols[CHAR_PIPE]);
	drw_row(win, colstart, colend, cellstart);
	wprintw(win, "%s", char_symbols[CHAR_PIPE]);

	rowstart = rowstart->below;
	cellstart = cellstart->below;

	while (rowstart && rowstart->above != rowend) {

		wmove(win, ++begy, begx);

		wprintw(win, "%s", symbols[LEFT_EDGE]);
		drw_row_seperator(win, colstart, colend, char_symbols[CHAR_CROSS]);
		wprintw(win, "%s", symbols[RIGHT_EDGE]);

		wmove(win, ++begy, begx);

		wprintw(win, "%s", char_symbols[CHAR_PIPE]);
		drw_row(win, colstart, colend, cellstart);
		wprintw(win, "%s", char_symbols[CHAR_PIPE]);

		rowstart = rowstart->below;
		cellstart = cellstart->below;
	}

	wmove(win, ++begy, begx);
	wprintw(win, "%s", symbols[BOTLEFT_CORNER]);
	drw_row_seperator(win, colstart, colend, symbols[BOT_EDGE]);
	wprintw(win, "%s", symbols[BOTRIGHT_CORNER]);

	/* 
	 *  20 | 345 
	 * ----+-----
	 *  20 | 345 
	 * width of a column is the number of charaters needed to represent that number
	 * in ascii. mat->width is the sum of all of the columns.
	 *
	 * In this case 20 has 2 and 345 has 3. Therefore mat->width is 5.
	 * 
	 * There is a one space padding for each cell to the left and right. And there
	 * is a pipe symbol seperating each cell.
	 * */

}

void drwtopwin(WINDOW *win) {

	char *name = "smm\0";
	// int tee;


	// init_pair(5, COLOR_WHITE, COLOR_BLUE);
	// init_pair(5, COLOR_WHITE, COLOR_BLUE);
	wattron(win, COLOR_PAIR(5) | A_BOLD);

	// tee = wattr_get(win, );
	wmove(win, 0, 0);
	wprintw(win, "%s", name);
	wattroff(win, A_BOLD);
	wattron(win, COLOR_PAIR(3));
	wmove(win, 2, 0);
	wprintw(win, "%s", "local | stashed | news");

}

void change_cell_attr(WINDOW *win, matrix *mat, attr_t attr) {
	
	unsigned int xmax, ymax;
	unsigned int ccx, ccy;

	getmaxyx(win, ymax, xmax);

	ccx = mat->curcellxcord;
	ccy = mat->curcellycord;

	ccx += 2;
	ccy += 1;
	
	wmove(win, ccy, ccx);

	wchgat(win, mat->curcol->width, attr | COLOR_PAIR(5), 0, NULL);

}
