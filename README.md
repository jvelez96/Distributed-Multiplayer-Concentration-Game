# Projeto de PSis

## Instruções GIT
1. Instalar git
2. Configurar o git com a tua conta:
$ git config --global user.name "USERNAME"
$ git config --global user.email "MY_NAME@example.com"

3. Fazer clone do repositório para a pasta de PSis:
$ git clone https://github.com/jvelez96/PSis.git

4. Antes de trabalhares fazer sempre pull (fazer o download do estado atual do repositório, com as alterações que fiz):
$ git pull

5. À medida que vais trabalhando, sempre que acabas de fazer qq coisa e está bem e testada fazes o upload para o Git:
$ git add -A

isto para dizeres que adicionas todos os ficheiros que tens nessa pasta, e depois fazes commit para ficar guardado o que vais fazendo, sem enviar os ficheiros:

$ git commit -m "MENSAGEM COM AQUILO QUE FIZESTE"

E depois quando quiseres enviar os ficheiros que fizeste commit para o repositório:

$ git push

## TODOs
1. Compile and run the example
  Para compilar:
  $ sudo apt-get install libsdl2-dev
  $ sudo apt-get install libsdl2-ttf-dev
  $ gcc memory-single.c board_library.c UI_library.c -lSDL2main -lSDL2 -lSDL2_ttf -o memory

2. Study the board manipulation functions
3. Define the messages exchanged between the clients and servers
  a. What information is sent from a clients to the server
  b. What messages are sent from the server to all clients?
4. Implement a server that receives a connection from a single player
  a. Guarantee some of the game rule (wait times)
5. Implement a UI client
6. Change the server to access multiple concurrent players
  a. Assignment of a color to each player
  b. Storage of the player information (sockets, state, color)
  c. Communication of board updates to all clients
7. Implement a bot
8. Implement correctly:
  a. the game start (two players are required)
  b. Addition of a new player during a game
  c. Pause of the game when only one player is connected
  d. Game end
9. Implementation suitable synchronization
