#include "server_th.h"

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
extern PlayerList *client_list;

int *get_colors(){
  static int colors[3];

  for(int i=0;i<3;i++){
    colors[i] = rand() % 255;
  }
  return colors;
}

void print_linked_list(PlayerList *head){
  PlayerList *curr;
  int i=1;
  curr = head;
  if(curr == NULL){
    printf("Lista vazia\n");
  }

  while(curr != NULL) {
    printf("socket: %d for player %d with a game with %d players\n", curr->socket,i, curr->player_number);
    curr = curr->next;
    i++;
  }
}

void * socketThread(void *socket)
{
  //int newSocket = *((int *)arg);
  int done = 0;
  int i;
  int newSocket = *((int*)socket);

  printf("%d and nummer players: %d\n", client_list->socket, client_list->player_number);
  //recv(newSocket , client_message , 2000 , 0);
  // Send message to the client socket
  pthread_mutex_lock(&lock);
  sprintf(buffer, "Thread with socket %d\n", newSocket);
  printf("buffer: %s\n", buffer);
  pthread_mutex_unlock(&lock);
  sleep(1);

  printf("Exit socketThread \n");

  send(newSocket,buffer,40,0);
  print_linked_list(client_list);

  close(newSocket);
  pthread_exit(NULL);
}
