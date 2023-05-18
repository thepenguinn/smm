
#define WITH_SEP_ABOVE 1
#define NO_SEP_ABOVE 0

void change_cell_attr(WINDOW *win, matrix *mat, attr_t attr);
void drwmatrix(WINDOW *win, matrix *mat);
void drwcell(WINDOW *win, matrix *mat);
void drwrow(WINDOW *win, matrix *mat, colm *coltostart, cell *celltobegin, int seperator);
void drwcol(WINDOW *win, matrix *mat);
void drwtopwin(WINDOW *win);

