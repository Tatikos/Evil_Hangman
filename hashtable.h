/**
 * @file hashtable.h
 * @brief Header file for hashtable and list data structures
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct node {
    char *name;
    struct node *next;
} NODE;

typedef struct list {
    NODE *head;
    int size;
} LIST;

typedef struct {
    LIST **slot;
    int num_slots;
} HASHTABLE;

LIST* create_list(void);
int insert_list(LIST *list, const char *word);
void free_list(LIST *list);
LIST* copy_list(LIST *list);

HASHTABLE* create_hashtable(int num_slots);
void free_hashtable(HASHTABLE *ht);
int insert_hashtable(HASHTABLE *ht, const char *word, const char *pattern, int word_length);
int find_largest_family(HASHTABLE *ht, int *largest_index);

#endif