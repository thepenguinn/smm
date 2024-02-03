#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "draw.h"
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
	// [CHAR_CORNER_TOPLEFT]   = "╭",
	[CHAR_CORNER_TOPRIGHT]  = "┐",
	// [CHAR_CORNER_TOPRIGHT]  = "╮",
	[CHAR_CORNER_BOTRIGHT]  = "┘",
	// [CHAR_CORNER_BOTRIGHT]  = "╯",
	[CHAR_CORNER_BOTLEFT]   = "└",
	// [CHAR_CORNER_BOTLEFT]   = "╰",
	[CHAR_EDGE_TOP]         = "┬",
	[CHAR_EDGE_RIGHT]       = "┤",
	[CHAR_EDGE_BOT]         = "┴",
	[CHAR_EDGE_LEFT]        = "├",
	[CHAR_MINUS]            = "─",
	[CHAR_PIPE]             = "│",
	[CHAR_CROSS]            = "┼",
	[CHAR_DOT]              = "•"
};

/*
••••••••••••••••••••••••
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


static const char *colors[COLOR_END] = {
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
		[ELEMENT_SECTION_NORMAL]          = ColorPair(PAIR_BRT_GREY_DEFAULT_BG),
		[ELEMENT_SECTION_SELECTED]        = ColorPair(PAIR_DIM_WHITE_DEFAULT_BG),
		[ELEMENT_SECTION_SEPERATOR]       = ColorPair(PAIR_DIM_GREY_DEFAULT_BG),
		[ELEMENT_MAIN_TITLE_NORMAL]       = ColorPair(PAIR_BRT_WHITE_DEFAULT_BG),
		[ELEMENT_MAIN_TITLE_SELECTED]     = ColorPair(PAIR_BRT_PINK_DEFAULT_BG),
		[ELEMENT_MAIN_INFO_NORMAL]        = ColorPair(PAIR_DIM_WHITE_DEFAULT_BG),
		[ELEMENT_MAIN_INFO_SELECTED]      = ColorPair(PAIR_DIM_PINK_DEFAULT_BG),
		[ELEMENT_MATRIX_NORMAL]           = ColorPair(PAIR_DIM_WHITE_DEFAULT_BG) | A_NORMAL,
		[ELEMENT_MATRIX_SELECTED]         = ColorPair(PAIR_BRT_WHITE_DEFAULT_BG) | A_BOLD,
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
	/* ill set this later */
	//[SCHEME_MONOCHROME] = NULL,
};

static void set_symbols(struct Row *rowstart, struct Row *rowend,
		struct Colm *colstart, struct Colm *colend, const char *symbols[]);
static void draw_row_seperator(WINDOW *win,
		struct Colm *colstart, struct Colm *colend, const char *widesymbol);
static void draw_row(WINDOW *win,
		struct Colm *colstart, struct Colm *colend, struct Cell *cellstart);
static void set_colstart(struct Blueprint *bprint);
static void set_colend(struct Blueprint *bprint);
static void set_rowstart(struct Blueprint *bprint);
static void set_rowend(struct Blueprint *bprint);
static void align_matrix(struct Blueprint *bprint);
static void draw_matrix(WINDOW *win, struct Blueprint *bprint);

