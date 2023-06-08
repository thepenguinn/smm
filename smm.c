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
static void stretch_window(WINDOW *win);
static void squeeze_window(WINDOW *win);
static struct Matrix *matrix_mode(struct Menu *menu);

static void stretch_window(WINDOW *win) {

	int ymax, xmax;
	int begy, begx;

	getmaxyx(win, ymax, xmax);
	getbegyx(win, begy, begx);

	mvwin(win, begy, begx - 2);
	wresize(win, ymax, xmax + 4);

}

static void squeeze_window(WINDOW *win) {

	int ymax, xmax;
	int begy, begx;

	getmaxyx(win, ymax, xmax);
	getbegyx(win, begy, begx);

	// werase(stdscr);
	// wrefresh(stdscr);
	wresize(win, ymax, xmax - 4);
	mvwin(win, begy, begx + 2);

}

static WINDOW *Topwin, *Mainwin, *Botwin;

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

	if (!mattodetach) {
		smm_log(WARN, "detach_matrix was called with a NULL mattodetach");
		return;
	}

	if (mattodetach->left)
		mattodetach->left->right = mattodetach->right;
	else
		Mhead = mattodetach->right;

	if (mattodetach->right)
		mattodetach->right->left = mattodetach->left;
	else
		Mtail = mattodetach->left;

	/*
	 * This is for matrix_dispose
	 * */

	mattodetach->left = mattodetach->right = NULL;

}

static void main_mvitem_left(struct Menu *menu) {

	int itemstoleft;

	if (!menu->curmat) {
		return;
	}

	itemstoleft = (menu->ymax + 1) / 3;

	if (menu->curitemidx - itemstoleft >= 0) {

		menu->curitemidx -= itemstoleft;

		while (itemstoleft) {

			menu->curmat = menu->curmat->left;
			itemstoleft--;

		}

	}

}

static void main_mvitem_right(struct Menu *menu) {

	int maxitems;
	int itemsbelow;
	int itemstoright;

	if (!menu->curmat) {
		return;
	}

	maxitems = (menu->ymax + 1) / 3;
	itemsbelow = maxitems - (menu->curitemidx % maxitems) - 1;

	if (menu->curitemidx + itemsbelow + 1 >= menu->totalitems)
		return;
	else if (menu->curitemidx + maxitems < menu->totalitems)
		itemstoright = maxitems;
	else
		itemstoright = menu->totalitems - menu->curitemidx - 1;

	menu->curitemidx += itemstoright;

	while (itemstoright) {

		menu->curmat = menu->curmat->right;
		itemstoright--;

	}

}

// static void main_mvitem_above(struct Menu *menu) {
//
// 	if (menu->curmat->left) {
//
//
//
//
// 	}
//
//
// }


static struct Matrix *matrix_mode(struct Menu *menu) {

	int event;
	struct Matrix *curmat = menu->curmat;

	draw_topwin(Topwin);
	draw_whole_matrix(Mainwin, curmat);
	draw_change_cell_attr(Mainwin, curmat, ELEMENT_MATRIX_SELECTED);

	wrefresh(Topwin);
	wrefresh(Botwin);
	wrefresh(Mainwin);

	while ((event = wgetch(Mainwin)) != 'q') {

		smm_log(WARN, "matrix_mode %d", event);
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
				if (curmat->curcol && curmat->curcol->right)
					matrix_dispose_col(curmat, curmat->curcol->right);
				break;
			case 'L':
				if (curmat)
					matrix_add_col(curmat, curmat->curcol,
							curmat->curcol->right, 0, 0);
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
				if (curmat->currow && curmat->currow->below)
					matrix_dispose_row(curmat, curmat->currow->below);
				break;
			case 'K':
				if (curmat)
					matrix_add_row(curmat, curmat->currow,
							curmat->currow->below, 0, 0);
				break;
			case 'p':
				if (curmat && curmat->left) {
					menu->curmat = curmat = curmat->left;
					menu->curitemidx -= 1;
				}
				break;

			case 'n':
				if (curmat && curmat->right) {
					menu->curmat = curmat = curmat->right;
					menu->curitemidx += 1;
				}
				break;

			case KEY_ESCAPE:
				return curmat;
				break;
			case KEY_RESIZE:
				resize_windows();
				draw_topwin(Topwin);
				wrefresh(Topwin);
				break;

		}

		draw_whole_matrix(Mainwin, curmat);
		draw_change_cell_attr(Mainwin, curmat, ELEMENT_MATRIX_SELECTED);

		wrefresh(Botwin);
		wrefresh(Mainwin);

	}

	return curmat;

}

