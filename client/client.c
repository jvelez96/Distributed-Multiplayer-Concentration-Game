#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#define MAX 150
#define PORT 8080
#define SA struct sockaddr
#include <sys/socket.h>
#include <arpa/inet.h>
#include<pthread.h>



#include "UI_library.h"

int create_socket( char * ip, int port )
{
	//Create session with the server
	int sockfd;
    struct sockaddr_in servaddr;

    // socket create and varification
    sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd == -1 ) {
        printf( "socket creation failed...\n" );
        exit( 0 );
    }
    else
        printf( "Socket successfully created..\n" );
    bzero( &servaddr, sizeof( servaddr ));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr( ip );
    servaddr.sin_port = htons( port );

	// connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

	return sockfd;
}

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

// void * client_listening_server(void * arg)
// {	printf("In thread listening plays\n");
	// int fd;
	// buff[MAX];

	// fd = create_socket( "127.0.0.1", 8888 );

	// bzero(buff, MAX);
	// recv(sockfd, buff, sizeof(buff), 0);
	// sscanf(buff, "%d %d %c%c %d %d %d", &aux_x, &aux_y, &xx[0], &xx[1], &color_r, &color_g, &color_b);
	// printf("Lido: %d %d %c%c %d %d %d\n", aux_x, aux_y, xx[0], xx[1], color_r, color_g, color_b);

	// xx[3] = '\0';

	// paint_card(aux_x, aux_y, color_r, color_g, color_b);
	// write_card(board_x, board_y, xx, color_r, color_g, color_b);
// }

void get_board(int dim, int sockfd)
{
		char buffer[MAX] = {'\0'};
		int i,j,n;
		int aux_x, aux_y;
    char xx[3];
    int color[3];

		printf("get_board começa aqui! :\n\n");

    for(i = 0; i < pow(dim,2); i++)
		{

				memset(buffer, 0, MAX);
				n = recv(sockfd, buffer, sizeof(buffer),0);
				if (n < 0 || n ==EOF)
        {
						close(sockfd);
            perror("error reading");
            exit(-1);
        }
				printf("buffer -> %s\ni -> %d\n", buffer, i);
				sscanf(buffer, "%d %d %s %d %d %d", &aux_x, &aux_y, xx, &color[0], &color[1], &color[2]);
				//printf("casa %d\ncoordenadas: %d %d\nletras: %s\ncor: %d %d %d\n", i, aux_x, aux_y, xx, color[0], color[1], color[2]);
				paint_card(aux_x, aux_y, color[0], color[1], color[2]);
				write_card(aux_x, aux_y, xx, 255,255,255);
				printf("casa %d\ncoordenadas: %d %d\nletras: %s\ncor: %d %d %d\n", i, aux_x, aux_y, xx, color[0], color[1], color[2]);
		}
}

void * manage_sdlEvents(void *socket)
{
    int done = 0;
		int sockfd = *((int*)socket);
    SDL_Event event;
    char buffer[MAX]= {'\0'};

    while (!done)
    {
        while (SDL_PollEvent(&event))
        {
					switch (event.type)
					{
						case SDL_QUIT:
						{
							memset(buffer,0,MAX);
							sprintf(buffer, "exit");
							send(sockfd, buffer, sizeof(buffer),0);
							done = SDL_TRUE;
							break;
						}

						case SDL_MOUSEBUTTONDOWN:
						{
								int x,y;
								get_board_card(event.button.x, event.button.y, &x, &y);
								memset(buffer,0,MAX);
								sprintf(buffer, "%d %d", x, y);
								send(sockfd, buffer, sizeof(buffer),0);

						}

					}
				}

		}
		pthread_exit(NULL);
}


void *play(int sockfd)
{
	char buffer[MAX] = {'\0'};
	int done = 0;
	int n;

	while (!done)
	{
		memset(buffer, 0, MAX);
		int code = 0;
		int playX, playY;
		int color[3];
		char str_place[3];
		int n,n2;


		//char aux[3], aux1[3];
		//aux[2] = '\0';
		//aux1[2] = '\0';
		//sscanf(buff, "%d,%d,%d,%s", &code, &board_x, &board_y, aux);

		n = recv(sockfd, buffer, sizeof(buffer),0);
		if (n < 0 || n ==EOF)
		{
				close(sockfd);
				perror("error reading");
				exit(-1);
		}

		sscanf(buffer, "%d", &code);

		switch(code)
		{
				case 0:
					sscanf(buffer,"0 %d %d 255 255 255", &playX, &playY);
					paint_card(playX, playY , 255, 255, 255);
	        break;

				case 1:
					sscanf(buffer,"1 %d %d %s %d %d %d", &playX, &playY, str_place, &color[0], &color[1], &color[2]);
					paint_card(playX, playY , color[0], color[1], color[2]);
					write_card(playX, playY , str_place, 200, 200, 200);
		      break;

				case 2:
					sscanf(buffer,"1 %d %d %s %d %d %d", &playX, &playY, str_place, &color[0], &color[1], &color[2]);
					paint_card(playX, playY, color[0], color[1], color[2]);
					write_card(playX, playY, str_place, 200, 200, 200);
					memset(buffer, 0, MAX);
					n2 = recv(sockfd, buffer, sizeof(buffer),0);
					if (n < 0 || n ==EOF)
	        {
							close(sockfd);
	            perror("error reading");
	            exit(-1);
	        }
					printf("%s", buffer);
		      break;
		}




	}
}

