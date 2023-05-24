#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "drw.h"
#include "smm.h"

// guess ill go to mars;

static int main_window_content = MAIN_MENU;


void normal_mode() {

}

void resize_windows() {

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



int main () {

	matrix *mat;
	int x, y;
	int change = 0;
	/*debugging*/
	int shit;
	colm *curcol;
	int ch;
	
	initscr();
	clear();
	noecho();
	keypad(stdscr, TRUE);
	if (has_colors())
		init_colorschemes();

	curs_set(0);

	getmaxyx(stdscr, y, x);

	topwin = newwin(topwin_nlines,
			x - (2 * edge_pad_vertical),
			edge_pad_horizontal,
			edge_pad_vertical);

	mainwin = newwin(y - (topwin_nlines + botwin_nlines)
			- (2 * edge_pad_horizontal) - (2 * mainwin_pad_horizontal),
			x - (2 * edge_pad_vertical),
			edge_pad_horizontal + topwin_nlines + mainwin_pad_horizontal,
			edge_pad_vertical);

	botwin = newwin(botwin_nlines,
			x - (2 * edge_pad_vertical),
			y - botwin_nlines - edge_pad_horizontal,
			edge_pad_vertical);

	mat = makematrix(6, 6);
	//  init_color(17, 1000, 112, 1000);
	//  init_color(18, 120, 112, 190);
	//  init_pair(20, 18, 17);
	// init_color(20, 238, 238, 234);

	if (shit > 500)
		shit = 0;
	shit++;
	mvwprintw(botwin, 0, 0, "hai this is botwin %d", shit);
	shit++;
	mvwprintw(botwin, 1, 0, "hai this is botwin %d", shit);
	shit++;
	mvwprintw(botwin, 2, 0, "hai this is botwin %d", shit);

	drw_topwin(topwin);

	wrefresh(topwin);
	wrefresh(botwin);

	row *currow;
	for(curcol = mat->colstart;curcol->right;curcol = curcol->right);
	for(currow = mat->rowstart;currow->below;currow = currow->below);

	drwmatrix(mainwin, mat);
	wrefresh(mainwin);
	while ((ch = wgetch(mainwin)) != 'q') {

		switch (ch) {
			case 'h':
				if (curcol->right)
					disposecol(mat, curcol->right);
				// change_cell_attr(mainwin, mat, A_NORMAL);
				// fputs("\033[6 q", stdout);
				// fflush(stdout);
				break;
			case 'l':
				addcol(mat, curcol, curcol->right);
				// change_cell_attr(mainwin, mat, A_BOLD);
				// fputs("\033[2 q", stdout);
				// fflush(stdout);
				// wprintw(mainwin, "\033[2 q");
				// printf("\033[2 q");
				break;
			case 'j':
				if (currow->below)
					disposerow(mat, currow->below);
				break;
			case 'k':
				addrow(mat, currow, currow->below);
				break;
			case KEY_RESIZE:
				resize_windows();
				break;

		}

		colm *debugcol;
		int anum = 1;

		fprintf(stderr, "----\n");

		for (debugcol = mat->colstart;debugcol;debugcol = debugcol->right) {
			fprintf(stderr, "%d -> %p\n", anum, debugcol);
			anum++;
		}

		fprintf(stderr, "----\n");

		drw_topwin(topwin);

		if (shit > 500)
			shit = 0;
		shit++;
		mvwprintw(botwin, 0, 0, "hai this is botwin %d", shit);
		shit++;
		mvwprintw(botwin, 1, 0, "hai this is botwin %d", shit);
		shit++;
		mvwprintw(botwin, 2, 0, "hai this is botwin %d", shit);

		// wattron(mainwin, COLOR_PAIR(3));
		drwmatrix(mainwin, mat);
		// attron(COLOR_PAIR(20));
		// printw("x --> %d, and y --> %d\n", x, y);

		wrefresh(topwin);
		wrefresh(botwin);
		wrefresh(mainwin);
	}
	endwin();
	printf("\033[2 q");
}


