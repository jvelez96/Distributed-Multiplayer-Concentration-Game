/******************************************************************************
 *  File name: LinkedList.h
 *
 *  Author: 2013 Fall Semester AED Team
 *
 *  Release date: 2013/10/04
 *
 *  Description: Header file for an Abstract Implementation of a
 *               Linked List
 *
 *  Data type list:
 *    Linked list node: LinkedList
 *
 *  Function list:
 *    A) Initialization & Termination
 *        initLinkedList
 *        freeLinkedList
 *
 *    B) Properties
 *        lengthLinkedList
 *
 *    C) Navigation
 *        getNextNodeLinkedList
 *
 *    D) Lookup
 *        getItemLinkedList
 *
 *    E) Insertion
 *        insertUnsortedLinkedList
 *
 *    F) Sorted linked lists
 *       insertSortedLinkedList
 *
 *  Dependencies:
 *    stdio.h
 *    stdlib.h
 *    defs.h
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/


/* Prevent multiple inclusions                                      */
#ifndef LinkedListHeader
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

typedef void * Item;



/******************************************************************************
 *  Data type: LinkedList
 *
 *  Description: Node of a linked list
 ******************************************************************************/
typedef struct LinkedListStruct
{
  int player_id;
  int color[3];
  int socket;
  int status;
  struct LinkedListStruct *next;
}PlayerList;

/******************************************************************************
 *  Function:
 *    freeLinkedList
 *
 *  Description:
 *    Frees the memory allocated to a linked list.
 *
 *  Arguments:
 *    Pointer to the first element of a linked list:
 *      (PlayerList*) first
 *    Function to free the memory allocated to the items:
 *      void freeItem(Item)
 *
 *  Return value:
 *    None
 ******************************************************************************/
void freeLinkedList(PlayerList* first);



/******************************************************************************
 *  Function:
 *    lengthLinkedList
 *
 *  Description:
 *    Determines the length of a linked list.
 *
 *  Arguments:
 *    Pointer to the first node of the linked list:
 *        (PlayerList*) first
 *
 *  Return value:
 *    Returns the length of the linked list.
 ******************************************************************************/
int lengthLinkedList(PlayerList* first);



/******************************************************************************
 *  Function:
 *    getNextNodeLinkedList
 *
 *  Description:
 *    Returns the next node of a linked list.
 *
 *  Arguments:
 *    Pointer to the current linked list node:
 *        (PlayerList*) node
 *
 *  Return value:
 *    Returns the pointer to the next node of a linked list. NULL
 *   is returned in case the current node is empty or there is no
 *   node following the current node.
 ******************************************************************************/
PlayerList* getNextNodeLinkedList(PlayerList* node);







/******************************************************************************
 *  Function:
 *    insertUnsortedLinkedList
 *
 *  Description:
 *    Creates a new linked list node.
 *
 *  Arguments:
 *    Item to associate to the new node:
 *      Item this
 *    Pointer to the next node:
 *      (PlayerList*) next
 *
 *  Return value:
 *    Returns the pointer to the new node.
 ******************************************************************************/
//PlayerList* insertUnsortedLinkedList(PlayerList* next, Item this);

PlayerList *insertLastLinkedList(PlayerList * head, int socket, int player_id ,int *colors, int status);

PlayerList *get_last_player(PlayerList *head);


/* End of: Protect multiple inclusions                              */
#endif
