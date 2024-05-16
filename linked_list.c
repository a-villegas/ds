#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DATA STRUCTURES
enum AVAILABLE_CHOICES{
    BEGIN_ZERO         = 0,
    ADD_ELEM           = 1,   // Option to Add an element to the list
    DEL_ELEM           = 2,   // Option to Delete an element from the list
    ENTER_AT_POSITION  = 3,   // Option to Enter an element at x position
    SEARCH_ELEM        = 4,   // Option to Search for an element
    GET_SIZE           = 5,   // Option to Get the size of the list
    PRINT_LIST         = 6,   // Option to Print the list
    QUIT               = 7,   // Option to Quit the program
    UNAVAILABLE        = -1,  // Used as a dummy value
};

struct _Node {
    void * next;
    uint32_t data;
};

typedef struct _Node Node;

struct _List {
    Node * head;
    uint32_t size;
};

typedef struct _List List;
const int32_t buf_size = 10000000;

// PROTOTYPES
static int printmenu(void);
static void printlist( List * listptr);
static inline bool pop_Node( List * listptr);
static inline bool destroy_list(List * listptr);
static inline Node * create_node(uint32_t _data);
static inline bool search(List * listptr, uint32_t key);
static inline bool write_inputfile(const char * filename);
static inline bool push_Node(List * listptr, uint32_t _data);
static inline bool get_elems_from_file(List * listptr, const char * filename);
static inline bool add_elem_at(List * listptr, uint32_t data, uint32_t position);

int main(int argc, char ** argv)
{
    List * list   = (List *)malloc(sizeof(List));
    list->head    = NULL;
    list->size    = BEGIN_ZERO;

    uint32_t data     = UNAVAILABLE;
    uint32_t position = UNAVAILABLE;
    int option        = UNAVAILABLE;
    int cleared       = BEGIN_ZERO;

    if(argc == DEL_ELEM) {
        if(get_elems_from_file(list, argv[ADD_ELEM])) {
            printf("Elements added to the list\n");
        }
    } else if (argc == ENTER_AT_POSITION) {
        if(strcmp(argv[2], "w") == 0){
            if(write_inputfile(argv[1])){
                printf("Done writing input file!!\n");
                return EXIT_SUCCESS;
            }
        }
    }

    do {
        if(cleared > BEGIN_ZERO) {
            getchar();
        }
        option = printmenu();
        switch(option){
            case ADD_ELEM:
                    printf("Enter the element data:");
                    scanf("%d", &data);
                    push_Node(list, data);
                 break;

            case DEL_ELEM:
                   printf("Remove the first element:");
                   pop_Node(list);
                 break;

            case ENTER_AT_POSITION:
                printf("Enter the position and element to add to list:");
                scanf("%d %d", &position, &data);
                printf("position:%d data:%d\n",position, data);
                if(!add_elem_at(list,data,position)){
                    printf("something went wrong!!");
                }
                break;

            case SEARCH_ELEM:
                printf("Enter the element to be searched in the list:");
                scanf("%d",&data);
                if(search(list, data)) {
                    printf("Key %d: found!!!!\n",data);
                } else {
                    printf("key not found!!!!\n");
                }
                 break;

            case GET_SIZE:
                printf("The size of the list:%d\n",list->size);
                break;

            case PRINT_LIST:
                 printf("\nPrinting the list:\n");
                 printf("The size of the list: %d",list->size);
                 printlist(list);
                 break;

            case QUIT:
                printf("Terminating the program.....\n");
                break;

            default:
                printf("This Option is not available...\n");
                break;
        };
        cleared++;
    } while(option != QUIT);

    destroy_list(list);
    if(list) {
        free(list);
    }
    list = NULL;

    return EXIT_SUCCESS;
}

