/**
 * @file as2.c
 * @brief Main program file for Evil Hangman game
 * @author Student Name
 * @date November 2025
 * 
 * This program implements an "evil" hangman game where the computer
 * tries to win by dynamically changing the secret word based on user guesses.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
#include "hashtable.h"
#include "game.h"

/**
 * @brief Main function - entry point of the program
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success, 1 on error
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dictionary_file>\n", argv[0]);
        return 1;
    }

    print_welcome();

    int word_length;
    LIST *dictionary = read_dictionary(argv[1], &word_length);
    if (dictionary == NULL) {
        fprintf(stderr, "Error: Could not read dictionary file\n");
        return 1;
    }

    int num_guesses = get_number_of_guesses();

    GAME_STATE *game = init_game(dictionary, word_length, num_guesses);
    if (game == NULL) {
        fprintf(stderr, "Error: Could not initialize game\n");
        free_list(dictionary);
        return 1;
    }

    play_game(game);
    free_game(game);

    return 0;
}