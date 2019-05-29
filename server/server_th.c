#include "server_th.h"

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
extern PlayerList *client_list;
extern int size;
extern board_place * board;
extern int play1[2];
extern int n_corrects;

void read_buffer(char *buffer, int *color, struct play_response *play, char *status, int *buffer_type)
{
  int i;
  const char delimiter[2] = ";";

  *buffer_type = ASCII - buffer[0];

  char *token = strtok(buffer,delimiter);

  switch(*buffer_type)
  {
    case 0:
      i = 0;

      while(token != NULL)
      {
        if(i==1){*color = atoi(token);}
        if(i==2){play->code = atoi(token);}
        if(i==3){play->play1[0] = atoi(token);}
        if(i==4){play->play1[1] = atoi(token);}
        if(i==5){play->play2[0] = atoi(token);}
        if(i==6){play->play2[1] = atoi(token);}
        if(i==7){strcpy(play->str_play1,token);}
        if(i==8){strcpy(play->str_play2,token);}
        if(i==9){strcpy(status,token);}
        token = strtok(NULL, delimiter);
        i++;
      }
    break;
  }

}

void write_buffer(char *buffer, int *color, struct play_response *play, char *status, int *buffer_type)
{
  sprintf(buffer,"%d;%d;%d;%d;%d;%d;%d;%s;%s;%s", *buffer_type, *color, play->code, play->play1[0], play->play1[1], play->play2[0], play->play2[1], play->str_play1, play->str_play2, status);
}

void send_board(){
  int i;
  int x, y;
  char buffer[BUFFERSIZE];
  PlayerList *curr = client_list;

  for(x=0;x<size;x++){
    for(y=0;y<size;y++){
      memset(buffer, 0, BUFFERSIZE);
      i = linear_conv(x,y);
      sprintf(buffer, "%d %d %s %d %d %d", x, y, board[i].v, board[i].color[0], board[i].color[1], board[i].color[2]);
      while(curr != NULL){
        //only sends board if the player has not received it yet
        if(curr->status == 0){
          send(curr->socket, buffer, BUFFERSIZE, 0);
          printf("sending buffer %s for cell x: %d y: %d of board i: %d\n",buffer, x,y,i);
        }
        curr = curr->next;
      }
    }
  }
  curr = client_list;
  while(curr != NULL){
    curr->status = 1;
    curr = curr->next;
  }
}

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
