#include "server_th.h"

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void print_linked_list(LinkedList *head){
  LinkedList *curr;
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
  int newSocket = ((client_data*)arg)->curr_socket;
  int thread_nr = ((client_data*)arg)->curr_thread;
  LinkedList *client_list = ((client_data*)arg)->clientlist;
  printf("%d\n", newSocket);
  recv(newSocket , client_message , 2000 , 0);
  // Send message to the client socket
  pthread_mutex_lock(&lock);
  sprintf(buffer, "Thread nº: %d with socket %d\n",thread_nr, newSocket);
  printf("buffer: %s\n", buffer);
  pthread_mutex_unlock(&lock);
  sleep(1);

  printf("Exit socketThread \n");
  while(!done){
    for(i=0;i<((client_data*)arg)->nthreads; i++){
      send(client_list->this,buffer,40,0);
    }
    print_linked_list(client_list);
  }
  close(newSocket);
  printf("Exit socketThread %d\n", thread_nr);
  pthread_exit(NULL);
}