/*************************************************************************
*   This function inserts an element in the list at the given position.
*
*   @param listpr: The list to add an element to
*   @param data:   The data element to be added to list
*   @param position: The position in the list
*   @return :     if successful return true, otherwise false
*
**************************************************************************/
bool add_elem_at(List * listptr, uint32_t data, uint32_t position)
{
    Node * curr_ptr = NULL;
    Node * next_ptr = NULL;
    Node * new_elem = NULL;
    uint32_t curr_pos = 0;
    bool success = false;

    if(!listptr){
        goto Error;
    }

    if(!listptr->head) {
        goto Error;
    }

    if(position > listptr->size || position < 0) {
        goto Error;
    }

    if( position  == ADD_ELEM) { //insert at the beginning
        if(!push_Node(listptr, data)) {
            goto Error;
        }
        success = true;
    } else if(listptr->size > ADD_ELEM){ //insert at position
        next_ptr = listptr->head;
        next_ptr = next_ptr->next;
        curr_ptr = listptr->head;
        curr_pos =2;
        while(next_ptr) {
            if(curr_pos == position){

                new_elem = create_node(data);
                if(!new_elem) {
                    goto Error;
                }

                new_elem->next = next_ptr;
                curr_ptr->next = new_elem;
                (listptr->size)++;
                success = true;                
                break;

            } else {    
                next_ptr = next_ptr->next;
                curr_ptr = curr_ptr->next;
                success  = false;
                curr_pos++;
            }
        }

        curr_ptr = NULL;
        next_ptr = NULL;

    }

    Error:
    if(!success && new_elem){
     new_elem->next = NULL;
     free(new_elem);   
    }    

    return success;
}

/**************************************************************************************
*    This function created a node on the heap and updates the data element for the node
*     it sets the next element to null.                                          
*
*    @param data:   The data element needed to be added to the node
*    @return :      If a node was created successfuly we return the Node *,
*                    otherwise we return NULL
*
**************************************************************************************/
Node * create_node(uint32_t _data)
{
    Node * temp = (Node *) malloc(sizeof(Node));
    if(!temp) {
        printf("Could not allocate a node!!\n");
        goto Error;
    }

    temp->data = _data;
    temp->next = NULL;

    Error:
      return temp;
}

/************************************************************************************
*
*   This function removes all the elements from the list and frees the memory
*   @param listptr:  The list we need to destroy
*   @return:   On success we return true, on failure we return false
*
*************************************************************************************/
static inline bool destroy_list(List * listptr)
{
    bool success = false;
    if(!listptr) {
        success = false;
        goto Error;
    }


    if(!listptr->head) {
        success = false;
        goto Error;
    }

    Node * temp_ptr = listptr->head;
    while(listptr->head) {
        listptr->head = listptr->head->next;
        temp_ptr->next = NULL;
        free(temp_ptr);
        if(listptr->size > 0) {
            (listptr->size)--;
        }
        temp_ptr = listptr->head;
    }

    listptr->head = NULL;
    temp_ptr = NULL;
    success = true;

    Error:
    return success;
}

/**********************************************************************************
*
*   This function reads an input file and adds the data found in the file as
*   elements of the list.
*
*   @param listptr: A pointer to the list we want to add elements from file
*   @param filename: The name of the file being parsed
*   @return :  If file is successfully parsed we return True, Otherwise return false
*
************************************************************************************/
bool get_elems_from_file(List * listptr, const char * filename)
{
    bool success = false;
    int32_t count_num = 0;
    uint32_t buffer[buf_size];
    FILE * file_ptr = fopen(filename, "r");
    if(!file_ptr) {
        goto Error;
    }

    while(!feof(file_ptr)) {
        fscanf(file_ptr, "%d", &buffer[count_num]);
        if(push_Node(listptr, buffer[count_num])) {
            //printf("success adding %d to list!\n", buffer[count_num]);
        }
        count_num++;
    }


    Error:
    if(file_ptr) {
        fclose(file_ptr);
    }
    return success;
}

/***********************************************************************************
*    This function prints the list to standard output
*
*    @param list: The pointer to the list to be printed
*
*************************************************************************************/
void printlist( List * listptr)
{
    if(!listptr){
        printf("The list hasn't been created\n");
        exit (EXIT_FAILURE);
    }

    Node * current = listptr->head;
    uint32_t elemnum = 0;
    printf("\n\t   -------------");
    printf("\n\t   | Head Node: |\n");
    printf("\t    -------------");
    printf("\t\n");
    printf("\t\t   | \n");
    printf("\t\t   | \n");
    printf("\t\t   V \n");
    printf("\t\n");
    if(!current) {
        printf("\tNULL\n");
    } else {
        while(current) {
            if(current->next) {
                printf("\t-------------------------\n");
                printf("\t  Node :  %d              \n\t  data : %d               \n\t  curr : %p  \n\t  next : %p  \n", 
                        ++elemnum, current->data, current, current->next);
                printf("\t-------------------------");
                printf("\t\n");
                printf("\t\t   | \n");
                printf("\t\t   | \n");
                printf("\t\t   V \n");
                printf("\t\n");
            } else {
                printf("\t-------------------------\n");
                printf("\t  Node :  %d       \n\t  data :  %d      \n\t  curr : %p  \n\t  next : %p  \n", 
                        ++elemnum, current->data, current, current->next);
                printf("\t-------------------------\n");                
            }
            current = current->next;
        }
    }
}

