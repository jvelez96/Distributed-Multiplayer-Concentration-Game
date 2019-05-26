#include "server_th.h"

PlayerList *client_list = NULL;

int main(){
  int nplayers=0, j;
  int serverSocket, newSocket;
  int i=0;

  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;

  client_data *args = (client_data *)malloc(sizeof(client_data));
  args->clientlist = NULL;

  socklen_t addr_size;

  //client_data *clientdata = (client_data *) malloc(sizeof(client_data));

  //Create the socket.
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  // Configure settings of the server address struct
  // Address family = Internet
  serverAddr.sin_family = AF_INET;
  //Set port number, using htons function to use proper byte order
  serverAddr.sin_port = htons(8080);
  //Set IP address to localhost
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);;
  //Set all bits of the padding field to 0
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  //Bind the address struct to the socket
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  //Listen on the socket, with 40 max connection requests queued
  if(listen(serverSocket,50)==0)
    printf("Listening\n");
  else
    printf("Error\n");

  pthread_t tid[60];

  client_list = NULL;

  while(1)
  {
      //Accept call creates a new socket for the incoming connection
      addr_size = sizeof serverStorage;
      newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
      //for each client request creates a thread and assign the client request to it to process
     //so the main thread can entertain next request

     printf("inserir na lista socket: %d\n",newSocket);
     nplayers++;
     client_list = insertLastLinkedList(client_list, newSocket);
     client_list->socket = newSocket;
     client_list->nplayers++;
     //client_list->color[0] = color[0];

    if( pthread_create(&tid[i], NULL, socketThread, (void *)newSocket) != 0 ){
      printf("Failed to create thread\n");
    }
    i++;

    for(j=0; j<i;j++){
      pthread_join(tid[j],NULL);
    }
    printf("acabou\n");
  }
  return 0;
}
