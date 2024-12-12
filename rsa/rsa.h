/**
 * @file rsa.h
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief File that contains the functions to generate the RSA keys
 * @version 0.1
 * @date 2024-12-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef RSA_H
#define RSA_H

#include "../utiles/utils.h"
#include "../primos/primo.h"

/**
 * @brief Generate the euler function value
 * 
 * @param p prime number p
 * @param q prime number q
 * @param euler_f euler function value
 */
void generate_euler_f(mpz_t p, mpz_t q, mpz_t euler_f);

/**
 * @brief Generate the public exponent e. It will be a random number coprime with euler_f
 * 
 * @param euler_f euler function value
 * @param e (return) public exponent e
 */
void generate_e(mpz_t euler_f, mpz_t e);

/**
 * @brief Generate the private exponent d. It will be the modular inverse of e mod euler_f
 * 
 * @param e public exponent e
 * @param euler_f euler function value
 * @param d (return) private exponent d
 * 
 * @return int 0 if the function was executed correctly, -1 otherwise
 */
int generate_d(mpz_t e, mpz_t euler_f, mpz_t d);

#endif