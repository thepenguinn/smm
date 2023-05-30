/*
 * TODO: Need to make these as variables so they can be tweaked from the
 * command line arguments and config file
 * */

/*
 * we are using use_default_colors() from ncurses library
 * */

#define COLOR_DEFAULT_BG  -1
#define COLOR_DEFAULT_FG  -1

/*
 * Since we are using use_default_colors() from ncurses library, the zeroth
 * pair doesn't seems to be overriden by assigning a new pair to it.
 *
 * So instead of changing the zeroth pair we will start from 1
 * */

#define ColorPair(x) COLOR_PAIR((x)+1)


/*
 * This will override the predefined colors from 1-7 in the ncurses library
 * */

enum Colors {
	COLOR_BRT_PINK,
	COLOR_BRT_YELLOW,
	COLOR_BRT_WHITE,
	COLOR_BRT_GREY,
	COLOR_BRT_GREEN,
	COLOR_BRT_VIOLET,
	COLOR_DIM_PINK,
	COLOR_DIM_WHITE,
	COLOR_DIM_GREY,
	COLOR_DIM_GREEN,
	COLOR_DIM_VIOLET,
	COLOR_DIM_BROWN,
	COLOR_END,
};

/*
 * When using */

enum ColorPairs {
	/*   fg            bg    */
	PAIR_BRT_YELLOW_BRT_PINK,
	PAIR_BRT_PINK_DEFAULT_BG,
	PAIR_BRT_WHITE_DEFAULT_BG,
	PAIR_BRT_GREY_DEFAULT_BG,
	PAIR_BRT_GREEN_DEFAULT_BG,
	PAIR_BRT_VIOLET_DEFAULT_BG,
	PAIR_DIM_PINK_DEFAULT_BG,
	PAIR_DIM_WHITE_DEFAULT_BG,
	PAIR_DIM_GREY_DEFAULT_BG,
	PAIR_DIM_GREEN_DEFAULT_BG,
	PAIR_DIM_VIOLET_DEFAULT_BG,
	PAIR_DIM_BROWN_DEFAULT_BG,
	PAIR_END,
};

enum ColorSchemes {
	SCHEME_DEFAULT,
	// SCHEME_MONOCHROME,
	SCHEME_END,
};

enum ColorElements {
	ELEMENT_TITLE,
	ELEMENT_SECTION_NORMAL,
	ELEMENT_SECTION_SELECTED,
	ELEMENT_MAIN_TITLE_NORMAL,
	ELEMENT_MAIN_TITLE_SELECTED,
	ELEMENT_MAIN_INFO_NORMAL,
	ELEMENT_MAIN_INFO_SELECTED,
	ELEMENT_MATRIX_NORMAL,
	ELEMENT_MATRIX_SELECTED,
	ELEMENT_MATRIX_SEPERATOR,
	ELEMENT_DOTS_NORMAL,
	ELEMENT_DOTS_SELECTED,
	ELEMENT_STASHED_TITLE_NORMAL,
	ELEMENT_STASHED_TITLE_SELECTED,
	ELEMENT_STASHED_INFO_NORMAL,
	ELEMENT_STASHED_INFO_SELECTED,
	ELEMENT_HELP_STASH_KEY,
	ELEMENT_HELP_STASH_ACTION,
	ELEMENT_HELP_OTHER_KEYS,
	ELEMENT_HELP_OTHER_ACTIONS,
	ELEMENT_END,
};

enum SeperatorChars {
	CHAR_CORNER_TOPLEFT,
	CHAR_CORNER_TOPRIGHT,
	CHAR_CORNER_BOTRIGHT,
	CHAR_CORNER_BOTLEFT,
	CHAR_EDGE_TOP,
	CHAR_EDGE_RIGHT,
	CHAR_EDGE_BOT,
	CHAR_EDGE_LEFT,
	CHAR_MINUS,
	CHAR_PIPE,
	CHAR_CROSS,
	CHAR_END,
};

enum SelectedSeperators {
	TOPLEFT_CORNER,
	TOPRIGHT_CORNER,
	BOTRIGHT_CORNER,
	BOTLEFT_CORNER,
	TOP_EDGE,
	RIGHT_EDGE,
	BOT_EDGE,
	LEFT_EDGE,
};

struct Blueprint {

	int begx;
	int endx;
	int begy;
	int endy;
	int xmax;
	int ymax;
	int ccx;
	int ccy;

	struct Colm *colstart;
	struct Colm *colend;
	struct Row *rowstart;
	struct Row *rowend;
	struct Cell *cellstart;

};


void draw_init_colorschemes();
void draw_topwin(WINDOW *win);
void draw_left_matrix(WINDOW *win, struct Matrix *mat,
		struct Colm *colend, struct Cell *cellstart, int begx);
void draw_right_matrix(WINDOW *win, struct Matrix *mat,
		struct Colm *colstart, struct Cell *cellstart, int begx);
void draw_above_matrix(WINDOW *win, struct Matrix *mat,
		struct Row *rowend, struct Cell *cellstart, int begy);
void draw_below_matrix(WINDOW *win, struct Matrix *mat,
		struct Row *rowstart, struct Cell *cellstart, int begy);
void draw_whole_matrix(WINDOW *win, struct Matrix *mat);
void draw_change_cell_attr(WINDOW *win, struct Matrix *mat, int attr_idx);
