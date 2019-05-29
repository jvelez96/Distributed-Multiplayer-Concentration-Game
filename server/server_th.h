#include "board_library.h"

int *get_colors();
void * socketThread(void *arg);
void print_linked_list(PlayerList *head);
void send_board(int socket, int size);
