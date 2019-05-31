
#define LinkedListHeader

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#define ASCII 48
#define BUFFERSIZE 150
#define COLORNR 255
#define MAXPLAYERS 50
#define CANCEL -1
#define OKAY 1

typedef struct LinkedListStruct
{
  int player_id;
  int color[3];
  int socket;
  int status;
  struct LinkedListStruct *next;
}PlayerList;

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


PlayerList *insertLastLinkedList(PlayerList * head, int socket, int player_id ,int *colors, int status)
{
  PlayerList * new;

  /* Memory allocation                                            */
  new = (PlayerList *) malloc(sizeof(PlayerList));

  if(new == NULL){
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }

  /* Initialize new node                                          */
  new->socket = socket;
  new->player_id = player_id;
  new->color[0] =  colors[0];
  new->color[1] =  colors[1];
  new->color[2] =  colors[2];
  new->status = status;
  new->next = NULL;

  if(head == NULL) {
    head = new;
  }else{
    //check for first insertion
    if(head->next == NULL){
        head->next = new;
    } else {
        //else loop through the list and find the last
        //node, insert next to it
        PlayerList *curr;
        curr = head;
        while (curr->next != NULL) {
          curr = curr->next;
        }
        curr->next = new;
    }
  }


  return head;
}

PlayerList *remove_player(PlayerList *head, int player_id){
  PlayerList *curr, *aux;

  curr = head;

  while(curr != NULL){
    if(player_id == curr->player_id){
        if(curr == head){
          head = head->next;
          free(curr);
          return head;
        }else{
          aux = head;
          while(aux->next != curr){
            aux = aux->next;
          }
          aux->next = curr->next;
          free(curr);
        }
    }//found player
    curr = curr->next;
  }
  return head;
}

int main(){
  PlayerList *client_list = NULL;
  int colors[3];
  colors[0] = 255;
  colors[1] = 255;
  colors[2] = 255;

  client_list = insertLastLinkedList(client_list, 0, 0, colors, 0);
  client_list = insertLastLinkedList(client_list, 1, 1, colors, 0);
  client_list = insertLastLinkedList(client_list, 2, 2, colors, 0);
  printf("insert list\n");
  print_linked_list(client_list);
  client_list = remove_player(client_list, 0);
  client_list = insertLastLinkedList(client_list, 3, 3, colors, 0);
  printf("removed player\n");
  print_linked_list(client_list);
  client_list = remove_player(client_list, 3);
  print_linked_list(client_list);


  return 0;
}
