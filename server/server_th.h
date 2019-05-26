#include "LinkedList.h"

typedef struct client_data {
  LinkedList *clientlist;
  int curr_socket;
  int curr_thread;
  int nthreads;
}client_data;

void * socketThread(void *arg);
void print_linked_list(LinkedList *head);
