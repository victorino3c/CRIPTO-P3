# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
LDFLAGS = -lgmp -lm
U = utiles/
O = obj/
PR = primos/
PO = potenciacion/
D = data/
V = rsa/

# Rules
all: $(PR)prime_generator $(PO)potenciacion $(V)vegas

###############################################################################
#COMANDOS                                                                     #
###############################################################################

run_vegas_script: $(V)vegas
	bash $(V)vegas.sh

run_vegas_graphic: $(V)vegas
	python3.11 $(V)graphic_vegas.py

run_vegas: $(V)vegas
	./$(V)vegas -s 8192 -o $(D)output.txt

run_primo_script: $(PR)primo
	bash $(PR)primo.sh

run_primo_graphic: $(PR)primo
	python3.11 $(PR)graphic_primo.py

run_primo: $(PR)prime_generator
	./$(PR)prime_generator -b 1024 -p 0.999999 -i 1 -o $(D)output.txt	

run_potenciacion_test: $(PO)potenciacion
	./$(PO)potenciacion test

run_potenciacion_get: $(PO)potenciacion
	./$(PO)potenciacion get 224363654434574456 376453453466745325 26

###############################################################################
#EJECUTABLES                                                                  #
###############################################################################
$(V)vegas: $(O)vegas.o $(O)rsa.o $(O)primo.o $(O)utils.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(O)vegas.o: $(V)vegas.c $(V)rsa.h $(PR)primo.h $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(PR)prime_generator: $(O)prime_generator.o $(O)primo.o $(O)utils.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(O)prime_generator.o: $(PR)prime_generator.c $(PR)primo.h $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(PO)potenciacion: $(O)potenciacion.o $(O)utils.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(O)potenciacion.o: $(PO)potenciacion.c $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(O)rsa.o: $(V)rsa.c $(PR)primo.h $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(O)primo.o: $(PR)primo.c $(PR)primo.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

$(O)utils.o: $(U)utils.c $(U)utils.h
	mkdir -p $(O)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(O)*.o $(PR)prime_generator $(PO)potenciacion  $(V)vegas
	
clean_data:
	rm -f $(D)output.txt $(D)grafico_comparacion.png

clean_all: clean clean_data
