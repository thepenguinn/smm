#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "drw.h"
#include "log.h"

/*
char char_top_l[]  = "┏" ;
char char_top_r[]  = "┓" ;
char char_bot_l[]  = "┗" ;
char char_bot_r[]  = "┛" ;
char char_minus[]  = "━" ;
char char_pipe[]   = "┃" ;
char char_space    = ' ' ;
*/

/*
char *char_symbols[] = { 
	[CHAR_CORNER_TOPLEFT]   = "+",
	[CHAR_CORNER_TOPRIGHT]  = "+",
	[CHAR_CORNER_BOTRIGHT]  = "+",
	[CHAR_CORNER_BOTLEFT]   = "+",
	[CHAR_EDGE_TOP]         = "-",
	[CHAR_EDGE_RIGHT]       = "|",
	[CHAR_EDGE_BOT]         = "-",
	[CHAR_EDGE_LEFT]        = "|",
	[CHAR_MINUS]            = "-",
	[CHAR_PIPE]             = "|",
	[CHAR_CROSS]            = "+"
};
*/

/*
 * TODO: Need to make these as variables so they can be tweaked from the
 * command line arguments and config file
 *
 * Or let the user specify their own characters to draw the seperators
 * and the box.
 * */

static const char *char_symbols[CHAR_END] = {
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


static char *colors[COLOR_END] = {
	[COLOR_BRT_PINK]     = "#EF6FF9",
	[COLOR_BRT_YELLOW]   = "#EBF980",
	[COLOR_BRT_WHITE]    = "#DDDDDD",
	[COLOR_BRT_GREY]     = "#616162",
	[COLOR_BRT_GREEN]    = "#10AF76",
	[COLOR_BRT_VIOLET]   = "#7571F9",
	[COLOR_DIM_PINK]     = "#AD58B3",
	[COLOR_DIM_WHITE]    = "#979797",
	[COLOR_DIM_GREY]     = "#3C3C3A",
	[COLOR_DIM_GREEN]    = "#036B46",
	[COLOR_DIM_VIOLET]   = "#514CC0",
	[COLOR_DIM_BROWN]    = "#3C3C3A",
};

static int16_t color_pairs[PAIR_END][2] = {
	[PAIR_BRT_YELLOW_BRT_PINK]   = { COLOR_BRT_YELLOW, COLOR_BRT_PINK,   },
	[PAIR_BRT_PINK_DEFAULT_BG]   = { COLOR_BRT_PINK,   COLOR_DEFAULT_BG, },
	[PAIR_BRT_WHITE_DEFAULT_BG]  = { COLOR_BRT_WHITE,  COLOR_DEFAULT_BG, },
	[PAIR_BRT_GREY_DEFAULT_BG]   = { COLOR_BRT_GREY,   COLOR_DEFAULT_BG, },
	[PAIR_BRT_GREEN_DEFAULT_BG]  = { COLOR_BRT_GREEN,  COLOR_DEFAULT_BG, },
	[PAIR_BRT_VIOLET_DEFAULT_BG] = { COLOR_BRT_VIOLET, COLOR_DEFAULT_BG, },
	[PAIR_DIM_PINK_DEFAULT_BG]   = { COLOR_DIM_PINK,   COLOR_DEFAULT_BG, },
	[PAIR_DIM_WHITE_DEFAULT_BG]  = { COLOR_DIM_WHITE,  COLOR_DEFAULT_BG, },
	[PAIR_DIM_GREY_DEFAULT_BG]   = { COLOR_DIM_GREY,   COLOR_DEFAULT_BG, },
	[PAIR_DIM_GREEN_DEFAULT_BG]  = { COLOR_DIM_GREEN,  COLOR_DEFAULT_BG, },
	[PAIR_DIM_VIOLET_DEFAULT_BG] = { COLOR_DIM_VIOLET, COLOR_DEFAULT_BG, },
	[PAIR_DIM_BROWN_DEFAULT_BG]  = { COLOR_DIM_BROWN,  COLOR_DEFAULT_BG, },
};

static int color_schemes[SCHEME_END][ELEMENT_END] = {
	[SCHEME_DEFAULT] = {
		[ELEMENT_TITLE]                   = ColorPair(PAIR_BRT_YELLOW_BRT_PINK),
		[ELEMENT_SECTION_NORMAL]          = ColorPair(PAIR_DIM_GREY_DEFAULT_BG),
		[ELEMENT_SECTION_SELECTED]        = ColorPair(PAIR_BRT_GREY_DEFAULT_BG),
		[ELEMENT_MAIN_TITLE_NORMAL]       = ColorPair(PAIR_BRT_WHITE_DEFAULT_BG),
		[ELEMENT_MAIN_TITLE_SELECTED]     = ColorPair(PAIR_BRT_PINK_DEFAULT_BG),
		[ELEMENT_MAIN_INFO_NORMAL]        = ColorPair(PAIR_DIM_WHITE_DEFAULT_BG),
		[ELEMENT_MAIN_INFO_SELECTED]      = ColorPair(PAIR_DIM_PINK_DEFAULT_BG),
		[ELEMENT_MATRIX_NORMAL]           = ColorPair(PAIR_DIM_WHITE_DEFAULT_BG),
		[ELEMENT_MATRIX_SELECTED]         = ColorPair(PAIR_BRT_WHITE_DEFAULT_BG),
		[ELEMENT_MATRIX_SEPERATOR]        = ColorPair(PAIR_DIM_BROWN_DEFAULT_BG),
		[ELEMENT_DOTS_NORMAL]             = ColorPair(PAIR_DIM_GREY_DEFAULT_BG),
		[ELEMENT_DOTS_SELECTED]           = ColorPair(PAIR_DIM_WHITE_DEFAULT_BG),
		[ELEMENT_STASHED_TITLE_NORMAL]    = ColorPair(PAIR_BRT_VIOLET_DEFAULT_BG),
		[ELEMENT_STASHED_TITLE_SELECTED]  = ColorPair(PAIR_BRT_PINK_DEFAULT_BG),
		[ELEMENT_STASHED_INFO_NORMAL]     = ColorPair(PAIR_DIM_VIOLET_DEFAULT_BG),
		[ELEMENT_STASHED_INFO_SELECTED]   = ColorPair(PAIR_DIM_PINK_DEFAULT_BG),
		[ELEMENT_HELP_STASH_KEY]          = ColorPair(PAIR_BRT_GREEN_DEFAULT_BG),
		[ELEMENT_HELP_STASH_ACTION]       = ColorPair(PAIR_DIM_GREEN_DEFAULT_BG),
		[ELEMENT_HELP_OTHER_KEYS]         = ColorPair(PAIR_BRT_GREY_DEFAULT_BG),
		[ELEMENT_HELP_OTHER_ACTIONS]      = ColorPair(PAIR_DIM_GREY_DEFAULT_BG),
	},
	/* we will set this later */
	//[SCHEME_MONOCHROME] = NULL,
};

/*
─ ━ │ ┃ ┄ ┅ ┆ ┇ ┈ ┉ ┊ ┋ ┌ ┍ ┎ ┏
U+251x    ┐ ┑ ┒ ┓ └ ┕ ┖ ┗ ┘ ┙ ┚ ┛ ├ ┝ ┞ ┟
U+252x    ┠ ┡ ┢ ┣ ┤ ┥ ┦ ┧ ┨ ┩ ┪ ┫ ┬ ┭ ┮ ┯
U+253x    ┰ ┱ ┲ ┳ ┴ ┵ ┶ ┷ ┸ ┹ ┺ ┻ ┼ ┽ ┾ ┿
U+254x    ╀ ╁ ╂ ╃ ╄ ╅ ╆ ╇ ╈ ╉ ╊ ╋ ╌ ╍ ╎ ╏
U+255x    ═ ║ ╒ ╓ ╔ ╕ ╖ ╗ ╘ ╙ ╚ ╛ ╜ ╝ ╞ ╟
U+256x    ╠ ╡ ╢ ╣ ╤ ╥ ╦ ╧ ╨ ╩ ╪ ╫ ╬ ╭ ╮ ╯
U+257x    ╰ ╱╳ ╲ ╳ ╴ ╵ ╶ ╷ ╸ ╹ ╺ ╻ ╼ ╽ ╾ ╿
U+257x    ╰ ╱ ╲ ╳ ╴ ╵ ╶ ╷ ╸ ╹ ╺ ╻ ╼ ╽ ╾ ╿
Notes            ╳
  │
──┼────
  │
  */

void drwcell(WINDOW *win, matrix *mat) {



}

void init_colorschemes() {

	char *hexstring;
	int r, g, b;
	int i;

	start_color();
	use_default_colors();

	/* initializing colors */

	for (i = 0;i < COLOR_END; i++) {
		hexstring = colors[i];
		if (*hexstring == '#')
			hexstring++;

		sscanf(hexstring, "%02x%02x%02x", &r, &g, &b);
		init_color(i, r*1000/255, g*1000/255, b*1000/255);
	}

	/* initializing color pairs */

	/*
	 * Since we are using use_default_colors() from ncurses library, the zeroth
	 * pair doesn't seems to be overriden by assigning a new pair to it.
	 *
	 * So instead of changing the zeroth pair we will start from 1
	 * */

	for (i = 0;i < PAIR_END;i++)
		init_pair(i + 1, color_pairs[i][0], color_pairs[i][1]);

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

static void set_symbols(row *rowstart, row *rowend, colm *colstart, colm *colend, const char *symbols[]) {

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

		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MATRIX_NORMAL]);
		wprintw(win, " %*ld ", colstart->width, cellstart->value);
		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MATRIX_SEPERATOR]);

		colstart = colstart->right;
		cellstart = cellstart->right;
	}

	while (colstart && colstart->left != colend) {
		wprintw(win, "%s", char_symbols[CHAR_PIPE]);

		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MATRIX_NORMAL]);
		wprintw(win, " %*ld ", colstart->width, cellstart->value);
		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MATRIX_SEPERATOR]);

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

	const char *symbols[8];

	if (!mat)
		return;

	getmaxyx(win, ymax, xmax);

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MATRIX_SEPERATOR]);

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

void drw_topwin(WINDOW *win) {

	char *name = " smm ";

	wmove(win, 0, 0);
	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_TITLE]);
	wprintw(win, "%s", name);
	wattroff(win, A_BOLD);
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
