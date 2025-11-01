/**
 * @file dictionary.h
 * @brief Header file for dictionary operations
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "hashtable.h"

LIST* read_dictionary(const char *filename, int *word_length);
void print_dictionary(LIST *dict);

#endif