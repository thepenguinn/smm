#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "draw.h"
#include "smm.h"
#include "log.h"

// static int main_window_content = MAIN_MENU;

static struct Matrix *Mhead = NULL;
static struct Matrix *Mtail = NULL;

static void create_windows(void);
static void normal_mode(void);
static void resize_windows(void);
static void attach_matrix(struct Matrix *mattoattach,
		struct Matrix *leftmat, struct Matrix *rightmat);
static void detach_matrix(struct Matrix *mattodetach);


/*
 * need this one to change cursor shape *
 * fputs("\033[6 q", stdout);
 * fflush(stdout);
 * */

static void attach_matrix(struct Matrix *mattoattach,
		struct Matrix *leftmat, struct Matrix *rightmat) {

	if (!mattoattach) {
		smm_log(WARN, "attach_matrix was called with a NULL mattoattach");
		return;
	}

	if (leftmat)
		leftmat->right = mattoattach;
	else
		Mhead = mattoattach;

	if (rightmat)
		rightmat->left = mattoattach;
	else
		Mtail = mattoattach;

	mattoattach->left = leftmat;
	mattoattach->right = rightmat;

}

static void detach_matrix(struct Matrix *mattodetach) {

	if (!mat) {
		smm_log(WARN, "detach_matrix was called with a NULL mattodetach");
		return;
	}

	if (mat->left)
		mat->left->right = mat->right;
	else
		Mhead = mat->right;

	if (mat->right)
		mat->right->left = mat->left;
	else
		Mtail = mat->left;

	/*
	 * This is for matrix_dispose
	 * */

	mat->left = mat->right = NULL;

}


static void normal_mode(void) {

	int event;
	struct Matrix *curmat = NULL;
	struct Colm *curcol = NULL;
	struct Row *currow = NULL;
	//   struct Cell *curcell = NULL;

	draw_topwin(topwin);

	Mhead = Mtail = curmat = matrix_create(3, 3, 1);
	curcol = curmat->curcol;
	currow = curmat->currow;
	//   curcell = curmat->curcell;

	if (curmat) {
		attach_matrix(matrix_create(3, 3, 1), curmat, curmat->right);
		curmat = curmat->right;
	} else {
		attach_matrix(matrix_create(3, 3, 1), curmat, NULL);
		curmat = Mhead;
	}

	curmat = curmat->left;
	attach_matrix(matrix_multiply(curmat, curmat->right), curmat, curmat->right);

	curmat = curmat->right;
	curcol = curmat->curcol;
	currow = curmat->currow;
	//   curcell = curmat->curcell;

	draw_whole_matrix(mainwin, curmat);
	draw_change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_SELECTED);

	wrefresh(topwin);
	wrefresh(botwin);
	wrefresh(mainwin);

	while ((event = wgetch(mainwin)) != 'q') {

		switch (event) {
			case KEY_LEFT:
			case 'h':
				matrix_mvcc_left(curmat);
				break;
			case KEY_RIGHT:
			case 'l':
				matrix_mvcc_right(curmat);
				break;
			case 'H':
				if (curcol && curcol->right)
					matrix_dispose_col(curmat, curcol->right);
				break;
			case 'L':
				if (curmat)
					matrix_add_col(curmat, curcol, curcol->right, 0, 0);
				break;
			case KEY_UP:
			case 'k':
				matrix_mvcc_above(curmat);
				break;
			case 'j':
			case KEY_DOWN:
				matrix_mvcc_below(curmat);
				break;
			case 'J':
				if (currow && currow->below)
					matrix_dispose_row(curmat, currow->below);
				break;
			case 'K':
				if (curmat)
					matrix_add_row(curmat, currow, currow->below, 0, 0);
				break;
			case 'n':

				if (curmat) {
					attach_matrix(matrix_create(3, 3, 256), curmat, curmat->right);
					curmat = curmat->right;
				} else {
					attach_matrix(matrix_create(3, 3, 256), curmat, NULL);
					curmat = Mhead;
				}

				curcol = curmat->curcol;
				currow = curmat->currow;
				//   curcell = curmat->curcell;

				break;
			case 'P':
				if (curmat && curmat->left) {
					curmat = curmat->left;
					curcol = curmat->curcol;
					currow = curmat->currow;
					//   curcell = curmat->curcell;

				}
				break;

			case 'N':
				if (curmat && curmat->right) {
					curmat = curmat->right;
					curcol = curmat->curcol;
					currow = curmat->currow;
					//   curcell = curmat->curcell;

				}
				break;

			case KEY_RESIZE:
				resize_windows();
				break;

		}

		draw_topwin(topwin);
		draw_whole_matrix(mainwin, curmat);

		draw_change_cell_attr(mainwin, curmat, ELEMENT_MATRIX_SELECTED);

		wrefresh(topwin);
		wrefresh(botwin);
		wrefresh(mainwin);
	}
}

static void resize_windows(void) {

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

static void create_windows(void) {

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


int main (void) {

	init_logger();

	initscr();
	clear();
	noecho();
	keypad(stdscr, TRUE);

#ifndef __DEBUG__

	if (has_colors())
		draw_init_colorschemes();

	curs_set(0);

#endif

	create_windows();

	normal_mode();

	endwin();

}