void draw_init_colorschemes(void) {

	const char *hexstring;
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

static void draw_row_seperator(WINDOW *win,
		struct Colm *colstart, struct Colm *colend, const char *widesymbol) {

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

static void set_symbols(struct Row *rowstart, struct Row *rowend,
		struct Colm *colstart, struct Colm *colend, const char *symbols[]) {

	/*
	struct Row *rowstart;
	struct Row *rowend;
	struct Colm *colstart;
	struct Colm *colend;
	*/

	/*
	 * I know this is probably inefficient but I think its more readable... I guess... :)
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

static void draw_row(WINDOW *win,
		struct Colm *colstart, struct Colm *colend, struct Cell *cellstart) {


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

static void set_colstart(struct Blueprint *bprint) {

	if (!(bprint->colstart)) {
		smm_log(WARN, "set_colstart: bprint->colstart is NULL");
		return;
	}

	while (bprint->colstart->left
			&& bprint->begx - bprint->colstart->left->width - 3 >= 0) {

			bprint->colstart = bprint->colstart->left;
			bprint->begx -= bprint->colstart->width + 3;
			bprint->cellstart = bprint->cellstart->left;

	}

}

static void set_colend(struct Blueprint *bprint) {

	if (!(bprint->colend)) {
		smm_log(WARN, "set_colend: bprint->colend is NULL");
		return;
	}

	while (bprint->colend->right
			&& bprint->endx + bprint->colend->right->width
			+ 3 <= bprint->xmax) {

			bprint->colend = bprint->colend->right;
			bprint->endx += bprint->colend->width + 3;

	}

}

static void set_rowstart(struct Blueprint *bprint) {

	if (!bprint->rowstart) {
		smm_log(WARN, "set_rowstart: bprint->rowstart is NULL");
		return;
	}

	while (bprint->rowstart->above && bprint->begy
			- bprint->rowstart->above->height - 1 >= 0) {

			bprint->rowstart = bprint->rowstart->above;
			bprint->begy -= bprint->rowstart->height + 1;
			bprint->cellstart = bprint->cellstart->above;

	}

}

static void set_rowend(struct Blueprint *bprint) {

	if (!bprint->rowend) {
		smm_log(WARN, "set_rowend: bprint->rowend is NULL");
		return;
	}

	while (bprint->rowend->below && bprint->endy
			+ bprint->rowend->below->height + 1 <= bprint->ymax) {

			bprint->rowend = bprint->rowend->below;
			bprint->endy += bprint->rowend->height + 1;

	}

}

void draw_left_matrix(WINDOW *win, struct Matrix *mat,
		struct Colm *colend, struct Cell *cellstart, int begx) {

	struct Blueprint bprint;

	bprint.begx = begx;
	bprint.begy = mat->curcellycord;
	/* end x cord of the colstart column */
	bprint.endx = begx + colend->width + 3;
	/* end y cord of the current row */
	bprint.endy = bprint.begy + mat->currow->height + 1;

	bprint.colstart = bprint.colend = colend;
	bprint.rowstart = bprint.rowend = mat->currow;
	bprint.cellstart = cellstart;

	bprint.xmax = mat->xmax;
	bprint.ymax = mat->ymax;

	set_colstart(&bprint);
	set_rowstart(&bprint);
	set_rowend(&bprint);

	draw_matrix(win, &bprint);

}

void draw_right_matrix(WINDOW *win, struct Matrix *mat,
		struct Colm *colstart, struct Cell *cellstart, int begx) {

	struct Blueprint bprint;

	bprint.begx = begx;
	/* end x cord of the colstart column */
	bprint.endx = begx + colstart->width + 3;
	bprint.begy = mat->curcellycord;
	/* end y cord of the current row */
	bprint.endy = bprint.begy + mat->currow->height + 1;

	bprint.colend = bprint.colstart = colstart;
	bprint.rowstart = bprint.rowend = mat->currow;
	bprint.cellstart = cellstart;

	bprint.xmax = mat->xmax;
	bprint.ymax = mat->ymax;

	set_rowstart(&bprint);
	set_colend(&bprint);
	set_rowend(&bprint);

	draw_matrix(win, &bprint);

}

void draw_above_matrix(WINDOW *win, struct Matrix *mat,
		struct Row *rowend, struct Cell *cellstart, int begy) {

	struct Blueprint bprint;

	bprint.begy = begy;
	bprint.endy = begy + rowend->height + 1;
	bprint.begx = mat->curcellxcord;
	bprint.endx = bprint.begx + mat->curcol->width + 3;

	bprint.rowstart = bprint.rowend = rowend;
	bprint.colstart = bprint.colend = mat->curcol;
	bprint.cellstart = cellstart;

	bprint.xmax = mat->xmax;
	bprint.ymax = mat->ymax;

	set_rowstart(&bprint);
	set_colstart(&bprint);
	set_colend(&bprint);

	draw_matrix(win, &bprint);

}

void draw_below_matrix(WINDOW *win, struct Matrix *mat,
		struct Row *rowstart, struct Cell *cellstart, int begy) {

	struct Blueprint bprint;

	bprint.begy = begy;
	bprint.endy = begy + rowstart->height + 1;
	bprint.begx = mat->curcellxcord;
	bprint.endx = bprint.begx + mat->curcol->width + 3;

	bprint.rowstart = bprint.rowend = rowstart;
	bprint.colstart = bprint.colend = mat->curcol;
	bprint.cellstart = cellstart;

	bprint.xmax = mat->xmax;
	bprint.ymax = mat->ymax;

	set_colstart(&bprint);
	set_rowstart(&bprint);
	set_colend(&bprint);

	draw_matrix(win, &bprint);

}

static void align_matrix(struct Blueprint *bprint) {

	int mwidth, mheight;

	if (!bprint->colstart->left && !bprint->colend->right) {

		mwidth = bprint->endx - bprint->begx + 1;
		bprint->ccx = bprint->ccx
			- (bprint->begx - ((bprint->xmax - mwidth) / 2));
		bprint->begx = (bprint->xmax - mwidth) / 2;

	} else if (!bprint->colstart->left) {

		bprint->ccx = bprint->ccx - bprint->begx;
		bprint->endx = bprint->endx - bprint->begx;
		bprint->begx = 0;
		set_colend(bprint);

	} else if (!bprint->colend->right) {

		bprint->ccx = bprint->ccx + (bprint->xmax - bprint->endx);
		bprint->begx = bprint->begx + (bprint->xmax - bprint->endx);
		bprint->endx = bprint->xmax;
		set_colstart(bprint);

	}

	if (!bprint->rowstart->above && !bprint->rowend->below) {

		mheight = bprint->endy - bprint->begy + 1;
		bprint->ccy = bprint->ccy
			- (bprint->begy - ((bprint->ymax - mheight) / 2));
		bprint->begy = (bprint->ymax - mheight) / 2;

	} else if (!bprint->rowstart->above) {

		bprint->ccy = bprint->ccy - bprint->begy;
		bprint->endy = bprint->endy - bprint->begy;
		bprint->begy = 0;
		set_rowend(bprint);

	} else if (!bprint->rowend->below) {

		bprint->ccy = bprint->ccy + (bprint->ymax - bprint->endy);
		bprint->begy = bprint->begy + (bprint->ymax - bprint->endy);
		bprint->endy = bprint->ymax;
		set_rowstart(bprint);

	}

}

void draw_whole_matrix(WINDOW *win, struct Matrix *mat) {

	struct Blueprint bprint;

	getmaxyx(win, bprint.ymax, bprint.xmax);

	/* its the number of cols and rows but cords starts with 0 */

	bprint.ymax--;
	bprint.xmax--;

	/*
	 * if the current xmax or ymax is not equal to the previous xmax
	 * and ymax, we need to find the new curcell coordinates
	 * */

	if (mat->xmax != bprint.xmax) {
		if (mat->xmax)
			bprint.begx = (float)mat->curcellxcord / mat->xmax * bprint.xmax;
		else
			bprint.begx = 0;
		mat->xmax = bprint.xmax;
	} else
		bprint.begx = mat->curcellxcord;

	if (mat->ymax != bprint.ymax) {
		if (mat->ymax)
			bprint.begy = (float)mat->curcellycord / mat->ymax * bprint.ymax;
		else
			bprint.begy = 0;
		mat->ymax = bprint.ymax;
	} else
		bprint.begy = mat->curcellycord;

	bprint.colstart = bprint.colend = mat->curcol;
	bprint.rowstart = bprint.rowend = mat->currow;
	bprint.cellstart = mat->curcell;

	if (bprint.begx < 0)
		bprint.begx = 0;

	if (bprint.begy < 0)
		bprint.begy = 0;

	if (bprint.begx + bprint.colstart->width + 3 > bprint.xmax)
		bprint.begx = bprint.xmax - bprint.colstart->width - 3;

	if (bprint.begy + bprint.rowstart->height + 1 > bprint.xmax)
		bprint.begy = bprint.ymax - bprint.rowstart->height - 1;

	bprint.ccx = bprint.begx;
	bprint.ccy = bprint.begy;

	bprint.endx = bprint.begx + bprint.colstart->width + 3;
	bprint.endy = bprint.begy + bprint.rowstart->height + 1;

	set_colstart(&bprint);
	set_rowstart(&bprint);
	set_colend(&bprint);
	set_rowend(&bprint);

	align_matrix(&bprint);
	draw_matrix(win, &bprint);

	mat->curcellxcord = bprint.ccx;
	mat->curcellycord = bprint.ccy;

}

static void draw_matrix(WINDOW *win, struct Blueprint *bprint) {

	struct Row *rowstart = bprint->rowstart;
	struct Row *rowend = bprint->rowend;
	struct Colm *colstart = bprint->colstart;
	struct Colm *colend = bprint->colend;
	struct Cell *cellstart = bprint->cellstart;

	int begx = bprint->begx;
	int begy = bprint->begy;

	const char *symbols[8];

	set_symbols(rowstart, rowend, colstart, colend, symbols);

	werase(win);
	wmove(win, begy, begx);

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MATRIX_SEPERATOR]);

	wprintw(win, "%s", symbols[TOPLEFT_CORNER]);
	draw_row_seperator(win, colstart, colend, symbols[TOP_EDGE]);
	wprintw(win, "%s", symbols[TOPRIGHT_CORNER]);

	wmove(win, ++begy, begx);
	wprintw(win, "%s", char_symbols[CHAR_PIPE]);
	draw_row(win, colstart, colend, cellstart);
	wprintw(win, "%s", char_symbols[CHAR_PIPE]);

	rowstart = rowstart->below;
	cellstart = cellstart->below;

	while (rowstart && rowstart->above != rowend) {

		wmove(win, ++begy, begx);

		wprintw(win, "%s", symbols[LEFT_EDGE]);
		draw_row_seperator(win, colstart, colend, char_symbols[CHAR_CROSS]);
		wprintw(win, "%s", symbols[RIGHT_EDGE]);

		wmove(win, ++begy, begx);

		wprintw(win, "%s", char_symbols[CHAR_PIPE]);
		draw_row(win, colstart, colend, cellstart);
		wprintw(win, "%s", char_symbols[CHAR_PIPE]);

		rowstart = rowstart->below;
		cellstart = cellstart->below;
	}

	wmove(win, ++begy, begx);
	wprintw(win, "%s", symbols[BOTLEFT_CORNER]);
	draw_row_seperator(win, colstart, colend, symbols[BOT_EDGE]);
	wprintw(win, "%s", symbols[BOTRIGHT_CORNER]);

}


