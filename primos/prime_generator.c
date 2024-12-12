/**
 * @file prime_generator.c
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief This file generates a random (pseudo) prime number of a given size using primo.h bib
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "primo.h"

/**
 * @brief Check the arguments of the program
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @param size size of the prime number
 * @param prob probability of the number being prime
 * @param iterations number of primes to generate
 * @param file_out output file to print results
 * @return int 0 if the arguments are correct, -1 otherwise
 */
int check_args(int argc, char *argv[], int *size, double *prob, int *iterations, char **file_out);

/**
 * @brief Print the help of the program
 * 
 */
void print_help();

int main(int argc, char *argv[])
{

    int size;
    mpz_t prime;
    double prob, acumulative_time=0;
    int iterations = 0;
    char *file_out = NULL;

    mpz_init(prime);

    srand(time(NULL));

    if (check_args(argc, argv, &size, &prob, &iterations, &file_out) == -1){
        printf("Error in the arguments\n");
        return -1;
    }

    if(file_out != NULL) {
        freopen(file_out, "w", stdout);
    }

    int rounds = 0;
    rounds = calculate_rounds(size, prob);

    for(int i=0; i<iterations; i++) {
        clock_t start = clock();

        generate_prime_number(size, rounds, &prime);

        clock_t end = clock();

        gmp_printf("Prime number Candidate: %Zd\nResult of our test: Is prime.\nResult of GMP test: ", prime);

        /* Check if prime is prime with gmp function */
        if(mpz_probab_prime_p(prime, 25) == 0) {
            printf("Not prime.\n");

        } else {
            printf("Is prime.\n");
        }

        printf("Theorical probability of being prime: %lf (Number of rounds: %d)\n", prob, rounds);

        printf("Time: %lf\n\n", (double)(end - start) / CLOCKS_PER_SEC);

        acumulative_time += (double)(end - start) / CLOCKS_PER_SEC;

    }

    printf("Average time: %lf\n", acumulative_time/iterations);

    mpz_clear(prime);

    return 0;
}

int check_args(int argc, char *argv[], int *size, double *prob, int *iterations, char **file_out)
{
    if (argc != 7 && argc != 9){
        print_help();
        return -1;
    }

    if (strcmp(argv[1], "-b") == 0)
    {
        *size = atoi(argv[2]);
        if(*size < 0) {
            printf("Size must be greater than 0\n");
            print_help();
            return -1;
        }
    }
    else
    {
        return -1;
    }

    if (strcmp(argv[3], "-p") == 0)
    {
        *prob = strtod(argv[4], NULL);
        if(*prob < 0 || *prob > 1) {
            printf("Probability must be between 0 and 1\n");
            print_help();
            return -1;
        }
    }
    else
    {
        return -1;
    }

    if (strcmp(argv[5], "-i") == 0)
    {
        *iterations = atoi(argv[6]);
        if(*iterations < 0) {
            printf("Iterations must be greater than 0\n");
            print_help();
            return -1;
        }
    }
    else
    {
        print_help();
        return -1;
    }

    if(argc == 9) {
        if (strcmp(argv[7], "-o") == 0)
        {
            *file_out = argv[8];
        }
        else
        {
            print_help();
            return -1;
        }
    }

    return 0;
}

void print_help() {
    printf("Usage: primo -b <size> -p <probability> -i <iterations> [-o <file_name>]\n");
}
