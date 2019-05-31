/******************************************************************************
 *  File name: LinkedList.c
 *
 *  Author: 2013 Fall Semester AED Team
 *
 *  Release date: 2013/10/04
 *
 *  Description: C-Implementation of an Abstract Linked List
 *
 *  Implementation details:
 *    Linked list node: Each node of the list contains a pointer to
 *   the item stored in the node and a pointer to the next node. The
 *   item is specified in the file defs.h.
 *
 *  Version: 1.0
 *
 *  Change log: N/A
 *
 ******************************************************************************/

/* Include Header File with Data Type and Function Prototypes     */
#include "LinkedList.h"

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
PlayerList * initLinkedList(void)
{
  return NULL;
}



/******************************************************************************
 *  Function:
 *    freeLinkedList
 *
 *  Description:
 *    Frees the memory allocated to a linked list.
 *
 *  Arguments:
 *    Pointer to the first element of a linked list:
 *      (LinkedList *) first
 *
 *  Return value:
 *    None
 ******************************************************************************/
void freeLinkedList(PlayerList * first)
{
  PlayerList * next;
  PlayerList * aux;

  /* Cycle from the first to the last element                     */
  for(aux = first; aux != NULL; aux = next)
  {
    /* Keep trace of the next node                                */
    next = aux->next;

    /* Free current item                                          */
	   //free(aux->this);

    /* Free current node                                          */
    free(aux);
  }

  return;
}



/******************************************************************************
 *  Function:
 *    lengthLinkedList
 *
 *  Description:
 *    Determines the length of a linked list.
 *
 *  Arguments:
 *    Pointer to the first node of the linked list:
 *        (LinkedList *) first
 *
 *  Return value:
 *    Returns the length of the linked list.
 ******************************************************************************/
int lengthLinkedList(PlayerList * first)
{
  PlayerList * aux;
  int counter;

  /* Length determination cycle                                   */
  for(aux = first, counter = 0;
      aux!=NULL;
      counter++, aux = aux->next);

  return counter;
}



/******************************************************************************
 *  Function:
 *    getNextNodeLinkedList
 *
 *  Description:
 *    Returns the next node of a linked list.
 *
 *  Arguments:
 *    Pointer to the current linked list node:
 *        (LinkedList *) node
 *
 *  Return value:
 *    Returns the pointer to the next node of a linked list. NULL
 *   is returned in case the current node is empty or there is no
 *   node following the current node.
 ******************************************************************************/
PlayerList * getNextNodeLinkedList(PlayerList * node)
{
	if(node == NULL || node->next == NULL)
		return NULL;

	return node->next;
}




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
 *      (LinkedList *) next
 *
 *  Return value:
 *    Returns the pointer to the node.
 ******************************************************************************/
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

PlayerList *get_last_player(PlayerList *head){
  PlayerList *curr;

  curr = head;
  printf("starting player %d\n\n", curr->player_id);

  while(curr->next != NULL){
    curr = curr->next;
  }
  printf("returning player %d\n\n", curr->player_id);
  return curr;
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
        while(aux->next->player_id != player_id || aux->next != NULL){
          aux = aux->next;
        }
        aux->next = curr->next;
        free(curr);
      }
    }
    curr = curr->next;
  }
  return head;
}
