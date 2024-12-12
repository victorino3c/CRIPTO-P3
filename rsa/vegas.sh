# File that will execute the prime number generator program with differents parameters and 
# will store the results in a file called "primos.txt" in the data folder.

# Create the data folder if it doesn't exist
mkdir -p data

# Table with the sizes of the prime numbers to generate
initial_size=64
final_size=4096
number_muestras=50
step_size=$((($final_size - $initial_size)/$number_muestras))

iterations=20

file="data/vegas.txt"
output="data/output.txt"

# Remove the file if it already exists
rm -f $file

# Generate the prime numbers and store the results in the file
for ((i=$initial_size; i<=$final_size; i+=$step_size))
do
    echo "Generating prime numbers with size $i"
    total_time=0
    for ((j=0; j<$iterations; j++))
    do
        ./rsa/vegas -s $i -o $output
        # Last line of output will be: "Time: X", take that time and store it in the file
        new_time=$(tail -n 1 $output | cut -d ' ' -f 2)
        total_time=$(echo "$total_time + $new_time" | bc)
    done
    avg_time=$(echo "scale=6; $total_time / $iterations" | bc)
    echo "$i $avg_time" >> $file
done