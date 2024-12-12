/**
 * @file rsa.c
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief This files contains the implementation of the functions defined in rsa.h
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "rsa.h"


void generate_euler_f(mpz_t p, mpz_t q, mpz_t euler_f){
    
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    
    mpz_mul(euler_f, p, q);
    
    mpz_add_ui(p, p, 1);
    mpz_add_ui(q, q, 1);
}

void generate_e(mpz_t euler_f, mpz_t e) {

    mpz_t *array;
    int size;
    int loop = 0;

    while(loop == 0) {
        /* We use the testigue function from primo.h 
        to get a random number for the exponent e */
        generate_testigue(e, euler_f);

        /* Check if e is coprime with euler_f */
        array = euclides(e, euler_f, &size);
        if(mpz_cmp_ui(array[size-1], 1) == 0) {
            loop = 1;
            for(int i=0; i<size; i++) {
                mpz_clear(array[i]);
            }
            free(array);
        }
    }
    
}

int generate_d(mpz_t e, mpz_t euler_f, mpz_t d){
    
    extended_euclides_inverse(e, euler_f, d);

    return 0;
}