/***********************************************************************************
*    This function prints a menu to std output and displays
*    the available options for the program.
*
*    @return: if the choice is not available function returns -1
*              if input is valid it returns option from 1-5... se AVAILABLE_CHOICES
*              for a summary of the options.
*
*************************************************************************************/
int printmenu(void)
{
    int choice = -1;
    char buffer[2];
    printf("\n*********************************************************\n");
    printf("Please select an option from the menu:\n");
    printf("1) Add an element to the list\n");
    printf("2) remove an element from the list\n");
    printf("3) Add an element at position x\n");
    printf("4) Search for an element\n");
    printf("5) Print the list size\n");
    printf("6) Print the list\n");
    printf("7) Quit the program\n");
    fgets(buffer, 2,stdin);
    if(isdigit(buffer[0])) {
        choice = buffer[0] - '0';
    }

    if(choice < ADD_ELEM || choice > QUIT) {
        choice = -1;
    }

    return choice;
}

/*************************************************************************************
*
*  This function searchs if a key value is inside our List.
*  @param  listprt: A pointer to the list we want to search
*  @param key:     A key value we need to look for
*  @return:  If we find the key in our list we return true
*             otherwise return false
*
****************************************************************************************/
bool search(List * listptr, uint32_t key)
{
    bool success = false;
    Node * temp_ptr = NULL;
    if(!listptr) {
        goto Error;
    }

    if(!listptr->head) {
        goto Error;
    }

    temp_ptr = listptr->head;
    while(temp_ptr) {
        if(temp_ptr->data == key) {
            success = true;
            break;
        }
        temp_ptr = temp_ptr->next;
    }

    Error:
    return success;
}

/*************************************************************************
*
*  This function removes an element from the beginning of the list and
*  prints the node's data. Removing from the beginning of a list is O(1)
*  
*  @param listptr: A pointer to a list we need to remove an element from
*  @return:    On success we return True, on failure false
*
**************************************************************************/
bool pop_Node( List * listptr)
{
    bool success = false;

    if(!listptr) {
        perror("The list hasn't been created!!\n");
        goto Error;
    }

    if(!listptr->head) {
        perror("Can't pop an element from an empty list!!\n");
        goto Error;
    }

    Node * temp_ptr = listptr->head;
    listptr->head = listptr->head->next;
    printf("\nWe popped: %d\n", temp_ptr->data);
    (listptr->size)--;
    temp_ptr->next = NULL;
    free(temp_ptr);
    temp_ptr = NULL;
    success = true;

    Error:
    return success;
}

/**************************************************************************************
*    This function adds an element to a list                                          
*
*    @param listptr: A pointer to the list we need to append an element to
*    @param data:   The data element needed to be added to the list
*    @ return :  On success of adding an element to the list return true,
*                otherwise return false.
*
**************************************************************************************/
bool push_Node(List * listptr, uint32_t _data)
{
    //if head is empty add it
    bool success = false;
    if(!listptr) {
        perror("The list is not created!\n");
        goto Error;
    }

    List * current = listptr;
    if(!current->head){

        current->head = create_node(_data);
        if(!listptr->head) {
            goto Error;
        }

        current->size += 1;

    } else {
        Node * temp = create_node(_data);
        if(!temp) {
            goto Error;
        }

        temp->next = current->head;
        current->head = temp;
        (current->size)++;
    }
    listptr = current;
    success = true;

    Error:
    if(current)
        current = NULL;

    return success;
}

/***************************************************************************************
*
*   This function writes a file with buf_size number of integers
*   @param filename: The name of the output file to write
*   @return:  If file is written successfully we return true, otherwise false
*
***************************************************************************************/
static inline bool write_inputfile(const char * filename)
{
    bool success = false;
    FILE * fileptr = NULL;
    char buff[buf_size];

    if(!filename){
        goto Error;
    }

    fileptr = fopen(filename, "w");
    if(!fileptr) {
        goto Error;
    }


    for(uint32_t index=0; index < buf_size; index++) {
        if(index == buf_size - 1) {
            sprintf(buff,"%d",index);
        } else {
            sprintf(buff,"%d ",index);
        }
        fputs(buff, fileptr);
    }

    success = true;

    Error:
    if(fileptr) {
        fclose(fileptr);
    }

    return success;
}