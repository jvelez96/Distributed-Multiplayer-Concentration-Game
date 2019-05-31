#include "board_library.h"

struct args {
    char buff[BUFFERSIZE];
    int socket;
};

void update_color(int x, int y, int *color);
void * broadcast_play(void *args);
void manage_player(char *buffer, int socket, int *done, PlayerList *player);
void * read_secondplay_buffer(void *socket);
int *get_colors();
void * first_play_thread(void *arg);
void print_linked_list(PlayerList *head);
void send_board();
