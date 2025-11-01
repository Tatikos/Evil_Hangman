/**
 * @file game.h
 * @brief Header file for game logic
 */

#ifndef GAME_H
#define GAME_H

#include "hashtable.h"

typedef struct {
    LIST *current_words;
    char *current_pattern;
    char guessed_letters[26];
    int num_guessed;
    int word_length;
    int guesses_remaining;
} GAME_STATE;

void print_welcome(void);
int get_number_of_guesses(void);
GAME_STATE* init_game(LIST *dictionary, int word_length, int num_guesses);
void play_game(GAME_STATE *game);
void free_game(GAME_STATE *game);

#endif