/**
 * @file primo.c
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief This file generates a random (pseudo) prime number of a given size
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../utiles/utils.h"

/**
 * @brief Generate a prime number of a given size
 * 
 * @param size size of the prime number
 * @param prob probability of the number being prime
 * @param prime pointer to the prime number
 */
void generate_prime_number(int size, double prob, mpz_t *prime);

/**
 * @brief Test if a number is prime using the Miller-Rabin test
 * 
 * @param number number to test
 * @param rounds number of rounds
 * @return int 1 if the number is prime, 0 otherwise
 */
int test_miller_rabin(mpz_t number, int rounds);

/**
 * @brief Calculate the number of rounds for the Miller-Rabin test
 * 
 * @param size size of the prime number
 * @param prob probability of the number being prime
 * @return int number of rounds
 */
int calculate_rounds(int size, double prob);

/**
 * @brief Check the arguments of the program
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @param size size of the prime number
 * @param prob probability of the number being prime
 * @return int 0 if the arguments are correct, -1 otherwise
 */
int check_args(int argc, char *argv[], int *size, double *prob);

int main(int argc, char *argv[])
{

    int size=1024;
    double prob=0.9;
    mpz_t prime;

    mpz_init(prime);

    srand(time(NULL));

    if (check_args(argc, argv, &size, &prob)){
        printf("Error in the arguments\n");
        printf("Usage: %s -b <size> -p <probability>\n", argv[0]);
        return -1;
    }

    generate_prime_number(size, prob, &prime);

    gmp_printf("Prime number: %Zd\n", prime);

    mpz_clear(prime);

    return 0;
}

void generate_prime_number(int size, double prob, mpz_t *prime)
{
    int found = 0;
    char *str;
    
    mpz_t number;

    mpz_init(number);

    /* Create random number */
    str = malloc((size+1) * sizeof(char));
    str[0] = '1';
    for (int i = 1; i < size-1; i++)
    {
        str[i] = random_num(0, 1) + '0';
    }
    str[size-1] = '1';
    str[size] = '\0';
    mpz_set_str(number, str, 2);

    int rounds = 0;
    rounds = calculate_rounds(size, prob);

    while (!found)
    {
        if (test_miller_rabin(number, rounds))
        {
            found = 1;
        }
        else
        {
            mpz_add_ui(number, number, 2);
        }
    }

    free(str);

    mpz_init_set(*prime, number);
    mpz_clear(number);
}

int test_miller_rabin(mpz_t number, int rounds)
{
    /* Algorithm */
    return 1;
}

int calculate_rounds(int size, double prob) {
    double p;

    return 16;
}

int check_args(int argc, char *argv[], int *size, double *prob)
{
    if (argc != 5)
        return -1;

    if (strcmp(argv[1], "-b") == 0)
    {
        *size = atoi(argv[2]);
    }
    else
    {
        return -1;
    }

    if (strcmp(argv[3], "-p") == 0)
    {
        *prob = atof(argv[4]);
    }
    else
    {
        return -1;
    }

    return 0;
}