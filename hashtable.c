/**
 * @file hashtable.c
 * @brief Implementation of hashtable and list data structures
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

LIST* create_list(void) {
    LIST *list = (LIST*)malloc(sizeof(LIST));
    if (list == NULL) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

int insert_list(LIST *list, const char *word) {
    if (list == NULL || word == NULL) return -1;

    NODE *new_node = (NODE*)malloc(sizeof(NODE));
    if (new_node == NULL) return -1;

    new_node->name = (char*)malloc(strlen(word) + 1);
    if (new_node->name == NULL) {
        free(new_node);
        return -1;
    }

    strcpy(new_node->name, word);
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
    return 0;
}

void free_list(LIST *list) {
    if (list == NULL) return;
    NODE *current = list->head;
    while (current != NULL) {
        NODE *temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }
    free(list);
}

LIST* copy_list(LIST *list) {
    if (list == NULL) return NULL;

    LIST *new_list = create_list();
    if (new_list == NULL) return NULL;

    NODE **array = (NODE**)malloc(list->size * sizeof(NODE*));
    if (array == NULL) {
        free_list(new_list);
        return NULL;
    }

    NODE *current = list->head;
    int i = 0;
    while (current != NULL) {
        array[i++] = current;
        current = current->next;
    }

    for (i = list->size - 1; i >= 0; i--) {
        if (insert_list(new_list, array[i]->name) != 0) {
            free(array);
            free_list(new_list);
            return NULL;
        }
    }

    free(array);
    return new_list;
}

HASHTABLE* create_hashtable(int num_slots) {
    HASHTABLE *ht = (HASHTABLE*)malloc(sizeof(HASHTABLE));
    if (ht == NULL) return NULL;

    ht->num_slots = num_slots;
    ht->slot = (LIST**)calloc(num_slots, sizeof(LIST*));
    if (ht->slot == NULL) {
        free(ht);
        return NULL;
    }

    for (int i = 0; i < num_slots; i++) {
        ht->slot[i] = create_list();
        if (ht->slot[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free_list(ht->slot[j]);
            }
            free(ht->slot);
            free(ht);
            return NULL;
        }
    }

    return ht;
}

void free_hashtable(HASHTABLE *ht) {
    if (ht == NULL) return;
    if (ht->slot != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            free_list(ht->slot[i]);
        }
        free(ht->slot);
    }
    free(ht);
}

int insert_hashtable(HASHTABLE *ht, const char *word, const char *pattern, int word_length) {
    if (ht == NULL || word == NULL || pattern == NULL) return -1;

    int hash = 0;
    for (int i = 0; i < word_length; i++) {
        if (pattern[i] != '_') {
            hash |= (1 << i);
        }
    }

    if (hash < 0 || hash >= ht->num_slots) return -1;

    if (insert_list(ht->slot[hash], word) != 0) return -1;

    return hash;
}

int find_largest_family(HASHTABLE *ht, int *largest_index) {
    if (ht == NULL || largest_index == NULL) return 0;

    int max_size = ht->slot[0]->size;
    *largest_index = 0;

    for (int i = 1; i < ht->num_slots; i++) {
        if (ht->slot[i]->size > max_size) {
            max_size = ht->slot[i]->size;
            *largest_index = i;
        }
    }

    return max_size;
}

#ifdef HASHTABLE_MAIN
int main(void) {
    printf("Testing hashtable module...\n");
    
    LIST *list = create_list();
    insert_list(list, "test1");
    insert_list(list, "test2");
    printf("List size: %d\n", list->size);
    free_list(list);

    HASHTABLE *ht = create_hashtable(16);
    insert_hashtable(ht, "cool", "__ll", 4);
    insert_hashtable(ht, "good", "__ll", 4);
    int idx;
    int max = find_largest_family(ht, &idx);
    printf("Largest family size: %d at index %d\n", max, idx);
    free_hashtable(ht);

    return 0;
}
#endif