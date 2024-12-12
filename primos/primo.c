/**
 * @file primo.c
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief This file contains the implementation of the functions defined in primo.h
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "primo.h"

void generate_prime_number(int size, int rounds, mpz_t *prime)
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

    /* Loop until find the prime number */
    while (!found)
    {
        if(check_divisibility_first_primes(number) == 0) {
            mpz_add_ui(number, number, 2);
        }

        else if (test_miller_rabin(number, rounds) > 0)
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

    mpz_t d;
    mpz_t a;
    mpz_t aux;
    mpz_t number_minus_1;
    mpz_t two;

    mpz_init(d);
    mpz_init(a);
    mpz_init(aux);
    mpz_init(two);
    mpz_init(number_minus_1);

    /* Discompose number in 2^n*d + 1 */
    mpz_set(number_minus_1, number);
    mpz_sub_ui(number_minus_1, number_minus_1, 1);
    mpz_set_ui(two, 2);
    mpz_sub_ui(d, number, 1);

    int s = 0;
    while (mpz_even_p(d))
    {
        mpz_fdiv_q_2exp(d, d, 1);
        s++;
    }

    /* Test if number is prime */
    for(int i=0; i<rounds; i++) {
        generate_testigue(a, number); // Generate random testigue

        /* Test if a^d mod number == 1  or -1*/
        potencia_modular(aux, a, d, number);
        if(mpz_cmp_ui(aux, 1) == 0 || mpz_cmp(aux, number_minus_1) == 0) {
            continue; // might be prime, continue with next round
        }

        /* For every 2^d*s */
        for(int ii=0; ii<s; ii++) {
            mpz_mul_ui(d, d, 2);
            potencia_modular(aux, a, d, number);

            if(mpz_cmp_ui(aux, 1) == 0) {
                mpz_clear(d);
                mpz_clear(a);
                mpz_clear(aux);
                mpz_clear(two);
                mpz_clear(number_minus_1);
                return -1; // 100% composite
            }

            if(mpz_cmp(aux, number_minus_1) == 0) {
                break; // don't answer, might be prime. Continue with next round
            }

            /* If last iteration, and algorithm didn't answer yet, is composite */
            if(ii == s-1) {
                mpz_clear(d);
                mpz_clear(a);
                mpz_clear(aux);
                mpz_clear(two);
                mpz_clear(number_minus_1);
                return -1;
            }
        }
    }

    mpz_clear(d);
    mpz_clear(a);
    mpz_clear(aux);
    mpz_clear(two);
    mpz_clear(number_minus_1);
    return 1;
}

int calculate_rounds(int size, double prob) {

    /* The method is this weird cause we can calculate the probability 
    given the number of rounds, but not the other way around. 
    Works well with double precision (maximum 31 rounds) */
    
    double estimated_prob = 0;
    int rounds = 0;

    while(prob > estimated_prob) {
        rounds++;
        
        estimated_prob = 1 / (1 + pow(4, rounds) / size * log(2));
        estimated_prob = 1 - estimated_prob;
    }

    return rounds;

}

void generate_testigue(mpz_t a, mpz_t number) {
   
    /* Random a testigue method */
    
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

int check_divisibility_first_primes(mpz_t number){

    for(int i=0; i<2000; i++) {
        if(mpz_divisible_ui_p(number, primes_table[i])) {
            return 0;
        }
    }

    return -1;
}
