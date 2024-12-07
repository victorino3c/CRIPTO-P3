/**
 * @file potenciacion.c
 * @author Nicolas Victorino && Ignacio Nunnez
 * @brief This file calculates the modular exponentiation of a number
 * @version 0.1
 * @date 2024-12-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "../utiles/utils.h"

/**
 * @brief Calculates the modular exponentiation of base^exp mod mod and stores the result in result
 * 
 * @param result result of the modular exponentiation
 * @param base base of the exponentiation
 * @param exp exponent of the exponentiation
 * @param mod modulus of the exponentiation
 */

void potencia_modular(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s base exponente modulo\n", argv[0]);
        return 1;
    }

    mpz_t base, exp, mod, result_n, result_mpz;
    mpz_init_set_str(base, argv[1], 10);
    mpz_init_set_str(exp, argv[2], 10);
    mpz_init_set_str(mod, argv[3], 10);
    mpz_init(result_n);
    mpz_init(result_mpz);

    // Get our initial time
    clock_t start_n = clock();

    potencia_modular(result_n, base, exp, mod);

    // Get our final time
    clock_t end_n = clock();

    // Get mpz_powm initial time
    clock_t start_mpz = clock();

    mpz_powm(result_mpz, base, exp, mod);

    // Get mpz_powm final time
    clock_t end_mpz = clock();

    // Calculate our time
    double time_n = (double)(end_n - start_n) / CLOCKS_PER_SEC;

    // Calculate mpz_powm time
    double time_mpz = (double)(end_mpz - start_mpz) / CLOCKS_PER_SEC;

    printf("Nuestro tiempo de ejecución: %f segundos\n", time_n);
    printf("Tiempo de ejecución de mpz_powm: %f segundos\n", time_mpz);

    gmp_printf("Nuestro resultado: %Zd\n", result_n);
    gmp_printf("Resultado de mpz_powm: %Zd\n", result_mpz);

    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);
    mpz_clear(result_n);
    mpz_clear(result_mpz);

    return 0;
}

void potencia_modular(mpz_t result, const mpz_t base, const mpz_t exp, const mpz_t mod) {
    mpz_t x;
    mpz_init_set_ui(x, 1);  // x = 1

    mpz_t base_mod;
    mpz_init_set(base_mod, base);  // base_mod = base

    mpz_t exp_copy;
    mpz_init_set(exp_copy, exp);  // exp_copy = exp

    while (mpz_cmp_ui(exp_copy, 0) > 0) {
        // Si el bit menos significativo de exp es 1
        if (mpz_odd_p(exp_copy)) {
            mpz_mul(x, x, base_mod);
            mpz_mod(x, x, mod);
        }
        // base_mod = (base_mod * base_mod) % mod
        mpz_mul(base_mod, base_mod, base_mod);
        mpz_mod(base_mod, base_mod, mod);

        // exp_copy = exp_copy / 2
        mpz_fdiv_q_2exp(exp_copy, exp_copy, 1);
    }

    mpz_set(result, x);

    mpz_clear(x);
    mpz_clear(base_mod);
    mpz_clear(exp_copy);
}