void draw_topwin(WINDOW *win) {

	char name[] = " smm ";

	wmove(win, 0, 0);
	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_TITLE]);
	wprintw(win, "%s", name);
	wattroff(win, A_BOLD);
	wmove(win, 2, 0);

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_SECTION_SELECTED]);
	wprintw(win, "%s", "0 local");

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_SECTION_SEPERATOR]);
	wprintw(win, "%s", " | ");

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_SECTION_NORMAL]);
	wprintw(win, "%s", "0 stashed");

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_SECTION_SEPERATOR]);
	wprintw(win, "%s", " | ");

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_SECTION_NORMAL]);
	wprintw(win, "%s", "0 wip");

}

void draw_change_cell_attr(WINDOW *win, struct Matrix *mat, int attr_idx) {

	unsigned int ccx, ccy;

	ccx = mat->curcellxcord;
	ccy = mat->curcellycord;

	ccx += 2;
	ccy += 1;

	wmove(win, ccy, ccx);

	wchgat(win, mat->curcol->width, color_schemes[SCHEME_DEFAULT][attr_idx], 0, NULL);

}

void draw_page_hints(WINDOW *win, struct Menu *menu) {

	int maxitems;
	int leftpages, rightpages;

	if (!menu->curmat)
		return;

	maxitems = (menu->ymax + 1) / 3;

	leftpages = menu->curitemidx / maxitems;
	rightpages = ((menu->totalitems + maxitems - 1) / maxitems) - leftpages - 1;

	wmove(win, 0, 0);
	wclrtoeol(win);

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_DOTS_NORMAL]);
	while (leftpages) {
		wprintw(win, "%s", char_symbols[CHAR_DOT]);
		leftpages--;
	}

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_DOTS_SELECTED]);
	wprintw(win, "%s", char_symbols[CHAR_DOT]);

	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_DOTS_NORMAL]);
	while (rightpages) {
		wprintw(win, "%s", char_symbols[CHAR_DOT]);
		rightpages--;
	}

}


