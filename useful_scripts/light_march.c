/*
 * A script to create memory trace files for memory testing
 * using march algorithms.
 * Input 1: Name of the output trace file
 * Input 2: March algorithm that will be followed
 * Input 3: N, for memory size 2^N
 *          N = 10 means 1Kb
 *            = 20 means 1Mb
 *            = 30 means 1Gb
 * Input format of march algorithm:
 *      -> Steps separated by commas with no spaces
 *      -> ↑ is u or U, ↓ is d or D, ↕ is a or A
 *      -> Example: aw,urw,dr
 *          + Creates a memory trace file that follows {↕(r);↑(wr);↓(r)}
 *      -> Both ↕ and ↑ follow memory addresses from bottom to top
 * Running example: ./light_march mats.trace aw,arw,ar
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int MEM_SIZE = 1048576; // 1Mb

/* 
 * Description: Makes all chars in string  uppercase
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
 * Description: Print sequence from march algorithm into trace file
 */
void print_sequence(FILE* fp, char* sequence) {
        int MAX_MEM = MEM_SIZE - 1;
        int num_sequences = strlen(sequence);
        int num_commands  = num_sequences - 1;
        char direction = sequence[0];
        switch (direction) {
                case 'A':
                case 'U':
                        for (int i = 0; i < MEM_SIZE; i++) {
                                for (int j = 1; j <= num_commands; j++) {
                                        fprintf(fp, "%X %c\n", i, sequence[j]);
                                }
                        }
                        break;
                case 'D':
                        for (int i = MAX_MEM; i >= 0; i--) {
                                for (int j = 1; j <= num_commands; j++) {
                                        fprintf(fp, "%X %c\n", i, sequence[j]);
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
        if (argc != 3 & argc != 4) {
                printf("Use: light_march FILE_NAME MARCH_ALGORITHM N (for memory size 2^N, optional)\n");
                exit(0);
        }

        if (argc == 4) {
                MEM_SIZE = pow(2, atoi(argv[3]));
        }
        
        char*  algorithm = argv[2];
        strToUpper(algorithm);
        int    seq_count = count_sequences(algorithm);
        char** sequences = tokenize_algorithm(algorithm);

        FILE* fp = fopen(argv[1], "w");
        
        for (int i = 0; i < seq_count; i++) {
                printf("Sequence %d: %s\n", i, sequences[i]);
                print_sequence(fp, sequences[i]);
        }

        fclose(fp);
        return 0;
}




