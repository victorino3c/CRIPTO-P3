#include "../utiles/utils.h"
#include "../primos/primo.h"

int main(int argc, char *argv[]) {
    mpz_t prime;

    mpz_init(prime);

    srand(time(NULL));

    generate_prime_number(1024, 15, &prime);

    gmp_printf("Prime number Candidate: %Zd\n", prime);

    mpz_clear(prime);
}