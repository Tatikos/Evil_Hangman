/**
 * @file game.c
 * @brief Implementation of game logic
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

void print_welcome(void) {
    printf("**************************************************\n");
    printf("***             Welcome to Hangman             ***\n");
    printf("*** You are going to play against the computer ***\n");
    printf("**************************************************\n");
}

int get_number_of_guesses(void) {
    int num_guesses;
    while (1) {
        printf("How many total guesses? ");
        if (scanf("%d", &num_guesses) == 1 && num_guesses > 0) {
            while (getchar() != '\n');
            return num_guesses;
        }
        printf("Wrong input!\n");
        while (getchar() != '\n');
    }
}

GAME_STATE* init_game(LIST *dictionary, int word_length, int num_guesses) {
    GAME_STATE *game = (GAME_STATE*)malloc(sizeof(GAME_STATE));
    if (game == NULL) return NULL;

    game->current_words = copy_list(dictionary);
    if (game->current_words == NULL) {
        free(game);
        return NULL;
    }

    game->current_pattern = (char*)malloc((word_length + 1) * sizeof(char));
    if (game->current_pattern == NULL) {
        free_list(game->current_words);
        free(game);
        return NULL;
    }

    for (int i = 0; i < word_length; i++) {
        game->current_pattern[i] = '_';
    }
    game->current_pattern[word_length] = '\0';

    game->num_guessed = 0;
    game->word_length = word_length;
    game->guesses_remaining = num_guesses;

    return game;
}

int has_been_guessed(GAME_STATE *game, char letter) {
    for (int i = 0; i < game->num_guessed; i++) {
        if (game->guessed_letters[i] == letter) {
            return 1;
        }
    }
    return 0;
}

char get_guess(GAME_STATE *game) {
    char input[256];
    char letter;

    while (1) {
        printf("Guess a letter: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            continue;
        }

        input[strcspn(input, "\n")] = 0;

        if (strlen(input) != 1) {
            printf("Wrong input!\n");
            continue;
        }

        letter = tolower(input[0]);

        if (letter < 'a' || letter > 'z') {
            printf("Wrong input!\n");
            continue;
        }

        if (has_been_guessed(game, letter)) {
            printf("Letter %c was given before\n", letter);
            printf("-------------------------------\n");
            printf("Words possible: %d\n", game->current_words->size);
            printf("Progress: %s\n", game->current_pattern);
            printf("Number of Guesses: %d\n", game->guesses_remaining);
            continue;
        }

        return letter;
    }
}

/**
 * @brief Generate pattern for a word based on current pattern and guess
 * @param word The word to generate pattern for
 * @param current_pattern Current revealed pattern
 * @param guess The guessed letter
 * @param word_length Length of the word
 * @return Newly allocated pattern string (must be freed by caller)
 */
char* get_word_pattern(const char *word, const char *current_pattern, char guess, int word_length) {
    char *pattern = (char*)malloc((word_length + 1) * sizeof(char));
    if (pattern == NULL) return NULL;
    
    for (int i = 0; i < word_length; i++) {
        if (current_pattern[i] != '_') {
            // Keep already revealed letters
            pattern[i] = current_pattern[i];
        } else if (word[i] == guess) {
            // Reveal the guessed letter
            pattern[i] = guess;
        } else {
            // Keep hidden
            pattern[i] = '_';
        }
    }
    pattern[word_length] = '\0';
    return pattern;
}

/**
 * @brief Convert pattern to hash index
 * @param pattern The pattern string
 * @param word_length Length of the pattern
 * @return Hash index
 */
int pattern_to_hash(const char *pattern, int word_length) {
    int hash = 0;
    for (int i = 0; i < word_length; i++) {
        if (pattern[i] != '_') {
            hash |= (1 << i);
        }
    }
    return hash;
}

int is_word_complete(const char *pattern) {
    for (int i = 0; pattern[i]; i++) {
        if (pattern[i] == '_') {
            return 0;
        }
    }
    return 1;
}

void play_game(GAME_STATE *game) {
    while (game->guesses_remaining > 0 && !is_word_complete(game->current_pattern)) {
        printf("-------------------------------\n");
        printf("Progress: %s\n", game->current_pattern);
        printf("Number of Guesses: %d\n", game->guesses_remaining);

        char guess = get_guess(game);
        game->guessed_letters[game->num_guessed++] = guess;

        // Create hashtable for word families
        int num_slots = 1 << game->word_length;
        HASHTABLE *families = create_hashtable(num_slots);

        // Partition words into families based on pattern
        NODE *current = game->current_words->head;
        while (current != NULL) {
            char *temp_pattern = get_word_pattern(current->name, game->current_pattern, 
                                                   guess, game->word_length);
            if (temp_pattern != NULL) {
                int hash = pattern_to_hash(temp_pattern, game->word_length);
                if (hash >= 0 && hash < num_slots) {
                    insert_list(families->slot[hash], current->name);
                }
                free(temp_pattern);
            }
            current = current->next;
        }

        // Find largest family
        int largest_index;
        find_largest_family(families, &largest_index);

        // Get the pattern from largest family
        char new_pattern[game->word_length + 1];
        strcpy(new_pattern, game->current_pattern);
        
        if (families->slot[largest_index]->head != NULL) {
            char *temp = get_word_pattern(families->slot[largest_index]->head->name, 
                                         game->current_pattern, guess, game->word_length);
            if (temp != NULL) {
                strcpy(new_pattern, temp);
                free(temp);
            }
        }

        // Check if letter was found
        int letter_found = strcmp(game->current_pattern, new_pattern) != 0;

        // Update game state with largest family
        free_list(game->current_words);
        game->current_words = copy_list(families->slot[largest_index]);
        strcpy(game->current_pattern, new_pattern);

        free_hashtable(families);

        // Print result
        if (letter_found) {
            printf("You guessed a letter correctly\n");
        } else {
            printf("%c not in secret word\n", guess);
            game->guesses_remaining--;
        }

        printf("-------------------------------\n");
        printf("Words possible: %d\n", game->current_words->size);
        printf("Progress: %s\n", game->current_pattern);
        printf("Number of Guesses: %d\n", game->guesses_remaining);
    }

    // Game over
    if (is_word_complete(game->current_pattern)) {
        printf("**************************************************\n");
        printf("Congratulations, you win the game\n");
        printf("The secret word is: %s\n", game->current_pattern);
        printf("**************************************************\n");
    } else {
        printf("**************************************************\n");
        printf("You run out of guesses. You lost\n");
        if (game->current_words->head != NULL) {
            printf("The secret word was: %s\n", game->current_words->head->name);
        }
        printf("**************************************************\n");
    }
}

void free_game(GAME_STATE *game) {
    if (game == NULL) return;
    free_list(game->current_words);
    free(game->current_pattern);
    free(game);
}

#ifdef GAME_MAIN
int main(void) {
    printf("Testing game module...\n");
    return 0;
}
#endif