static void normal_mode(void) {

	int event;
	struct Menu mainmenu;

	mainmenu.ymax = 0;
	mainmenu.cury = 0;
	mainmenu.totalitems = 0;
	mainmenu.curitemidx = 0;
	mainmenu.curmat = NULL;

	draw_topwin(Topwin);

	draw_main_menu(Mainwin, &mainmenu);

	draw_page_hints(Botwin, &mainmenu);

	wrefresh(Topwin);
	wrefresh(Botwin);
	wrefresh(Mainwin);

	while ((event = wgetch(Mainwin)) != 'q') {

		smm_log(WARN, "normal_mode %d", event);
		switch (event) {

			case KEY_RIGHT:
			case 'l':
				if (mainmenu.curmat)
					main_mvitem_right(&mainmenu);
				break;
			case KEY_LEFT:
			case 'h':
				if (mainmenu.curmat)
					main_mvitem_left(&mainmenu);
				break;
			case KEY_UP:
			case 'k':
				if (mainmenu.curmat && mainmenu.curmat->left) {
					mainmenu.curmat = mainmenu.curmat->left;
					mainmenu.curitemidx -= 1;
				}
				break;
			case KEY_DOWN:
			case 'j':
				if (mainmenu.curmat && mainmenu.curmat->right) {
					mainmenu.curmat = mainmenu.curmat->right;
					mainmenu.curitemidx += 1;
				}
				break;
			case 'N':

				if (mainmenu.curmat) {
					attach_matrix(matrix_create(3, 3, 256),
							mainmenu.curmat, mainmenu.curmat->right);
					mainmenu.curmat = mainmenu.curmat->right;
					mainmenu.curitemidx += 1;
				} else {
					attach_matrix(matrix_create(3, 3, 256), NULL, NULL);
					mainmenu.curmat = Mhead;
				}
				mainmenu.totalitems += 1;

				break;
			case KEY_RETURN:
				if (mainmenu.curmat) {
					/* TODO: clean this */
					wmove(Mainwin, mainmenu.cury, 0);
					wprintw(Mainwin, " ");
					wmove(Mainwin, mainmenu.cury + 1, 0);
					wprintw(Mainwin, " ");
					wrefresh(Mainwin);
					squeeze_window(Mainwin);

					matrix_mode(&mainmenu);
					stretch_window(Mainwin);
				}
				break;
			case KEY_RESIZE:
				resize_windows();
				draw_topwin(Topwin);
				stretch_window(Mainwin);
				wrefresh(Topwin);
				break;


		}



		draw_main_menu(Mainwin, &mainmenu);

		draw_page_hints(Botwin, &mainmenu);

		wrefresh(Botwin);
		wrefresh(Mainwin);

	}

}

static void resize_windows(void) {

	int xmax, ymax;

	getmaxyx(stdscr, ymax, xmax);

	werase(stdscr);
	wrefresh(stdscr);

	wresize(Topwin, topwin_nlines, xmax - (2 * edge_pad_vertical));

	wresize(Mainwin, ymax - (topwin_nlines + botwin_nlines)
			- (2 * edge_pad_horizontal) - (2 * mainwin_pad_horizontal),
			xmax - (2 * edge_pad_vertical));

	wresize(Botwin, botwin_nlines, xmax - (2 * edge_pad_vertical));

	mvwin(Botwin, ymax - botwin_nlines - edge_pad_horizontal, edge_pad_vertical);

}

static void create_windows(void) {

	int xmax, ymax;

	getmaxyx(stdscr, ymax, xmax);

	Topwin = newwin(topwin_nlines,
			xmax - (2 * edge_pad_vertical),
			edge_pad_horizontal,
			edge_pad_vertical);

	Mainwin = newwin(ymax - (topwin_nlines + botwin_nlines)
			- (2 * edge_pad_horizontal) - (2 * mainwin_pad_horizontal),
			xmax - (2 * edge_pad_vertical),
			edge_pad_horizontal + topwin_nlines + mainwin_pad_horizontal,
			edge_pad_vertical);

	Botwin = newwin(botwin_nlines,
			xmax - (2 * edge_pad_vertical),
			ymax - botwin_nlines - edge_pad_horizontal,
			edge_pad_vertical);

}


int main (void) {

	init_logger();

	initscr();
	clear();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

#ifndef __DEBUG__

	if (has_colors())
		draw_init_colorschemes();

	curs_set(0);

#endif

	create_windows();

	/*
	 * TODO: choose between arrowkeys or lightning speed escape
	 * capture, dumbass !!
	 * */
	keypad(Mainwin, TRUE);
	ESCDELAY = 10;
	stretch_window(Mainwin);
	normal_mode();

	endwin();

}
