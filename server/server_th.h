#include "board_library.h"

void update_color(int x, int y, int *color);
void * broadcast_play(void *buffer);
void manage_player(char *buffer, int socket, int *done, PlayerList *player);
void * read_secondplay_buffer(void *socket);
int *get_colors();
void * first_play_thread(void *arg);
void print_linked_list(PlayerList *head);
void send_board();
