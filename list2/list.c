#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int LIST_NODE_SIZE = 3;

struct node {
    int data;
    struct node * next;
};


int addAtTail(struct node * head, int _data);
int Count(struct node * list, int key);
int GetNth(struct node * list, int position);
int Length(struct node * head);
int Pop(struct node ** headRef);
struct node * BuildOneTwoThree();
struct node * createNode(int _data);
void DeleteList(struct node ** ListPtr);
void printList(struct node * List);
void Push(struct node ** headRef, int newData);

// UNIT TEST FUNCTIONS
void BasicsCaller();
void DeleteListTest();
void GetNthTest();
void PopTest();

int main(int argc, char * argv[])
{
    BasicsCaller();
    return EXIT_SUCCESS;
}

//##################################### UNIT TEST FUNCTIONS: ###############################

void BasicsCaller()
{
    struct node * head;
    int key = 69;
    int len;

    head = BuildOneTwoThree(); // Start with {1, 2, 3}
    Push(&head, 13); // Push 13 on the front, yielding {13, 1, 2, 3}
                     // (THe '&' is because head is passed as a reference pointer.)
    Push(&(head->next), 42); //Push 42 into the second position {13, 42 , 1, 2 ,3}
    addAtTail(head, key);
    Push(&(head->next->next), key);
    Push(&head, key);
    len = Length(head);  //Computes the length = 6;
    printf("List size: %d\n",len);
    printList(head);
    printf("key %d: occurs %d times\n", key, Count(head, key));

    printf("------------------\nUNIT TESTING:\n");
    DeleteList(&head);
    GetNthTest();
    DeleteListTest();
    PopTest();
}

void GetNthTest()
{
    int key = 3;
    struct node * mylist = BuildOneTwoThree();
    printf("List size: %d\n", Length(mylist));
    printList(mylist);
    printf("key %d: occurs %d times\n", key, Count(mylist, key));
    if(mylist) {
        int lastNode = GetNth(mylist, 2);
        printf("GetNth Element : %d\n",lastNode);

        lastNode = GetNth(mylist, 0);
        printf("GetNth Element : %d\n",lastNode);

        //lastNode = GetNth(mylist, -1);
        //printf("GetNth Element : %d\n",lastNode);

        //lastNode = GetNth(mylist, (Length(mylist) + 1));
        //printf("GetNth Element : %d\n",lastNode);
    }
    DeleteList(&mylist);
}

void PopTest()
{
    struct node * head = BuildOneTwoThree();
    int a = Pop(&head);
    int b = Pop(&head);
    int c = Pop(&head);
    printf("pop 1: %d \npop 2: %d\npop 3: %d\n",a,b,c);
    int len = Length(head);
    printf("List size after all pops: %d\n",len);
    DeleteList(&head);
}

void DeleteListTest()
{
    struct node * list = BuildOneTwoThree();
    if(list) {
        printf("List size: %d\n", Length(list));
        printList(list);
        DeleteList(&list);
        printf("List size: %d\n", Length(list));
        printList(list);
    }
}

//############## END UNIT TEST FUNCTIONS ###########################

/****************************************************************************
*   Adds an element at the end of the list O(N) ~Iterates to the end of list
*   @param head: the head of te list
*   @param data: data to add to list 
*   @return: On success return 1, on failure return 0
*****************************************************************************/
int addAtTail(struct node * head, int _data)
{
    int SUCCESS = 0;
    if(!head) { 
        printf("Something went wrong\n");
        goto Error;
    }
    
    struct node * newElem = createNode(_data);
    if(!newElem) {
        printf("Something went wrong\n");
        goto Error;
    }

    struct node * tail = head;
    while(tail) {
        if(!tail->next) {
            
            tail->next = newElem;
            break;
        }
        tail = tail->next;
    }
    SUCCESS = 1;

    Error:
    return SUCCESS;
}

/********************************************************************
*     Allocates and returns the list {1, 2, 3}
*     @return: On success it returns list with elements {1,2,3}
*              On failure returns NULL
********************************************************************/
struct node * BuildOneTwoThree()
{
    struct node * headList = NULL;
    struct node * temp = NULL;
    for( int index = 3; index > 0; index-- ) {
        temp = createNode(index);
        if(!temp) {
            goto Error;
        }

        if(!headList) {
            headList = temp;
        } else {
            temp->next = headList;
            headList = temp;
        }

    }

