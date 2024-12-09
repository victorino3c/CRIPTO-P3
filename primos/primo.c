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
void generate_prime_number(int size, mpf_t prob, mpz_t *prime);

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
int calculate_rounds(int size, mpf_t prob);

/**
 * @brief generates a random testigue a for the Miller-Rabin test
 * 
 * @param a testigue (return)
 * @param number number to test primality
 */
void generate_testigue(mpz_t a, mpz_t number);

/**
 * @brief Check the arguments of the program
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @param size size of the prime number
 * @param prob probability of the number being prime
 * @return int 0 if the arguments are correct, -1 otherwise
 */
int check_args(int argc, char *argv[], int *size, char **prob);


int main(int argc, char *argv[])
{

    int size=1024;
    mpf_t prob;
    mpz_t prime;
    char *p;

    mpz_init(prime);
    mpf_init2(prob, 256);

    srand(time(NULL));

    if (check_args(argc, argv, &size, &p)){
        printf("Error in the arguments\n");
        printf("Usage: %s -b <size> -p <probability>\n", argv[0]);
        return -1;
    }

    printf("Size: %d\n", size);
    printf("Probability: %s\n", p);

    clock_t start = clock();

    generate_prime_number(size, prob, &prime);

    clock_t end = clock();

    gmp_printf("Prime number: %Zd\nObtained in %lf seconds\n", prime, (double)(end - start) / CLOCKS_PER_SEC);

    /* Check if prime is prime with gmp function */
    if(mpz_probab_prime_p(prime, 25) == 0) {
        printf("No es primo\n");
    } else {
        printf("Es primo\n");
    }

    mpz_clear(prime);

    return 0;
}

void generate_prime_number(int size, mpf_t prob, mpz_t *prime)
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

    gmp_printf("Prime number: %Zd\n", number);

    int rounds = 0;
    rounds = calculate_rounds(size, prob);
    printf("Number of rounds: %d\n", rounds);

    while (!found)
    {
        printf("Testing number: ");
        gmp_printf("%Zd\n", number);

        if (test_miller_rabin(number, rounds) > 0)
        {
            found = 1;
        }
        else
        {
            printf("Not prime\n");
            mpz_add_ui(number, number, 2);
        }
    }

    free(str);

    mpz_init_set(*prime, number);
    mpz_clear(number);
}

int test_miller_rabin(mpz_t number, int rounds)
{

    /* Discompose number in 2^n*d + 1 */
    mpz_t d;
    mpz_t a;
    mpz_t aux;
    mpz_t two;

    mpz_init(d);
    mpz_init(a);
    mpz_init(aux);
    mpz_init(two);

    mpz_set_ui(two, 2);

    mpz_sub_ui(d, number, 1);

    int s = 0;
    while (mpz_even_p(d))
    {
        mpz_fdiv_q_2exp(d, d, 1);
        s++;
    }

    //gmp_printf("d: %Zd\n", d);
    //gmp_printf("s: %d\n", s);

    /* Test if number is prime */

    for(int i=0; i<rounds; i++) {
        //printf("Round %d\n", i);
        /* Generate testigue a */
        generate_testigue(a, number);
        //gmp_printf("a: %Zd\n", a);

        /* Test if a^d mod number == 1  o -1*/
        potencia_modular(aux, a, d, number);

        mpz_sub_ui(number, number, 1);
        if(mpz_cmp_ui(aux, 1) == 0 || mpz_cmp(aux, number) == 0) {
            mpz_add_ui(number, number, 1);
            continue;
        }
        mpz_add_ui(number, number, 1);

        for(int ii=0; ii<s; ii++) {
            mpz_mul_ui(d, d, 2);
            potencia_modular(aux, a, d, number);

            if(mpz_cmp_ui(aux, 1) == 0) {
                return -1;
            }

            mpz_sub_ui(number, number, 1);
            if(mpz_cmp(aux, number) == 0) {
                mpz_add_ui(number, number, 1);
                break;
            }
            mpz_add_ui(number, number, 1);

            if(ii == s-1) {
                return -1;
            }
        }
        
    }

    return 1;
}

int calculate_rounds(int size, mpf_t prob) {

    /*mpfr_t result;
    mpfr_t aux;
    mpfr_t probabbility;

    mpfr_init(result);
    mpfr_init(aux);
    mpfr_init(probabbility);

    mpfr_set(probabbility, prob, MPFR_RNDN);

    mpfr_set(result, prob, MPFR_RNDN);


    return (int) result_d;*/
    mpf_add_ui(prob, prob, 1);
    mpf_sub_ui(prob, prob, 1);

    return size*0 + 2;
}

void generate_testigue(mpz_t a, mpz_t number) {
    mpz_t aux;
    gmp_randstate_t state;
    gmp_randinit_default(state);

    gmp_randseed_ui(state, rand());

    mpz_init(aux);

    mpz_sub_ui(aux, number, 2);

    mpz_urandomm(a, state, aux);

    mpz_add_ui(a, a, 2);

    mpz_clear(aux);
    gmp_randclear(state);
}

int check_args(int argc, char *argv[], int *size, char **prob)
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
        *prob = argv[4];
    }
    else
    {
        return -1;
    }

    return 0;
}