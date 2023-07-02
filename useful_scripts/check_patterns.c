/*
 * A script to create type-2 neighborhood patterns for a
 * given memory March algorithm.
 * Input 1: Name of the output pattern file
 * Input 2: March algorithm that will be followed
 * Input format of march algorithm:
 *      -> Steps separated by commas with no spaces
 *      -> ↑ is u or U, ↓ is d or D, ↕ is a or A
 *      -> The letters z or Z for 0, any other letter of choice for 1.
 *      -> Example: awz,urzwi,dri
 *          + Creates a memory trace file that follows {↕(w0);↑(r0,w1);↓(r1)}
 *      -> Both ↕ and ↑ follow memory addresses from bottom to top
 * Running example: ./check_patterns mats_patterns awz,urzwi,dri
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* 
 * Description: Makes all chars in string uppercase
 */
void strToUpper(char *str) {
        int len = strlen(str);
        for (int i = 0; i < len; i++) {
            str[i] = toupper(str[i]);
        }
        return;
}

/* 
 * Description: Counts how many sequences there are in a given march algorithm
 */
int count_sequences(char* algorithm) {
        int len   = strlen(algorithm);
        int count = 1;
        for (int i = 0; i < len; i++) {
            if (algorithm[i] == ',') count++;
        }
        return count;
}

/* 
 * Description: Tokenize sequences in a given march algorithm
 */
char** tokenize_algorithm(char* algorithm) {
        int seq_count = count_sequences(algorithm);
        char** sequences = malloc(seq_count * sizeof(char*));
        char divider[] = {','};
        char* sequence = strtok(algorithm, divider);
        sequences[0] = sequence;
        for (int i = 1; i < seq_count; i++) {
                sequence = strtok(NULL, divider);
                sequences[i] = sequence;
        }
        return sequences;
}

/* 
 * Description: Checks if a mem_state is equal to a given piven pattern.
 */
bool are_same(char** mem_state, char** pattern) {
        if (!pattern) return false;
        for (int r = 0; r < 3; r++) {
                for (int c = 0; c < 3; c++) {
                        if (mem_state[r][c] != pattern[r][c]) return false;
                }
        }
        return true;
}

/* 
 * Description: Checks if the neighborhood pattern observed in mem_state can
 * be found in patterns. Add if it is a new pattern.
 */
void add_pattern(char** mem_state, char*** patterns) {
        for (int i = 0; i < 4608; i++) {
                if (are_same(mem_state, patterns[i])) return;
                if (patterns[i]) continue;
                patterns[i] = malloc(3 * sizeof(char**));
                for (int r = 0; r < 3; r++) {
                        patterns[i][r] = malloc(3 * sizeof(char*));
                        for (int c = 0; c < 3; c++) {
                                patterns[i][r][c] = mem_state[r][c];
                        }
                }
                return;
        }
        return;
}

/* 
 * Description: Description
 */
void update_state(char** mem_state, char*** patterns, int r, int c, int command, int new_val) {
        char initial_char = mem_state[r][c];
        if (command != 'W' || initial_char == new_val) return;
        mem_state[r][c] = (new_val == 'Z') ? ('D') : ('U');
        if (initial_char != 'i') add_pattern(mem_state, patterns);
        mem_state[r][c] = new_val;

        return;
}



/* 
 * Description: Given a March sequence and DRAM array state, add to patterns
 * array all the newly observed neighborhood patterns.
 */
void find_patterns(char** mem_state, char*** patterns, char* sequence) {
        int len_sequence = strlen(sequence);
        int len_commands = len_sequence - 1;
        char direction   = sequence[0];
        switch (direction) {
                case 'A':
                case 'U':
                        for (int r = 0; r < 3; r++) {
                                for (int c = 0; c < 3; c++) {
                                        for (int i = 0; i < len_commands; i++) {
                                                update_state(mem_state, patterns, r, c, sequence[i], sequence[i + 1]);
                                        }
                                }
                        }
                        break;
                case 'D':
                        for (int r = 2; r >= 0; r--) {
                                for (int c = 2; c >= 0; c--) {
                                        for (int i = 0; i < len_commands; i++) {
                                                update_state(mem_state, patterns, r, c, sequence[i], sequence[i + 1]);
                                        }
                                }
                        }
                        break;
                default:
                        printf("Inappropriate direction, exiting\n");
                        exit(0);
                        
        }
        return;
}

int main(int argc, char *argv[])
{
        if (argc != 3) {
                printf("Use: pattern_check FILE_NAME MARCH_ALGORITHM\n");
                exit(0);
        }

        char*  algorithm = argv[2];
        strToUpper(algorithm);
        
        FILE* fp = fopen(argv[1], "w");
        fprintf(fp, "Neighborhood patterns in %s:\n\n", argv[2]);
        
        int    seq_count = count_sequences(algorithm);
        char** sequences = tokenize_algorithm(algorithm);
        
        // Initialize memory state
        char** mem_state = malloc(3 * sizeof(char*));
        for (int i = 0; i < 3; i++) {
                mem_state[i] = malloc(3 * sizeof(char));
                for (int j = 0; j < 3; j++) {
                        mem_state[i][j] = 'i';
                }
        }
        // Initialize list of patterns
        char*** patterns = malloc(4608 * sizeof(char**));
        for (int i = 0; i < 4608; i++) {
                patterns[i] = NULL;
        }
        
        // Find patterns with each sequence
        for (int i = 0; i < seq_count; i++) {
                printf("Sequence %d: %s\n", i, sequences[i]);
                find_patterns(mem_state, patterns, sequences[i]);
        }

        // Save patterns in a text file
        int i = 0;
        while (patterns[i] != NULL) {
                i++;
        }
        fprintf(fp, "%d patterns found \n\n", i);
        i = 0;
        while (patterns[i] != NULL) {
                for (int r = 0; r < 3; r++) {
                        fprintf(fp, "%c %c %c\n", patterns[i][r][0], patterns[i][r][1], patterns[i][r][2]);
                }
                fprintf(fp, "\n");
                i++;
                //if (i > 5000) break;
        }
        fclose(fp);

        return 0;
}




