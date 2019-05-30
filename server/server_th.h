#include "board_library.h"

void update_color(int x, int y, int *color);
void read_firstplay_buffer(char *buffer, int socket, PlayerList *player);
int *get_colors();
void * first_play_thread(void *arg);
void print_linked_list(PlayerList *head);
void send_board();