    return headList;

    Error:
    return NULL;
}

/*************************************************************************************
*   Searches for the number of iterations a key has in the list: O(N) ~Iterates List
*   @param list: the head of te list
*   @param key:  element to look for in the list 
*   @return: On success returns the number of iterations of key in the list
*            On failure returns 0.
**************************************************************************************/
int Count(struct node * list, int key)
{
    int count = 0;
    if(!list) {
        printf("The list is empty\n");
        goto Error;
    }

    struct node * current = list;
    while(current) {
        if(current->data == key){
            count++;
        }
        current = current->next;
    }
    Error:
    return count;
}

/**************************************************************************************
*  Function creates a Node and initializes the data field O(1)
*  @param _data:  Element data for a node list
*  @return: On success returns a struct node with data field populated, and next=NULL
*           On failure returns NULL
***************************************************************************************/
struct node * createNode(int _data)
{
    struct node * temp = (struct node *) malloc(sizeof(struct node));
    if(!temp) {
        printf("\nCouldn't allocate memory for a node!!!\n");
        goto Error;
    }

    temp->data = _data;
    temp->next = NULL;

    Error:
    return temp;
}

/*******************************************************************************
*   This Function Deletes all the elements from a List O(N) ~Iterates the list
*   @param ListPtr: A pointer to the Head List node
*******************************************************************************/
void DeleteList(struct node ** ListPtr)
{
    if(*ListPtr) {
        struct node * headPtr = *ListPtr;
        while(headPtr) {
            (*ListPtr) = headPtr->next;
            headPtr->next = NULL;
            free(headPtr);
            headPtr = *ListPtr;
        }
        
        if(headPtr){
            free(headPtr);
        }

        *ListPtr = NULL;
    }
}

/*******************************************************************
*   This Function returns the element at the Nth place
*   @param list: A pointer to the list we need to iterate
*   @param position: The Nth position we need to obtain
*   @return: On Success it returns the element at the Nth position
*            On Failure returns -99999
********************************************************************/
int GetNth(struct node * list, int position)
{
    int element = -99999;
    //assert(position > 0 || position < (Length(list) - 1));
    if( position < 0 || position > (Length(list) - 1)) {
        printf("Error: Program Asserted!!\n");
        exit(EXIT_FAILURE);
    }

    if(!list) {
        goto Error;
    }

    struct node * current = list;
    while(current) {
        if(position == 0){
            element = current->data;
            break;
        }
        current = current->next;
        position--;
    }

    Error:
    return element;
}

/***********************************************************************
*   Returns the number of nodes in the list O(N) ~iterates the list
*   @param head: pointer to the list we need to get the size of
*   @return: Returns the number of elements in the list, zero if empty
************************************************************************/
int Length(struct node * head)
{
    int size = 0;
    if(!head) {
        goto Error;
    }

    struct node * current = head;
    while(current != NULL) {
        if(current) {
            size++;
        }
        current = current->next;
    }


    Error:
    return size;
}

/**************************************************************************
*   This function deletes the head node and returns its data O(1)
*   @param headRef: a reference to the list
*   @return:  On success, it returns the data element of the deleted node
*              On Failure, returns -99999
***************************************************************************/
int Pop(struct node ** headRef)
{
    int result = -99999;
    if(!(*headRef)) {
        goto Error;
    }

    struct node * current = *headRef;
    (*headRef) = current->next;
    current->next = NULL;
    result = current->data;

    free(current);
    current = NULL;

    Error:
    return result;
}

/***************************************************************
*   Prints the elements of a List O(N) ~iterates the whole list
*   @param list: the head of te list
****************************************************************/
void printList(struct node * List)
{
    if(List) {
        struct node * current = List;
        printf("{ ");
        while(current) {
            printf("%d ", current->data);
            current = current->next;
        }
        printf("}\n");
    } else {
        printf("NULL List\n");
    }
}

/*****************************************************************
*   This function adds a node to the head of the list O(1)
*   @param headRef: a reference to the list
*   @param newData: The data to be included in the new node
******************************************************************/
void Push(struct node ** headRef, int newData)
{
    if(headRef){
        struct node * newNode = createNode(newData);
        if(newNode){
            newNode->next = *(headRef);
            *(headRef) = newNode;
        }
    }
}