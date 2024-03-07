#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/**
 * Structure representing a node in a linked list.
 */
typedef struct node 
{
    int run;           // The run number. 
    double speed;      //The speed achieved in the run.
    double time;       // The time taken for the run.
    struct node* next; // Pointer to the next node in the list. 
} Node;

/**
 * Structure representing a linked list of nodes.
 */
typedef struct _List
{
    Node* _head;    // Pointer to the head of the linked list. 
    size_t _size;   //The number of nodes in the linked list. 
} List;

/**
 * Allocates memory for a new node and initializes its members.
 * 
 * @param run The run number for the node.
 * @param time The time taken for the run.
 * @param speed The speed achieved in the run.
 * @param next Pointer to the next node in the list.
 * @return A pointer to the newly allocated node.
 */
Node* Node_alloc(int run, double time, double speed, Node* next);

/*
 * This function frees the memory allocated to a Node structure.
 * Parameters:
 *   - node: A pointer to the Node structure to be freed.
 */
void Node_free(Node* node);

/*
 * This function allocates memory for a new List structure and initializes its members.
 * The head pointer is set to NULL, indicating an empty list, and the size is initialized to 0.
 * Returns:
 *   - A pointer to the newly allocated List if successful.
 *   - NULL if memory allocation fails.
 */
List* List_alloc();

/*
 * Frees the memory and resources allocated to List.
 */
void List_free(List* List);

/*
 * Inserts element at the end of the List.
 */
void List_insertLast(List* List, int run, double time, double speed);

/*
 * Return the size of the List.
 */
size_t List_size(const List* List);

#endif /* LIST_H */
