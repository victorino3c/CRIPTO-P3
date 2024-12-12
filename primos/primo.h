/**
 * @file primo.h
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief Functions to generate prime numbers and test if a number is prime following the Miller-Rabin test
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../utiles/utils.h"

#ifndef PRIMO_H
#define PRIMO_H

/**
 * @brief Generate a prime number of a given size
 * 
 * @param size size of the prime number
 * @param rounds number of rounds for the Miller-Rabin test
 * @param prime pointer to the prime number
 */
void generate_prime_number(int size, int rounds, mpz_t *prime);

/**
 * @brief Test if a number is prime using the Miller-Rabin test
 * 
 * @param number number to test
 * @param rounds number of rounds
 * @return int 1 if the number is potentially prime, 0 otherwise
 */
int test_miller_rabin(mpz_t number, int rounds);

/**
 * @brief Calculate the number of rounds for the Miller-Rabin test for a given size and probability
 * 
 * @param size size of the prime number
 * @param prob theorical probability of the number being prime
 * @return int number of rounds
 */
int calculate_rounds(int size, double prob);

/**
 * @brief generates a random testigue a for the Miller-Rabin test. We opt for a random number between [2, number-1]
 * 
 * @param a testigue (return)
 * @param number number to test primality
 */
void generate_testigue(mpz_t a, mpz_t number);

/**
 * @brief Check if a number is divisible by the first 2000 prime numbers
 * 
 * @param number number to check
 * 
 * @return int 0 if the number is divisible by one of the first 2000 prime numbers, -1 otherwise
 */
int check_divisibility_first_primes(mpz_t number);

#endif
