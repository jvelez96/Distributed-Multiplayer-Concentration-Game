
# include "LinkedList.h"

LinkedList *client_list;

void print_linked_list(LinkedList *head){
  LinkedList *curr;
  curr = head;
  while(curr != NULL) {
    printf("%d\n", curr->this);
    curr = curr->next;
  }
}

int main(){
  int n;
  client_list = NULL;

  n=0;
  client_list = insertLastLinkedList(client_list, n);
  n=7;
  client_list = insertLastLinkedList(client_list, n);
  n=2;
  client_list = insertLastLinkedList(client_list, n);

  print_linked_list(client_list);

  return 0;
}
