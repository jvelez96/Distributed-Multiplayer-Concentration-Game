#include "server_th.h"

extern pthread_mutex_t lock;
extern PlayerList *client_list;
extern int size;
extern board_place * board;
extern int n_corrects;
extern int play1[MAXPLAYERS][2];
extern play_response resp[MAXPLAYERS];

void update_color(int x, int y, int *color){
  int i;
  i = linear_conv(x,y);
  board[i].color[0] = color[0];
  board[i].color[1] = color[1];
  board[i].color[2] = color[2];

  return;
}

void * broadcast_play(void *buffer){
  PlayerList *curr;

  curr = client_list;

  while(curr != NULL){
    send(curr->socket, (char*) buffer, BUFFERSIZE,0);
    curr = curr->next;
  }
  pthread_exit(NULL);
}


void * read_secondplay_buffer(void *socket){
  int newSocket = *((int*)socket);
  int x,y;
  fd_set rfds;
  struct timeval tv;
  int retval;
  char buffer[BUFFERSIZE];

  /* Watch stdin (socket) to see when it has input. */

  FD_ZERO(&rfds);
  FD_SET(newSocket, &rfds);

  /* Wait up to five seconds. */

  tv.tv_sec = 5;
  tv.tv_usec = 0;

  retval = select(newSocket+1, &rfds, NULL, NULL, &tv);
  /* Don't rely on the value of tv now! */

  if (retval == -1)
     perror("select()");
  else if (retval){
     printf("Data is available now.\n");
     /* FD_ISSET(socket, &rfds) will be true. */
     recv(newSocket, buffer, BUFFERSIZE, 0);
     printf("2nd play buffer:\n%s\n", buffer);

     if(strcmp(buffer, "exit")== 0){
       printf("player with socket %d exited\n", newSocket);
       resp[newSocket].code = -1;
       pthread_exit(NULL);
     } else{
       sscanf(buffer, "%d %d", &x, &y);
       //pthread_mutex_lock(&lock[x][y]);
       resp[newSocket] = board_play(x, y, newSocket, OKAY);
       printf("2nd play code: %d\n", resp[newSocket].code);

       /*
          unlock mutex if code=0
       */
     }
  }
  else{
    printf("No data within five seconds.\n");
    resp[newSocket] = board_play(0,0,newSocket, CANCEL);
  }

  printf("finished reading 2nd play\n");
  pthread_exit(NULL);
}

void manage_player(char *buffer, int socket, int *done, PlayerList *player)
{
  //char first_play[3];
  int x,y;
  pthread_t tid;

  if(strcmp(buffer, "exit")== 0){
    //remove from list
    printf("player with socket %d exited\n", socket);
    *done = 1;
    return;
  }

  sscanf(buffer, "%d %d", &x, &y);

  //pthread_mutex_lock(&lock[x][y]);
  resp[socket] = board_play(x,y,socket, OKAY);
  printf("play response:\ncode %d\n", resp[socket].code);

  if(resp[socket].code == 0){
    /* filled position */
    //pthread_mutex_unlock(&lock[x][y]);
  }else if(resp[socket].code == 1){
    /* valid first play */
    //fill the position
    resp[socket].code = 0;
    update_color(x,y, player->color);

    //broadcast play to all Players
    memset(buffer, 0, BUFFERSIZE);
    sprintf(buffer, "%d %d %s %d %d %d", x, y, resp[socket].str_play1, player->color[0], player->color[1], player->color[2]);
    pthread_create(&tid, NULL, broadcast_play, (void*)buffer);
    pthread_join(tid, NULL);

    //pthread_mutex_unlock(&lock[x][y]);

    /*
    for(int i=0; i<3; i++)
      first_play[i] = resp[socket].str_play1[i];*/

    //create thread to read the second play
    pthread_create(&tid, NULL, read_secondplay_buffer, (void*)&socket);
    pthread_join(tid, NULL);

    switch (resp[socket].code) {
      /* exit in second play */
      case -1:
        *done = 1;
        // remove player from list
      break;
      case -2:
        // different cards
      break;
      case 0:
        // place filled
      break;
      case 2:
        // correct cards
      break;
    }
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
  PlayerList *curr;

  for(x=0;x<size;x++){
    for(y=0;y<size;y++){
      memset(buffer, 0, BUFFERSIZE);
      i = linear_conv(x,y);
      sprintf(buffer, "%d %d %s %d %d %d", x, y, board[i].v, board[i].color[0], board[i].color[1], board[i].color[2]);
      curr = client_list;
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
    printf("socket: %d for player %d with color %d %d %d and status %d\n", curr->socket, curr->player_id, curr->color[0], curr->color[1], curr->color[2], curr->status);
    curr = curr->next;
    i++;
  }
}

void * first_play_thread(void *socket)
{
  int done = 0;
  int newSocket = *((int*)socket);
  char buffer[BUFFERSIZE];
  PlayerList *player_info;

  player_info = get_last_player(client_list);

  if(player_info == NULL){
    printf("error in list\n");
    close(newSocket);
    pthread_exit(NULL);
  }else{
    printf("Reading 1st play of player %d\n", player_info->player_id);
  }


  while(!done){
    memset(buffer, 0, BUFFERSIZE);
    recv(newSocket, buffer, BUFFERSIZE,0);

    manage_player(buffer, newSocket, &done, player_info);

  }

  close(newSocket);
  pthread_exit(NULL);
}
