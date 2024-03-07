/*
 this implementation of List is used to store information about multiple runs received by the TCP receiver program.
  This linked list consists of nodes,
   where each node represents a single run and contains details such as the run number, 
   time taken for the run, and the speed achieved.
   */

/**
 * Structure representing a node in a linked list.
 */
typedef struct node 
{
    int run;           // The run number.
    double speed;      // The speed achieved in the run. 
    double time;       // The time taken for the run. 
    struct node* next; // Pointer to the next node in the list. 
} Node;

/**
 * Structure representing a linked list of nodes.
 */
typedef struct _List
{
    Node* _head;    // Pointer to the head of the linked list. 
    size_t _size;   // The number of nodes in the linked list. 
} List;

Node* Node_alloc(int run, double time, double speed, Node* next)
{
    // Allocate memory for a new Node structure
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL)  {
        return -1; // Memory allocation failed
    }
    // Initialize the members of the node structure
    p->run = run;
    p->time = time;
    p->speed = speed;
    p->next = next;
    return p;
}


void Node_free(Node* node) 
{
	free(node);
}


//List implemntaion:

   List* List_alloc()
{
    // Allocate memory for a new List structure
    List* p = (List*)malloc(sizeof(List));
    
    // Check if memory allocation was successful
    if (p != NULL)  {
        // Initialize the members of the List structure
        p->_head = NULL; // Set the head pointer to NULL
        p->_size = 0;    // Initialize the size to 0
    }
    
    return p; // Return a pointer to the newly allocated List
}


void List_free(List* List)
{
    if (List == NULL){
     }

    Node* p1 = List->_head; // Pointer to traverse the linked list.
    Node* p2; // Temporary pointer to hold the next node while freeing the current node.
    
    // Iterate through the linked list and free the memory allocated to each node.
    while (p1)  {
        p2 = p1; // Store the current node in p2.
        p1 = p1->next; // Move to the next node.
        Node_free(p2); // Free the memory allocated to the current node.
    }
    
    free(List); // Free the memory allocated to the List structure itself.
}


void List_insertLast(List* List,int run, double time, double speed)
{
    Node* p = Node_alloc(run, time, speed, NULL);
    if(List->_head == NULL) {
        List->_head = p;
    }else {
        Node* q = List->_head;
        while(q->next) {
            q = q->next;
        }
        q->next = p;
    }
    List->_size++;
}

size_t List_size(const List* List){
size_t s = List->_size;
	return s;
}

