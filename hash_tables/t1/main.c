#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 10

/* TODO: Implement chained linked lists for hash table */

typedef struct person {
    char name[MAX_NAME];
    int age;
    //...add other stuff later, maybe
    struct person *next;
} person;

person *hash_table[TABLE_SIZE];

unsigned int hash(char *name) {
    int length = (int) strnlen(name, MAX_NAME);
    unsigned int hash_value = 0;
    for (int i = 0; i< length; i++) {
        hash_value += (unsigned int) name[i];
        hash_value = (hash_value * (unsigned int) name[i]) 
            % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
    // table is empty
}

void print_table() {
    printf("Start\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == NULL) {
            printf("\t%i\t---\n", i);
        } else {
            person *tmp = hash_table[i];
            while (tmp != NULL) {
                printf("\t%i\t%s", i, tmp->name);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("End\n");
}

bool hash_table_insert(person *p) {
    if (p == NULL) return false;
    int index = (int) hash(p->name);
    p->next = hash_table[index];
    hash_table[index] = p;
    return true;
}

// find a person in the table by their name
person *hash_table_lookup(char *name) {
    int index = (int) hash(name);
    person *tmp = hash_table[index];
    while (tmp != NULL) {
        if (strncmp(tmp->name, name, MAX_NAME) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return tmp;
}

person *hash_table_delete(char *name) {
    int index = (int) hash(name);
    person *tmp = hash_table[index];
    person *prev = tmp;
    while (tmp != NULL) {
        if (strncmp(tmp->name, name, MAX_NAME) == 0) {
            if (prev == tmp) { // found at head
                /* free the head (if we were using malloc)*/
                hash_table[index] = tmp->next;
            } else { // found in middle or end
                prev->next = tmp->next;
            }
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
    return tmp;
}

int main() {

    init_hash_table();
    print_table();

    person jacob    = {.name="Jacob", .age=256};
    person kate     = {.name="Kate", .age=27};
    person mpho     = {.name="Mpho", .age=14};
    person sarah    = {.name="Sarah", .age=256};
    person edna     = {.name="Edna", .age=256};
    person maren    = {.name="Maren", .age=27};
    person eliza    = {.name="Eliza", .age=14};
    person robert   = {.name="Robert", .age=27};
    person jane     = {.name="Jane", .age=14};

    hash_table_insert(&jacob);
    hash_table_insert(&kate);
    hash_table_insert(&mpho);
    hash_table_insert(&sarah);
    hash_table_insert(&edna); 
    hash_table_insert(&maren); 
    hash_table_insert(&eliza); 
    hash_table_insert(&robert); 
    hash_table_insert(&jane); 

    print_table();

    person *tmp = hash_table_lookup("Mpho");
    if (tmp == NULL) {
        printf("Not found!\n");
    } else {
        printf("Found %s.\n", tmp->name);
    }

    tmp = hash_table_lookup("George");
    if (tmp == NULL) {
        printf("Not found!\n");
    } else {
        printf("Found %s.\n", tmp->name);
    }
   
    hash_table_delete("Mpho");
    tmp = hash_table_lookup("Mpho");
    if (tmp == NULL) {
        printf("Not found!\n");
    } else {
        printf("Found %s.\n", tmp->name);
    }

    print_table();

    /*
    printf("Jacob => %u\n", hash("Jacob"));
    printf("Natalie => %u\n", hash("Natalie"));
    printf("Sara => %u\n", hash("Sara"));
    printf("Mpho => %u\n", hash("Mpho"));
    printf("Tebogo => %u\n", hash("Tebogo"));
    printf("Ron => %u\n", hash("Ron"));
    printf("Jane => %u\n", hash("Jane"));
    printf("Maren => %u\n", hash("Maren"));
    printf("Bill => %u\n", hash("Bill"));
    */

    return 0;
}
