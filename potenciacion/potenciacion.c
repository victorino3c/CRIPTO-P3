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

#define INITIAL_N 100
#define FINAL_N 4000
#define STEP 100

#define OUTPUT_FILE "data/output.txt"
#define GNUPLOT_SCRIPT "potenciacion/grafica.gnu"

/**
 * @brief Generates a random number with n bits
 * 
 * @param n number of bits of the number
 */
mpz_t *generate_nbit_number(int n);

/**
 * @brief Test function for potencia_modular, comparing it with mpz_powm and getting the time of execution
 */
void test_potencia_modular();

/**
 * @brief Generates a plot with the time of execution of potencia_modular and mpz_powm
 */
void generate_plot();

int main(int argc, char *argv[]) {
    if (argc != 5 && strcmp(argv[1], "test") != 0) {
        printf("Uso: %s mode base exponent module\n", argv[0]);
        return 1;
    }

    mpz_t base, exp, mod, result;

    if (strcmp(argv[1], "test") == 0) {
        test_potencia_modular();
        generate_plot();
        return 0;
    } else if (strcmp(argv[1], "get") == 0) {
        if (mpz_cmp_ui(base, 0) < 0 || mpz_cmp_ui(exp, 0) < 0 || mpz_cmp_ui(mod, 0) < 0) {
            printf("Los valores de base, exponente y modulo deben ser mayores a 0\n");
            return 1;
        }

        mpz_init_set_str(base, argv[2], 10);
        mpz_init_set_str(exp, argv[3], 10);
        mpz_init_set_str(mod, argv[4], 10);
        mpz_init(result);

        //Get initial time
        float start = clock();

        potencia_modular(result, base, exp, mod);

        //Get final time
        float finish = clock();

        gmp_printf("Resultado: %Zd\tTiempo: %lf\n", result, (finish - start) / 1000000);

        mpz_clear(base);
        mpz_clear(exp);
        mpz_clear(mod);
        mpz_clear(result);
    } else {
        printf("Modo no reconocido\n");
        return 1;
    }

    return 0;
}

mpz_t *generate_nbit_number(int n) {
    mpz_t *number = malloc(sizeof(mpz_t));  // Reservar espacio para el número

    mpz_init(*number);

    // Crear un estado para los números aleatorios
    gmp_randstate_t state;
    gmp_randinit_default(state);  // Inicializar con el generador por defecto

    // Generar una semilla más aleatoria combinando el tiempo actual con un valor aleatorio
    unsigned long seed = time(NULL) ^ rand();  // XOR con rand() para mezclar con más aleatoriedad
    gmp_randseed_ui(state, seed);

    // Generar el número aleatorio de n bits
    mpz_urandomb(*number, state, n);

    // Limpiar el estado del generador de números aleatorios
    gmp_randclear(state);

    return number;
}

void test_potencia_modular() {
    
    mpz_t base, exp, mod, result, result2;

    float start_n, start_mpz, finish_n, finish_mpz;

    FILE *output = fopen(OUTPUT_FILE, "w");

    mpz_init(base);
    mpz_init(exp);
    mpz_init(mod);
    mpz_init(result);
    mpz_init(result2);

    for(int i = INITIAL_N; i < FINAL_N; i += STEP) {

        mpz_set(base, *generate_nbit_number(i));
        mpz_set(exp, *generate_nbit_number(i));
        mpz_set(mod, *generate_nbit_number(i));

        start_n = clock();

        potencia_modular(result, base, exp, mod);

        finish_n = clock();

        start_mpz = clock();

        mpz_powm(result2, base, exp, mod);


        finish_mpz = clock();

        //gmp_printf("base: %Zd\nexp: %Zd\nmod: %Zd\nresult: %Zd\nresult2: %Zd\n", base, exp, mod, result, result2);

        if (mpz_cmp(result, result2) != 0) {
            printf("Error en la potenciacion modular\n");
            return;
        }

        fprintf(output, "%d %lf %lf\n", i, (finish_n - start_n) / 1000000, (finish_mpz - start_mpz) / 1000000);

    }

    fclose(output);

    mpz_clear(base);
    mpz_clear(exp);
    mpz_clear(mod);
    mpz_clear(result);
    mpz_clear(result2);

}

void generate_plot() {
    // Ejecuto el script de gnuplot contenido en GNUPLT_SCRIPT
    char *command = malloc(strlen(GNUPLOT_SCRIPT) + 10);
    sprintf(command, "gnuplot %s", GNUPLOT_SCRIPT);
    system(command);

    free(command);
    
}
