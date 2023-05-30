#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "drw.h"
#include "smm.h"
#include "log.h"

// guess ill go to mars;

static int main_window_content = MAIN_MENU;

static struct Matrix *mhead = NULL;
static struct Matrix *mtail = NULL;

static void create_windows();
static void normal_mode();
static void resize_windows();

/*
 * need this one to change cursor shape *
 * fputs("\033[6 q", stdout);
 * fflush(stdout); 
 * */

static void attach_matrix(struct Matrix *mattoattach, struct Matrix *leftmat, struct Matrix *rightmat) {

	if (!mattoattach) {
		smm_log(WARN, "attach_matrix was called with a NULL mattoattach");
		return;
	}

	if (leftmat)
		leftmat->right = mattoattach;
	else {
		mhead = mattoattach;
		smm_log(DEBUG, "mhead changed to mattoattach");
	}

	if (rightmat)
		rightmat->left = mattoattach;
	else {
		mtail = mattoattach;
		smm_log(DEBUG, "mtail changed to mattoattach");
	}

	mattoattach->left = leftmat;
	mattoattach->right = rightmat;

}

static void normal_mode() {

	int event;
	struct Matrix *curmat = NULL;
	struct Colm *curcol = NULL;
	struct Row *currow = NULL;
	struct Cell *curcell = NULL;

	drw_topwin(topwin);

	mhead = mtail = curmat = make_matrix(10, 30);
	curcol = curmat->curcol;
	currow = curmat->currow;
	curcell = curmat->curcell;

	drw_whole_matrix(mainwin, curmat);
	change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_SELECTED);

	int i;

	for (i = 0;i < 13;i++)
		mvccright(curmat);

	drw_whole_matrix(mainwin, curmat);

	wgetch(mainwin);

	drw_whole_matrix(mainwin, curmat);

	wrefresh(topwin);
	wrefresh(botwin);
	wrefresh(mainwin);

	while ((event = wgetch(mainwin)) != 'q') {

		switch (event) {
			case KEY_LEFT:
			case 'h':
				// change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_NORMAL);
				mvccleft(curmat);
				// change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_SELECTED);
				break;
			case KEY_RIGHT:
			case 'l':
				// change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_NORMAL);
				mvccright(curmat);
				// change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_SELECTED);
				break;
			case 'H':
				if (curcol && curcol->right)
					dispose_col(curmat, curcol->right);
				break;
			case 'L':
				if (curmat)
					add_col(curmat, curcol, curcol->right);
				break;
			case KEY_UP:
			case 'k':
				mvccabove(curmat);
				break;
			case 'j':
			case KEY_DOWN:
				mvccbelow(curmat);
				break;
			case 'J':
				if (currow && currow->below)
					dispose_row(curmat, currow->below);
				break;
			case 'K':
				if (curmat)
					add_row(curmat, currow, currow->below);
				break;
			case 'n':
				if (curmat) {
					attach_matrix(make_matrix(3, 3), curmat, curmat->right);
					curmat = curmat->right;
				} else {
					attach_matrix(make_matrix(3, 3), curmat, NULL);
					curmat = mhead;
				}

				curcol = curmat->curcol;
				currow = curmat->currow;
				curcell = curmat->curcell;

				break;
			case 'P':
				if (curmat && curmat->left) {
					curmat = curmat->left;
					curcol = curmat->curcol;
					currow = curmat->currow;
					curcell = curmat->curcell;

				}
				break;

			case 'N':
				if (curmat && curmat->right) {
					curmat = curmat->right;
					curcol = curmat->curcol;
					currow = curmat->currow;
					curcell = curmat->curcell;

				}
				break;

			case KEY_RESIZE:
				resize_windows();
				break;

		}

		drw_topwin(topwin);
		drw_whole_matrix(mainwin, curmat);

		change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_SELECTED);

		wrefresh(topwin);
		wrefresh(botwin);
		wrefresh(mainwin);
	}
}

static void resize_windows() {

	int xmax, ymax;

	getmaxyx(stdscr, ymax, xmax);

	werase(stdscr);
	wrefresh(stdscr);

	wresize(topwin, topwin_nlines, xmax - (2 * edge_pad_vertical));

	wresize(mainwin, ymax - (topwin_nlines + botwin_nlines)
			- (2 * edge_pad_horizontal) - (2 * mainwin_pad_horizontal),
			xmax - (2 * edge_pad_vertical));

	wresize(botwin, botwin_nlines, xmax - (2 * edge_pad_vertical));

	mvwin(botwin, ymax - botwin_nlines - edge_pad_horizontal, edge_pad_vertical);

}

static void create_windows() {

	int xmax, ymax;

	getmaxyx(stdscr, ymax, xmax);

	topwin = newwin(topwin_nlines,
			xmax - (2 * edge_pad_vertical),
			edge_pad_horizontal,
			edge_pad_vertical);

	mainwin = newwin(ymax - (topwin_nlines + botwin_nlines)
			- (2 * edge_pad_horizontal) - (2 * mainwin_pad_horizontal),
			xmax - (2 * edge_pad_vertical),
			edge_pad_horizontal + topwin_nlines + mainwin_pad_horizontal,
			edge_pad_vertical);

	botwin = newwin(botwin_nlines,
			xmax - (2 * edge_pad_vertical),
			ymax - botwin_nlines - edge_pad_horizontal,
			edge_pad_vertical);

}


int main () {

	init_logger();
	
	initscr();
	clear();
	noecho();
	keypad(stdscr, TRUE);
	// if (has_colors())
	// 	init_colorschemes();

	// curs_set(0);

	create_windows();

	normal_mode();

	endwin();
	printf("\033[2 q");
}


