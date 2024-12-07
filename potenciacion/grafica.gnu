# Script de Gnuplot para graficar los datos
set title "Comparación de Tiempos Potenciacion modular"
set xlabel "Número de bits"
set ylabel "Tiempo (segundos)"
set grid

# Definir las leyendas de las curvas
set key left top

# Establecer el tipo de archivo de salida
set terminal pngcairo size 800,600
set output "data/grafico_comparacion.png"

# Graficar los datos
plot "data/output.txt" using 1:2 with lines title "Algoritmo propio", \
     "data/output.txt" using 1:3 with lines title "Algoritmo mpz powm"

# Cerrar el archivo de salida
set output
