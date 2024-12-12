# File that will execute the prime number generator program with differents parameters and 
# will store the results in a file called "primos.txt" in the data folder.

# Create the data folder if it doesn't exist
mkdir -p data

# Table with the sizes of the prime numbers to generate
initial_size=8
final_size=2048
number_muestras=20
step_size=$((($final_size - $initial_size)/$number_muestras))

iterations=1
probabilidad=0.999

file="data/primos.txt"
output="data/output.txt"

# Remove the file if it already exists
rm -f $file

# Generate the prime numbers and store the results in the file
for ((i=$initial_size; i<=$final_size; i+=$step_size))
do
    echo "Generating prime numbers with size $i"
    ./primos/primo -b $i -p $probabilidad -i $iterations -o $output

    # Last line of output will be: "Average time: X", take that time and store it in the file
    time=$(tail -n 1 $output | cut -d ' ' -f 3)
    echo "$i $time" >> $file
done