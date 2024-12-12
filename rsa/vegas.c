/**
 * @file vegas.c
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief Programa that will simulates the attack of the RSA algorithm using the Vegas algorithm
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../utiles/utils.h"
#include "../primos/primo.h"
#include "rsa.h"

/**
 * @brief Function that will simulate the attack of the RSA algorithm using the Vegas algorithm
 *        Will manage to guess p and q knowing d.
 * 
 * @param d private exponent d
 * @param n p*q
 * @param mod euler function value
 * @param p (return) prime number p
 * @param q (return) prime number q
 */
void vegas_attack(mpz_t d, mpz_t n, mpz_t mod, mpz_t p, mpz_t q);

/**
 * @brief Function to check the arguments of the program
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @param size size of the prime number
 * @param string output file
 * @return int 0 if the arguments are correct, -1 otherwise
 */
int check_args(int argc, char *argv[], int *size, char **string);

/**
 * @brief Function to print the help of the program
 * 
 */
void print_help();

int main(int argc, char *argv[]) {
    
    mpz_t p, q, n, euler_f, e, d;
    int size;
    char *string = NULL;

    if(check_args(argc, argv, &size, &string) == -1) {
        printf("Error in the arguments\n");
        print_help();
        return -1;
    }

    if(string != NULL) {
        freopen(string, "w", stdout);
    }

    mpz_init(p); mpz_init(q); mpz_init(n); 
    mpz_init(euler_f); mpz_init(e); mpz_init(d);

    srand(time(NULL));

    /* Starts RSA procedure */
    /* Generate primes p and q */
    printf("Generating p...\n");
    generate_prime_number(size, 15, &p);
    printf("Generating q...\n");
    generate_prime_number(size-1, 15, &q);
    mpz_mul(n, p, q);
    /* Generates Euler function value*/
    generate_euler_f(p, q, euler_f);
    /* Generate e */
    generate_e(euler_f, e);
    /* Generate d */
    if( generate_d(e, euler_f, d) == -1) {
        printf("Error generating d\n");
        return -1;
    }

    /* Print all values */
    //gmp_printf("p: %Zd\nq: %Zd\nEuler function: %Zd\nPublic exponent: %Zd\nPrivate exponent: %Zd\n", p, q, euler_f, e, d);
    gmp_printf("p: %Zd\nq: %Zd\n", p, q);

    printf("Starting Vegas attack...\n");

    mpz_t guess_p, guess_q;
    mpz_init(guess_p);
    mpz_init(guess_q);

    clock_t start = clock();

    vegas_attack(d, n, euler_f, guess_p, guess_q);

    clock_t end = clock();

    gmp_printf("Guessed p: %Zd\nGuessed q: %Zd\n", guess_p, guess_q);

    if(mpz_cmp(p, guess_p) == 0 && mpz_cmp(q, guess_q) == 0) {
        printf("Attack successful!\n");
    } else if(mpz_cmp(p, guess_q) == 0 && mpz_cmp(q, guess_p) == 0) {
        printf("Attack successful!\n");
    } else {
        printf("Attack failed.\n");
    }

    printf("Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(euler_f);
    mpz_clear(e);
    mpz_clear(d);

    return 0;
}

void vegas_attack(mpz_t d, mpz_t n, mpz_t mod, mpz_t p, mpz_t q) {

    /* Calculate e (implicit data)*/
    mpz_t e;
    mpz_init(e);
    
    if( generate_d(d, mod, e) == -1) {
        printf("Error generating e\n");
        return;
    }

    mpz_t ed, n_1, two, m, aux, pre_aux, w;
    mpz_init(ed);
    mpz_init(n_1);
    mpz_init(m);
    mpz_init(aux);
    mpz_init(pre_aux);
    mpz_init(w);

    mpz_mul(ed, e, d);
    mpz_init_set_ui(two, 2);
    mpz_sub_ui(m, ed, 1);
    mpz_sub_ui(n_1, n, 1);

    int s = 0;
    while (mpz_even_p(m))
    {
        mpz_fdiv_q_2exp(m, m, 1);
        s++;
    }

    /* Test if number is prime */
    int found = 0;
    while(found==0) {

        generate_testigue(w, n); // Generate random testigue
        
        /* Test if a^m mod number == 1  or -1*/
        potencia_modular(aux, w, m, n);

        if(mpz_cmp_ui(aux, 1) == 0 || mpz_cmp(aux, n_1) == 0) {
            continue; // can't answer, continue with next round
        }

        /* For every 2^d*s */
        for(int ii=0; ii<s; ii++) {
            mpz_set(pre_aux, aux);
            mpz_mul_ui(m, m, 2);
            potencia_modular(aux, w, m, n);

            if(mpz_cmp_ui(aux, 1) == 0) {
                mpz_sub_ui(pre_aux, pre_aux, 1);
                euclides_mcd(pre_aux, n, p);
                found = 1;
                break; // Found p 
            }

            if(mpz_cmp(aux, n_1) == 0) {
                break; // don't answer, might be prime. Continue with next round
            }

            /* If last iteration, and algorithm didn't answer yet, is composite */
            if(ii == s-1) {
                mpz_sub_ui(pre_aux, pre_aux, 1);
                euclides_mcd(pre_aux, n, p);
                found = 1;
                break; // Found p
            }
        }
    }

    if(found == 0) {
        printf("Error, couldn't find p\n");
        return;
    }

    /* Verify the test worked mod%p = 0*/
    mpz_t mod_p;
    mpz_init(mod_p);
    mpz_mod(mod_p, n, p);

    if(mpz_cmp_ui(mod_p, 0) != 0) {
        printf("Error, the test didn't work\n");
        return;
    }

    mpz_div(q, n, p);

    return;
}

int check_args(int argc, char *argv[], int *size, char **string) {
    if (argc != 3 && argc != 5) {
        return -1;
    }

    if (strcmp(argv[1], "-s") == 0) {
        *size = atoi(argv[2]);
        if (*size < 0) {
            printf("Size must be greater than 0\n");
            return -1;
        }
    } else {
        return -1;
    }

    if (argc == 5) {
        if (strcmp(argv[3], "-o") == 0) {
            *string = argv[4];
        } else {
            return -1;
        }
    }

    return 0;
}

void print_help() {
    printf("Usage: ./vegas -s <size> [-o <output_file>]\n");
    printf("Options:\n");
    printf("  -s <size>          Size of the prime number\n");
    printf("  -o <output_file>   Output file\n");
}