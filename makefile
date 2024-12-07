# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lgmp -lm
U = utiles/
O = obj/
PR = primos/
PO = potenciacion/

# Rules
all: $(PR)primo $(PO)potenciacion

###############################################################################
#COMANDOS                                                                     #
###############################################################################

run_primo: $(PR)primo
	./$(PR)primo

run_potenciacion_test: $(PO)potenciacion
	./$(PO)potenciacion test

run_potenciacion_get: $(PO)potenciacion
	./$(PO)potenciacion get 224363654434574456 376453453466745325 26

###############################################################################
#EJECUTABLES                                                                  #
###############################################################################
$(PR)primo: $(O)primo.o $(O)utils.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(O)primo.o: $(PR)primo.c $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(PO)potenciacion: $(O)potenciacion.o $(O)utils.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(O)potenciacion.o: $(PO)potenciacion.c $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(O)utils.o: $(U)utils.c $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(O)*.o $(PR)primo $(PO)potenciacion
