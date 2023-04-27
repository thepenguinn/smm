#include<stdio.h>
//#include<ncurses.h>

#define MLEN 5

/*
char char_top_l[]  = "┏" ;
char char_top_r[]  = "┓" ;
char char_bot_l[]  = "┗" ;
char char_bot_r[]  = "┛" ;
char char_minus[]  = "━" ;
char char_pipe[]   = "┃" ;
char char_space    = ' ' ;
*/

char char_top_l[]  = "┌" ;
char char_top_r[]  = "┐" ;
char char_bot_l[]  = "└" ;
char char_bot_r[]  = "┘" ;
char char_minus[]  = "─" ;
char char_pipe[]   = "│" ;
char char_space    = ' ' ;

int max_len = 1;
int max_num = 10;

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
*/


void get_val (int mat[MLEN][MLEN], int mnum);
void print_mat (int mat[MLEN][MLEN]);
void multmat (int mat1[MLEN][MLEN], int mat2[MLEN][MLEN], int mat3[MLEN][MLEN]);
int get_len (int num);
void get_width (int mat[MLEN][MLEN]);

void get_width (int mat[MLEN][MLEN]) {

	int i, j;
	max_len = 1;
	max_num = 10;

	for (i=0; i < MLEN;i++) {
		for (j=0; j < MLEN;j++) {
			if (mat[i][j] > max_num)
				max_len = get_len(mat[i][j]);
		}
	}

}

int get_len (int num) {

	if (num >= 1000000000) {
		max_num = 1000000000;
		return 10;
	}
	if (num >= 100000000) {
		max_num = 100000000;
		return 9;
	}
	if (num >= 10000000) {
		max_num = 10000000;
		return 8;
	}
	if (num >= 1000000) {
		max_num = 1000000;
		return 7;
	}
	if (num >= 100000) {
		max_num = 100000;
		return 6;
	}
	if (num >= 10000) {
		max_num = 10000;
		return 5;
	}
	if (num >= 1000) {
		max_num = 1000;
		return 4;
	}
	if (num >= 100) {
		max_num = 100;
		return 3;
	}
	if (num >= 10) {
		max_num = 10;
		return 2;
	}

	return 1;

}

void get_val (int mat[MLEN][MLEN], int mnum) {

	int i, j;

	printf("Enter the matrix %d\n", mnum);
	for (i=0; i < MLEN;i++) {
		for (j=0; j < MLEN;j++) {
			printf("Enter the value for (%d,%d): ", i, j);
			scanf("%d", &mat[i][j]);
		}
	}
	printf("\n");
}

void print_mat (int mat[MLEN][MLEN]) {

	int i, j;

	get_width (mat);
	max_len++;

	printf("%s%s", char_top_l, char_minus);
	printf("%*c", max_len * MLEN - 1, char_space);
	printf("%s%s\n", char_minus, char_top_r);

	for (i=0; i < MLEN;i++) {
		printf("%s", char_pipe);
		for (j=0; j < MLEN;j++) {
			printf("% *d", max_len, mat[i][j]);
		}
		printf(" %s\n", char_pipe);
	}

	printf("%s%s", char_bot_l, char_minus);
	printf("%*c", max_len * MLEN - 1, char_space);
	printf("%s%s\n", char_minus, char_bot_r);

}

void multmat (int mat1[MLEN][MLEN], int mat2[MLEN][MLEN], int mat3[MLEN][MLEN]) {

	int i, j, k;

	for (i=0; i < MLEN; i++){
		for (j=0; j < MLEN; j++){
			mat3[i][j] = 0;
		}
	}

	// TODO : figure out this shit
	for (i=0; i < MLEN; i++){
		for (j=0; j < MLEN; j++){
			for (k=0; k < MLEN; k++){
				mat3[i][j] = mat3[i][j] + (mat1[i][k] * mat2[k][j]);
			}
		}
	}

}

int main () {

	int mat1[MLEN][MLEN];
	int mat2[MLEN][MLEN];
	int mat3[MLEN][MLEN];

	get_val(mat1, 1);
	get_val(mat2, 2);

	multmat(mat1, mat2, mat3);

	print_mat(mat1);
	print_mat(mat2);
	print_mat(mat3);

}