int main(){

	//Create session with the server
	int sockfd;
  struct sockaddr_in servaddr, cli;
	char buffer[MAX] = {'\0'};
	pthread_t events_thread;
	int n;

	//int done = 0;
	SDL_Event event;

	//Player and board constants
	int dim, color_0, color_1, color_2;
	int code, board_x, board_y;
	char xx [3];

	sockfd = create_socket( "127.0.0.1", PORT );
	start_ui();

	memset(buffer, 0, MAX);
	n = recv(sockfd, buffer, sizeof(buffer),0);
	if (n < 0 || n ==EOF)
	{
			close(sockfd);
			perror("error reading");
			exit(-1);
	}
	sscanf(buffer, "%d %d %d %d", &color_0, &color_1, &color_2, &dim);
	printf("first information (%d,%d,%d,%d)\n", color_0, color_1, color_2, dim);

	/*bzero(buff, MAX);
	recv(sockfd, buff, sizeof(buff), 0);
	printf("received buffer %s\n", buff);
	sscanf(buff, "%d %d %c%c %d %d %d", &aux_x, &aux_y, &xx[0], &xx[1], &color_r, &color_g, &color_b);
	printf("Lido: %d %d %c%c %d %d %d\n", aux_x, aux_y, xx[0], xx[1], color_r, color_g, color_b);*/


	xx[2] = '\0';

	create_board_window(300, 300,  dim);

	get_board(dim,sockfd);

	pthread_create(&events_thread, NULL, manage_sdlEvents, (void*)&sockfd);

	play(sockfd);
	//paint_card(aux_x, aux_y, color_r, color_g, color_b);
	//write_card(aux_x, aux_y, xx, color_r, color_g, color_b);

	//exit(0);



	/*while (!done){
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					done = SDL_TRUE;
					break;
				}
				case SDL_MOUSEBUTTONDOWN:{
					printf("click (%d %d)\n", event.button.x, event.button.y);

					//TO DO: funcao que comunica a jogada para o servidor

					bzero(buff, MAX);
					sprintf(buff, "(%d,%d)", event.button.x, event.button.y);
					send(sockfd, buff, sizeof(buff), 0);

					bzero(buff, MAX);
					recv(sockfd, buff, sizeof(buff), 0);


					char aux[3], aux1[3];
					aux[2] = '\0';
					aux1[2] = '\0';
					sscanf(buff, "%d,%d,%d,%s", &code, &board_x, &board_y, aux);
					printf("jogada (%d,%d,%d,%s)", code, board_x, board_y, aux);

					//Actualizes the board
					switch (code) {
						case 1:
							strcpy(aux1,aux);
							paint_card(board_x, board_y, color_0, color_1, color_2);
							write_card(board_x, board_y, aux1, 200, 200, 200);
							aux_x = board_x;
							aux_y = board_y;
							break;
						case 3:
						  done = 1;
						case 2:
							paint_card(aux_x, aux_y, color_0, color_1, color_2);
							write_card(aux_x, aux_y, aux1, 0, 0, 0);
							paint_card(board_x, board_y , color_0, color_1, color_2);
							write_card(board_x, board_y, aux, 0, 0, 0);
							break;
						case -2:
							paint_card(aux_x, aux_y, color_0, color_1, color_2);
							write_card(aux_x, aux_y, aux1, 255, 0, 0);
							paint_card(board_x, board_y , color_0, color_1, color_2);
							write_card(board_x, board_y, aux, 255, 0, 0);
							sleep(2);
							paint_card(aux_x, aux_y , 255, 255, 255);
							paint_card(board_x, board_y, 255, 255, 255);
							break;
					}
				}
			}
		}
	}*/
	printf("fim\n");
	close_board_windows();
}
