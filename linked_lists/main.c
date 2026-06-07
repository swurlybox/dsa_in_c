#include <stdio.h>
#include <stdlib.h>

/* Generic Linked List implementation. Caller is responsible for casting data
    into a known data type for ease of access. */
typedef struct ll_node {
    struct ll_node *next;   // 8 bytes?
    void *data;             // 8 bytes?
} ll_node_t;

/* Generic API for linked list manipulation. */

/* Allocates memory for a node, and assigns a reference to that memory through
    the passed-in pointer. After execution, the user passed-pointer will point
    to the allocated memory or NULL if memory allocation failed. Intention
    is that *head should point to NULL; if it already points to a non-null
    memory address that is valid memory, that reinitializing the pointer will
    cause a loss of reference to the initial memory. Doing this is entirely
    the user's fault, but we'll add the restriction that *head must have a 
    valid of NULL in order to work. */
void ll_node_init(ll_node_t **head, void *data) {
    if (*head == NULL) {
        *head = malloc(sizeof(ll_node_t));
        if (*head == NULL) {
            printf("ll_node_init: Failed to allocate memory"
                    "for a linked node\n");
            return;
        }
        else {
            (*head)->next = NULL;
            (*head)->data = data;
            printf("ll_node_init: ll_node allocated and initialized\n");
            return;
        }
    }
    printf("ll_node_init: passed in ll_node_t pointer points to non-NULL"
        " memory, possible risk of losing a handle to valid memory.\n"
        " API demand you to pass in a pointer initialized to NULL\n");
}

/* add a new node to the linked list pointed to by head with the given data. */
void ll_node_add(ll_node_t **head, void *data) {
    if (*head == NULL) {
        printf("ll_node_add: invalid head node, can't add a node\n");
        return;
    }
    /* assumption that head points to a valid head node. */
    ll_node_t *new_node = malloc(sizeof(ll_node_t));
    if (new_node == NULL) {
        printf("ll_node_add: failed to allocate a new node\n");
        return;
    }

    /* hook up the new node to the end of the list. */
    ll_node_t *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->next = NULL;
    new_node->data = data;
    printf("ll_node_add: new node succesfully added to end of list\n");
}

/* Since this API is generic, and data is a void *, we can't go about doing
    a recursive free into the data members, as to the LL API, it doesn't
    know the structure of the data. If the data member contains references
    further allocated data, the caller is responsible for calling that
    particular member's free API or manually freeing those members themselves.
    
   Actually, what we can do is pass in a destroy function, which allows the LL
   API to free the data member as it destroys the linked list. */
void ll_node_free(ll_node_t **head, void (*p_free)(void *data)) {
    /* Walk the linked list, freeing from head to tail? */
    if (*head == NULL) {
        printf("ll_node_free: invalid head node\n");
        return;
    }
    if (p_free == NULL) {
        printf("ll_node_free: null function pointer p_free\n");
        printf("assume that the data was allocated on the stack\n");
    }

    /* assume head node and p_free are valid. */
    ll_node_t *temp = *head;
    ll_node_t *prev = NULL;;
    while (temp != NULL) {
        if (p_free != NULL) {
            p_free(temp->data);
        }
        temp->data = NULL;  // sets the data pointer on the heap to 0.
        prev = temp;        // save a reference to previous node
        temp = temp->next;  // advance to the next node
        free(prev);         // free the prev node.
    }

    /* we have freed the linked list pointed to by head. User's head pointer
        still points to the old location of the linked list head. We could set
        its value to NULL here. */
    *head = NULL;
    printf("ll_node_free: successfully freed linked list\n");
}

/* print the contents of the linked list to the terminal */
void ll_node_print(ll_node_t **head, void (*p_print)(void *data)) {
    if (*head == NULL) {
        printf("ll_node_print: invalid head node\n");
        return;
    }
    if (p_print == NULL) {
        printf("ll_node_print: invalid data print function\n");
        printf("will just print the number of nodes\n");
    }

    int index = 0;
    ll_node_t *temp = *head;
    while(temp != NULL) {
        printf("{\nindex %d,\n" /* print data */, index);
        if (p_print != NULL) {
            p_print(temp->data);
            printf("\n");
        }
        printf("},\n\n");
        temp = temp->next;
        index++;
    }
}

ll_node_t * ll_node_getByIndex(ll_node_t **head, int index) {
    if (*head == NULL) {
        printf("ll_node_getByIndex: invalid head node\n");
        return NULL;
    }

    if (index < 0) {
        printf("ll_node_getByIndex: negative index err\n");
        return NULL;
    }

    ll_node_t *temp = *head;
    while (temp != NULL && index-- > 0) {
        temp = temp->next;
    }

    if(temp == NULL) {
        printf("ll_node_getByIndex: invalid index, maybe exceeded bounds\n");
        return NULL;
    }

    return temp;
}

/* print an integer, yeah no sh*t sherlock */
void print_int(void *data) {
    int *i_data = data;
    printf("%d", *i_data);
}

/* Test the Generic Linked List implementation: ensure 100% code coverage. */
void test() {
    /* INIT NODE TEST */
    ll_node_t *head = (void *) 0x12345678;
    int data0 = 30;

    ll_node_init(&head, &data0); // should print non-null err message.

    head = NULL;
    ll_node_init(&head, &data0); // should succeed
   
    int *datap = head->data;
    printf("Address of new node: %p\n"
           "Data of head node: %d\n", (void *) head, *datap);

    /* ADD NODE TEST */  
    int data1 = 40;
    ll_node_t *null_head = NULL;
    ll_node_add(&null_head, &data1); // should print invalid head node

    ll_node_add(&head, &data1);  // should add the node succesfully
    datap = head->next->data;
    printf("Address of added node: %p\n"
            "Data of added node: %d\n", (void *) head->next, *datap);

    printf("size of pointer: %ld\n", sizeof(void *));
    printf("size of ll_node_t: %ld\n", sizeof(ll_node_t));

    printf("\nNotice that the next node is located 0x20 (32) bytes apart from\n"
        "the head node, even though the sizeof an ll_node_t is 16 bytes.\n"
        "The reason for the extra 16 byte offset is that the malloc\n"
        "implementation maintains some header metadata about the allocated\n"
        "memory, right before the actual allocated user data.\n"
        "Looking at glibc's documentation for malloc internals, it maintains\n"
        "16 bytes of header data right before the payload, which adds up to\n"
        "our observation of 32 bytes offset between adjacent ll_nodes on\n"
        "the heap.\n\n");

    /* PRINT LL TEST */
    ll_node_print(&null_head, print_int); // should err w/ invalid head
    ll_node_print(&head, NULL); // should just print the nodes w/o data
    ll_node_print(&head, print_int);    // should print the int data

    /* GET LL_NODE TEST */
    ll_node_t *res_node;
    res_node = ll_node_getByIndex(&null_head, 0);  // should err w/ invalid head
    if (res_node == NULL) {
        printf("couldn't find node at index %d\n", 0);
    }
    res_node = ll_node_getByIndex(&head, -2);  // should err w/ negative index
    if (res_node == NULL) {
        printf("couldn't find node at index %d\n", -2);
    }
    res_node = ll_node_getByIndex(&head, 1);   

    datap = res_node->data;
    printf("received node's address: %p\n"
            "received node's data: %d\n", (void *) res_node, *datap);

    /* FREE NODE TEST */
    ll_node_free(&null_head, NULL); // should err
    ll_node_free(&head, NULL);  // should destroy the linked list.
}

int main() {
    test();

    printf("Hello World!\n");
    return 0;
}
