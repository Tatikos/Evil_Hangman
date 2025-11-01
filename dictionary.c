/**
 * @file dictionary.c
 * @brief Implementation of dictionary operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

LIST* read_dictionary(const char *filename, int *word_length) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }

    if (fscanf(fp, "%d", word_length) != 1) {
        fprintf(stderr, "Error: Invalid file format\n");
        fclose(fp);
        return NULL;
    }

    LIST *dict = create_list();
    if (dict == NULL) {
        fclose(fp);
        return NULL;
    }

    char buffer[256];
    while (fscanf(fp, "%s", buffer) == 1) {
        for (int i = 0; buffer[i]; i++) {
            buffer[i] = tolower(buffer[i]);
        }
        
        if (insert_list(dict, buffer) != 0) {
            fprintf(stderr, "Error: Could not insert word\n");
            free_list(dict);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    return dict;
}

void print_dictionary(LIST *dict) {
    if (dict == NULL) {
        printf("Dictionary is empty\n");
        return;
    }

    printf("Dictionary (%d words):\n", dict->size);
    NODE *current = dict->head;
    while (current != NULL) {
        printf("%s\n", current->name);
        current = current->next;
    }
}

#ifdef DICTIONARY_MAIN
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dictionary_file>\n", argv[0]);
        return 1;
    }

    int word_length;
    LIST *dict = read_dictionary(argv[1], &word_length);
    if (dict == NULL) return 1;

    printf("Word length: %d\n", word_length);
    print_dictionary(dict);
    free_list(dict);

    return 0;
}
#endif