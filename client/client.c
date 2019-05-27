#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#include <sys/socket.h>
#include <arpa/inet.h>

#include "UI_library.h"

int main(){

	//Create session with the server
	int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

	SDL_Event event;
	int done = 0;

	 if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		 printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		 exit(-1);
	}
	if(TTF_Init()==-1) {
			printf("TTF_Init: %s\n", TTF_GetError());
			exit(2);
	}

	
	char buff[MAX];
	bzero(buff, MAX);
	read(sockfd, buff, sizeof(buff));
	int dim, color_0, color_1, color_2;
	sscanf(buff, "%d %d %d %d", &color_0, &color_1, &color_2, &dim);
	printf("jogada (%d,%d,%d,%d)", color_0, color_1, color_2, dim);
	
	create_board_window(300, 300,  dim);

	while (!done){
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
					write(sockfd, buff, sizeof(buff));

					bzero(buff, MAX);
					read(sockfd, buff, sizeof(buff));

					int code, board_x, board_y, aux_x, aux_y;
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
	}
	printf("fim\n");
	close_board_windows();
}
