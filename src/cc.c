#include <stdio.h>
#include <stdbool.h>
#include "cc.h"

//bite

char BOARD[BOARD_SIZE][BOARD_SIZE];

int main(int argc, char **argv) {

	int line, column, color = -1;
	bool isAlreadyInHome;

	init_board();

	do {
		print_board();
		color = (color + 1) % 6;

		printf("Player %c turn. Move from:\n", color_num_to_char(color));
		do {
			get_position(&line, &column, color_num_to_char(color));
			isAlreadyInHome = is_in_home(line, column, color);
			search_all_reachable_positions(line, column);
		} while (!found_reachable_positions(color, isAlreadyInHome));

		if (HELP_ON) {
			print_board();
		}

		BOARD[line][column] = CHAR_EMPTY;
		printf("Move to:\n");
		get_position(&line, &column, CHAR_REACHABLE);
		BOARD[line][column] = color_num_to_char(color);

		hide_reachable_positions();
	} while (!player_won(line, column, color, isAlreadyInHome));

	print_board();
	printf("Player %c won!\n", color_num_to_char(color));
	return 0;
}

void init_board() {

	int line, column;

	for (line = 0; line < BOARD_SIZE; line++) {
		for (column = 0; column < BOARD_SIZE; column++) {
			BOARD[line][column] = CHAR_BLOCK;
		}
	}

	for (line = 0; line <= 3 * BASE_SIZE; line++) {
		for (column = 3 * BASE_SIZE - line; column <= 3 * BASE_SIZE; column++) {
			if (line < BASE_SIZE) {
				BOARD[line][column] = color_num_to_char(0);
			} else if (column < 2 * BASE_SIZE) {
				BOARD[line][column] = color_num_to_char(4);
			} else {
				BOARD[line][column] = color_num_to_char(2);
			}
		}
	}

	for (line = BASE_SIZE; line < BOARD_SIZE; line++) {
		for (column = BASE_SIZE; column < BOARD_SIZE + BASE_SIZE - line; column++) {
			if (BOARD[line][column] != CHAR_BLOCK) {
				BOARD[line][column] = CHAR_EMPTY;
			} else if (line > 3 * BASE_SIZE) {
				BOARD[line][column] = color_num_to_char(3);
			} else if (column < 2 * BASE_SIZE) {
				BOARD[line][column] = color_num_to_char(5);
			} else {
				BOARD[line][column] = color_num_to_char(1);
			}
		}
	}
}

void print_board() {

	int line, column;

	printf("\t");

	for (column = 0; column < BOARD_SIZE; column++) {
		if (column < 26) {
			printf("%c ", 'A' + column);
		} else {
			printf("%c ", 'a' + column - 26);
		}
	}

	printf("\n\n");

	for (line = 0; line < BOARD_SIZE; line++) {
		printf("%d\t", line + 1);
		for (column = 0; column < BOARD_SIZE; column++) {
			printf("%c ", BOARD[line][column]);
		}
		printf("\n");
	}

	printf("\n\n");
}

char color_num_to_char(int colorNum) {

	const char colorChars[6] = { CHAR_COLOR1, CHAR_COLOR2, CHAR_COLOR3, CHAR_COLOR4, CHAR_COLOR5, CHAR_COLOR6 };
	return colorChars[colorNum];
}

void search_straight_reachable_positions(int line, int column, int diffLine, int diffCol) {

	int newLine = line + diffLine, newColumn = column + diffCol;

	if (!position_exists(newLine, newColumn)) {
		return;
	}

	if (BOARD[newLine][newColumn] == CHAR_EMPTY) {
		if (BOARD[line][column] != CHAR_REACHABLE) {
			BOARD[newLine][newColumn] = CHAR_REACHABLE;
		}
		return;
	}

	newLine += diffLine;
	newColumn += diffCol;

	if (position_exists(newLine, newColumn) && BOARD[newLine][newColumn] == CHAR_EMPTY) {
		BOARD[newLine][newColumn] = CHAR_REACHABLE;
		search_all_reachable_positions(newLine, newColumn);
	}
}

void search_all_reachable_positions(int line, int column) {

	search_straight_reachable_positions(line, column, -1, 1);
	search_straight_reachable_positions(line, column, 0, 1);
	search_straight_reachable_positions(line, column, 1, 0);
	search_straight_reachable_positions(line, column, 1, -1);
	search_straight_reachable_positions(line, column, 0, -1);
	search_straight_reachable_positions(line, column, -1, 0);
}

bool found_reachable_positions(int color, bool isAlreadyInHome) {

	int line, column;
	bool positionFound = false;

	for (line = 0; line < BOARD_SIZE; line++) {
		for (column = 0; column < BOARD_SIZE; column++) {
			if (BOARD[line][column] == CHAR_REACHABLE) {
				if (isAlreadyInHome && !is_in_home(line, column, color)) {
					BOARD[line][column] = CHAR_EMPTY;
				} else {
					positionFound = true;
				}
			}
		}
	}

	return positionFound;
}

void hide_reachable_positions() {

	int line, column;

	for (line = 0; line < BOARD_SIZE; line++) {
		for (column = 0; column < BOARD_SIZE; column++) {
			if (BOARD[line][column] == CHAR_REACHABLE) {
				BOARD[line][column] = CHAR_EMPTY;
			}
		}
	}
}

bool is_in_home(int line, int column, int color) {

	switch (color) {
	case 0:
		if (line > 3 * BASE_SIZE) {
			return true;
		}
		break;
	case 1:
		if (column < BASE_SIZE) {
			return true;
		}
		break;
	case 2:
		if (column + line < 3 * BASE_SIZE) {
			return true;
		}
		break;
	case 3:
		if (line  < BASE_SIZE) {
			return true;
		}
		break;
	case 4:
		if (column > 3 * BASE_SIZE) {
			return true;
		}
		break;
	case 5:
		if (column + line > 5 * BASE_SIZE) {
			return true;
		}
		break;
	}

	return false;
}

bool player_won(int line, int column, int color, bool wasInHome) {

	static int scores[6] = { 0, 0, 0, 0, 0, 0 };
	const int maxScore = (BASE_SIZE * (BASE_SIZE + 1)) / 2;

	if (!wasInHome && is_in_home(line, column, color)) {
		scores[color]++;
		if (scores[color] == maxScore) {
			return true;
		}
	}

	return false;
}

bool position_exists(int line, int column) {

	if (line < 0 || line >= BOARD_SIZE) {
		return false;
	}
	if (column < 0 || column >= BOARD_SIZE  ) {
		return false;
	}
	if (BOARD[line][column] == CHAR_BLOCK) {
		return false;
	}
	return true;
}

void get_position(int *ptr_line, int *ptr_col, char piece) {

	int line, column;
	char colChar;

	do {
		printf("\tLine -> ");
		scanf(" %d", &line);
		printf("\tColumn -> ");
		scanf(" %c", &colChar);

		line--;

		if (colChar < 'a') {
			column = colChar - 'A';
		} else {
			column = colChar - 'a' + 26;
		}



	} while (!position_exists(line, column) || BOARD[line][column] != piece);

	*ptr_line = line;
	*ptr_col = column;
}






