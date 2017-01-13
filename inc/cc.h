#ifndef CC_H
#define CC_H

// EDIT BELOW
#define BASE_SIZE 4
#define HELP_ON true
#define CHAR_BLOCK ' '
#define CHAR_EMPTY '.'
#define CHAR_REACHABLE '+'
#define CHAR_COLOR1 '1'
#define CHAR_COLOR2 '2'
#define CHAR_COLOR3 '3'
#define CHAR_COLOR4 '4'
#define CHAR_COLOR5 '5'
#define CHAR_COLOR6 '6'
// EDIT ABOVE

#define BOARD_SIZE (4*BASE_SIZE+1)

void init_board();
void print_board();
char color_num_to_char(int colorNum);
void search_straight_reachable_positions(int line, int column, int diffLine, int diffCol);
void search_all_reachable_positions(int line, int column);
bool found_reachable_positions(int color, bool isAlreadyInHome);
void hide_reachable_positions();
bool is_in_home(int line, int column, int color);
bool player_won(int line, int column, int color, bool wasInHome);
bool position_exists(int line, int column);
void get_position(int *ptr_line, int *ptr_col, char piece);

#endif