void draw_main_menu(WINDOW *win, struct Menu *menu) {

	/*
	 * TODO: clean menu->cury and menu->ymax
	 * */

	int ymax;
	int cury;
	int maxitems;
	int itemsabove;
	int itemsbelow;

	struct Matrix *mstart, *mend;
	struct Matrix *mat;

	if (!menu) {
		smm_log(WARN, "draw_main_menu was called with a NULL menu");
		return;
	}

	mat = menu->curmat;

	if (!mat)
		return;

	ymax = getmaxy(win);

	if (ymax < 3) {
		return;
	}

	maxitems = (ymax + 1) / 3;

	menu->cury = (menu->curitemidx % maxitems) * 3;
	menu->ymax = ymax;

	cury = menu->cury;

	itemsabove = cury / 3;

	if (menu->totalitems - menu->curitemidx >= maxitems - itemsabove)
		itemsbelow = maxitems - itemsabove - 1;
	else
		itemsbelow = menu->totalitems - menu->curitemidx - 1;

	werase(win);

	wmove(win, cury, 0);
	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_INFO_SELECTED]);
	wprintw(win, "%s", char_symbols[CHAR_PIPE]);

	wmove(win, cury, 2);
	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_TITLE_SELECTED]);
	wprintw(win, "%s", mat->name);

	wmove(win, cury + 1, 0);
	wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_INFO_SELECTED]);
	wprintw(win, "%s", char_symbols[CHAR_PIPE]);

	wmove(win, cury + 1, 2);
	wprintw(win, "Rows: %d Cols: %d", mat->nrows, mat->ncols);


	while (itemsabove > 0) {

		cury -= 3;
		mat = mat->left;

		wmove(win, cury, 2);
		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_TITLE_NORMAL]);
		wprintw(win, "%s", mat->name);

		wmove(win, cury + 1, 2);
		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_INFO_NORMAL]);
		wprintw(win, "Rows: %d Cols: %d", mat->nrows, mat->ncols);

		itemsabove--;

	}

	mat = menu->curmat;
	cury = menu->cury;

	while (itemsbelow > 0) {

		cury += 3;
		mat = mat->right;

		wmove(win, cury, 2);
		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_TITLE_NORMAL]);
		wprintw(win, "%s", mat->name);

		wmove(win, cury + 1, 2);
		wattron(win, color_schemes[SCHEME_DEFAULT][ELEMENT_MAIN_INFO_NORMAL]);
		wprintw(win, "Rows: %d Cols: %d", mat->nrows, mat->ncols);

		itemsbelow--;

	}

}
