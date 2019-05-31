#include "server_th.h"

extern pthread_mutex_t **lock;
extern PlayerList *client_list;
extern int size;
extern board_place * board;
extern int n_corrects;
extern int play1[MAXPLAYERS][2];
extern play_response resp[MAXPLAYERS];
extern int nplayers;

void update_color(int x, int y, int *color){
  int i;
  i = linear_conv(x,y);
  board[i].color[0] = color[0];
  board[i].color[1] = color[1];
  board[i].color[2] = color[2];

  return;
}

void * broadcast_play(void *args){
  /*
  while(curr != NULL){
    send(curr->socket, (char*) buffer, BUFFERSIZE,0);
    curr = curr->next;
  }
  */
  int socket = ((struct args*)args)->socket;
  char buffer[BUFFERSIZE] = {'\0'};
  strcpy(buffer, ((struct args*)args)->buff);

  printf("sending buffer: %s\nto %d\n", buffer,socket);
  send(socket, (char*) buffer, sizeof(buffer),0);
  pthread_exit(NULL);
}


void * read_secondplay_buffer(void *socket){
  int n;
  int newSocket = *((int*)socket);
  int x,y;
  fd_set rfds;
  struct timeval tv;
  int retval;
  char buffer[BUFFERSIZE]={'\0'};

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
     n=recv(newSocket, buffer, sizeof(buffer),0);
     if(n<0 || n==EOF){
       //remove_player()
       close(newSocket);
       exit(-1);
       pthread_exit(NULL);
     }
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

void broadcast(struct args *broadcast_args){
  PlayerList *curr = client_list;
  int i= 0;
  int j;
  pthread_t tids[MAXPLAYERS];

  // verificar se esta a receber bem os args
  while(curr != NULL){
    //send(curr->socket, (char*) buffer, BUFFERSIZE,0);
    broadcast_args->socket = curr->socket;
    pthread_create(&tids[i], NULL, broadcast_play, (void*)broadcast_args);
    curr = curr->next;
    i++;
  }
  //pthread_create(&tid, NULL, broadcast_play, (void*)buffer);
  for(j=0;j<i;j++)
    pthread_join(tids[j], NULL);
}

void manage_player(char *buffer, int socket, int *done, PlayerList *player)
{
  int x,y;
  pthread_t tid;
  struct args *broadcast_args = (struct args *)malloc(sizeof(struct args));
  //struct args *broadcast_args;
  printf("manage player\n");
  if(strcmp(buffer, "exit")== 0){
    //remove from list
    client_list = remove_player(client_list, player->player_id);
    printf("player with socket %d exited\n", socket);
    nplayers--;
    *done = 1;
    return;
  }

  sscanf(buffer, "%d %d", &x, &y);
  printf("1st play x:%d y:%d\n", x, y);

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
    memset(broadcast_args->buff, 0, BUFFERSIZE);
    sprintf(broadcast_args->buff, "1 %d %d %s %d %d %d", x, y, resp[socket].str_play1, player->color[0], player->color[1], player->color[2]);

    broadcast(broadcast_args);
    /*
    curr = client_list;
    while(curr != NULL){
      //send(curr->socket, (char*) buffer, BUFFERSIZE,0);
      broadcast_args->socket = curr->socket;
      pthread_create(&tids[i], NULL, broadcast_play, (void*)broadcast_args);
      curr = curr->next;
      i++;
    }
    //pthread_create(&tid, NULL, broadcast_play, (void*)buffer);
    for(j=0;j<i;j++)
      pthread_join(tids[j], NULL);
      */


    //pthread_mutex_unlock(&lock[x][y]);

    /*
    for(int i=0; i<3; i++)
      first_play[i] = resp[socket].str_play1[i];*/

    //create thread to read the second play
    pthread_create(&tid, NULL, read_secondplay_buffer, (void*)&socket);
    pthread_join(tid, NULL);

    //broadcast_args->buff[BUFFERSIZE] ={'\0'};
    memset(broadcast_args->buff, 0, BUFFERSIZE);
    switch (resp[socket].code) {
      /* exit in second play */
      case -1:
        *done = 1;
        // remove player from list
        client_list = remove_player(client_list, player->player_id);
        nplayers--;
        // send card down
        //broadcast play to all Players
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "0 %d %d 255 255 255", resp[socket].play1[0], resp[socket].play1[1]);

        broadcast(broadcast_args);


      break;
      case -2:
        // different cards
        // send 2 cards down

        //broadcast play to all Players 2nd play
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "1 %d %d %s %d %d %d",resp[socket].play2[0] , resp[socket].play2[1], resp[socket].str_play2, player->color[0], player->color[1], player->color[2]);
        //pthread_mutex_unlock(&lock[resp[socket].play2[0]][resp[socket].play2[1]]);

        broadcast(broadcast_args);

        sleep(2);

        //broadcast play to all Players cards down
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "0 %d %d 255 255 255", resp[socket].play2[0], resp[socket].play2[1]);

        broadcast(broadcast_args);

        //broadcast play to all Players
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "0 %d %d 255 255 255", resp[socket].play1[0], resp[socket].play1[1]);

        broadcast(broadcast_args);



      break;
      case 0:
        // place filled
        resp[socket] = board_play(x,y,socket, CANCEL);

        //broadcast play to all Players
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "0 %d %d 255 255 255", resp[socket].play1[0], resp[socket].play1[1]);

        broadcast(broadcast_args);

      break;
      case 2:
        // correct cards

        //broadcast play to all Players
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "1 %d %d %s %d %d %d", x, y, resp[socket].str_play1, player->color[0], player->color[1], player->color[2]);

        broadcast(broadcast_args);
        //pthread_mutex_unlock(&lock[resp[socket].play2[0]][resp[socket].play2[1]]);
      break;
      case 3:
        //broadcast play to all Players
        //memset(broadcast_args->buff, 0, BUFFERSIZE);
        sprintf(broadcast_args->buff, "1 %d %d %s %d %d %d", x, y, resp[socket].str_play1, player->color[0], player->color[1], player->color[2]);

        broadcast(broadcast_args);
        //pthread_mutex_unlock(&lock[resp[socket].play2[0]][resp[socket].play2[1]]);
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
  char buffer[BUFFERSIZE] = {'\0'};
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
          send(curr->socket, buffer, sizeof(buffer),0);
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
  char buffer[BUFFERSIZE]= {'\0'};
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
    n=recv(newSocket, buffer, sizeof(buffer),0);
    if(n<0 || n==EOF){
      //remove_player()
      close(newSocket);
      exit(-1);
      pthread_exit(NULL);
    }
    printf("first play received %s\n", buffer);

    manage_player(buffer, newSocket, &done, player_info);

  }

  close(newSocket);
  pthread_exit(NULL);
}
