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

typedef void * Item;



/******************************************************************************
 *  Data type: LinkedList
 *
 *  Description: Node of a linked list
 ******************************************************************************/
typedef struct LinkedListStruct
{
  int nplayers;
  int color[3];
  int socket;
  Item this;
  struct LinkedListStruct *next;
}PlayerList;




/******************************************************************************
 *  Function:
 *    initLinkedList
 *
 *  Description:
 *    Initializes a new linked list.
 *
 *  Arguments:
 *    None
 *
 *  Return value:
 *    Returns the pointer to a new linked list.
 ******************************************************************************/
PlayerList* initLinkedList(void);



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
 *    getItemLinkedList
 *
 *  Description:
 *    Gets the item of a linked list node.
 *
 *  Arguments:
 *    Pointer to a linked list node:
 *        (PlayerList*) node
 *
 *  Return value:
 *    Returns the pointer to the item of a linked list node. NULL
 *   is returned if the node is NULL (or if the item is NULL).
 ******************************************************************************/
Item getItemLinkedList(PlayerList* node);



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

PlayerList* insertLastLinkedList(PlayerList* head, Item this);



/******************************************************************************
 *  Function:
 *    InsertSortedLinkedList
 *
 *  Description:
 *    Inserts an item in order in an sorted linked list.
 *
 *  Arguments:
 *    Pointer to the first node of a sorted linked list:
 *        (PlayerList*) first
 *    Pointer to item to be inserted:
 *        Item item
 *    Pointer to function to compare two items:
 *        int comparisonItemFnt(void * item1, void * item2)
 *
 *        This function returns a value less, equal, or greater
 *       than zero if item1 compares less, equal, or greater than
 *       item2, respectively.
 *
 *    Pointer to integer to write error return value:
 *        (int *) err
 *
 *        0 upon sucess, 1 in case the item is NULL, and 2 in
 *   case of memory allocation failure.
 *
 *  Return value:
 *    Returns the pointer to the first node of the sorted linked list.
 ******************************************************************************/
PlayerList* insertSortedLinkedList(PlayerList * first,
                           Item item,
                           int (* comparisonItemFnt)
                           (Item item1, Item item2),
                           int * err);



/* End of: Protect multiple inclusions                              */
#endif
