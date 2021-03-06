#include "server_th.h"

PlayerList *client_list = NULL;
int size;
pthread_mutex_t **lock;
int nplayers;

void start_ui()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		 printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		 exit(-1);
	}
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
}

int main(int argc, char* argv[]){
  int j;
  int serverSocket, newSocket;
  int *colors;
  int i=0;
  char buffer[BUFFERSIZE] ={'\0'};

  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;

  socklen_t addr_size;

  if (argc!=2){
    printf("Incorrect number of arguments.\n");
    exit(1);
  }

  if(!sscanf(argv[1], "%d", &size)){
    printf("Incorrect format provided for seed\n");
    exit(1);
  }

  if(size%2 != 0){
    printf("Only even numbers allowed for grid size\n");
    exit(1);
  }

  nplayers=0;
  start_ui();
  create_board_window(300, 300,  size);

  printf("initializing a board with size: %d\n", size);
  init_board(size);


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


  //Initializing mutex locks
  /*
  lock = (pthread_mutex_t **)malloc (sizeof(pthread_mutex_t *) * size);
  for(j=0;j<size;j++){
    lock[j]= (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)*size);
  }
  */


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
    colors = get_colors();

   nplayers++;
   client_list = insertLastLinkedList(client_list, newSocket, i, colors, 0, 0);
   i++;
   print_linked_list(client_list);

   memset(buffer, 0, BUFFERSIZE); //erase buffer before inserting data
   sprintf(buffer, "%d %d %d %d", colors[0], colors[1], colors[2], size);
   send(newSocket,buffer,sizeof(buffer),0);

   if(nplayers >= 2){
     //send board
     send_board();
   }
   printf("create thread\n");
    if( pthread_create(&tid[i], NULL, first_play_thread, (void *)&newSocket) != 0 ){
      printf("Failed to create thread\n");
    }

  }
  for(j=0; j<i;j++){
    pthread_join(tid[j],NULL);
  }
  //pthread_mutex_destroy(*lock);
  return 0;
}
