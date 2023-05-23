#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include<stdint.h>

#include "matrix.h"
#include "drw.h"

// guess ill go to mars;


int main () {

	matrix *mat;
	int x, y;
	int change = 0;
	WINDOW *topwin, *botwin, *mainwin;
	/*debugging*/
	int shit;
	colm *curcol;
	char ch;
	
	initscr();
	clear();
	noecho();
	keypad(stdscr, TRUE);
	if (has_colors())
		init_colorschemes();

	curs_set(0);

	getmaxyx(stdscr, y, x);
	topwin = newwin(3, x, 0, 0);
	mainwin = newwin(y - 6, x, 3, 0);
	botwin = newwin(3, x, y - 3, 0);

	mat = makematrix(6, 6);
	//  init_color(17, 1000, 112, 1000);
	//  init_color(18, 120, 112, 190);
	//  init_pair(20, 18, 17);
	// init_color(20, 238, 238, 234);


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

		}

		colm *debugcol;
		int anum = 1;

		fprintf(stderr, "----\n");

		for (debugcol = mat->colstart;debugcol;debugcol = debugcol->right) {
			fprintf(stderr, "%d -> %p\n", anum, debugcol);
			anum++;
		}

		fprintf(stderr, "----\n");

		getmaxyx(stdscr, y, x);

		wresize(mainwin, y - 6, x);

		wresize(topwin, 3, x);

		wresize(botwin, 3, x);
		mvwin(botwin, y - 3, 0);
		drwtopwin(topwin);

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


