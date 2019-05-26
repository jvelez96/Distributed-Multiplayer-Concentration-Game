#include "server_th.h"

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void print_linked_list(PlayerList *head){
  PlayerList *curr;
  curr = head;
  if(curr == NULL){
    printf("Lista vazia\n");
  }

  while(curr != NULL) {
    printf("%d\n", curr->this);
    curr = curr->next;
  }
}

void * socketThread(void *arg)
{
  //int newSocket = *((int *)arg);
  int done = 0;
  int i;
  int newSocket = *((int*)arg);
  //int newSocket = ((client_data*)arg)->curr_socket;
  //int thread_nr = ((client_data*)arg)->curr_thread;
  //LinkedList *client_list = ((client_data*)arg)->clientlist;
  printf("%d\n", newSocket);
  //recv(newSocket , client_message , 2000 , 0);
  // Send message to the client socket
  pthread_mutex_lock(&lock);
  sprintf(buffer, "Thread with socket %d\n", newSocket);
  printf("buffer: %s\n", buffer);
  pthread_mutex_unlock(&lock);
  sleep(1);

  printf("Exit socketThread \n");

  send(socket,buffer,40,0);

  close(newSocket);
  pthread_exit(NULL);
}
