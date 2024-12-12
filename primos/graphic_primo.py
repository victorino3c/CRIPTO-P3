import matplotlib.pyplot as plt

# Create graphic of average time to generate and size of primes
def graphic_primo(file):
    with open(file) as f:
        lines = f.readlines()
        x = []
        y = []
        for line in lines:
            data = line.split()
            x.append(int(data[0]))
            y.append(float(data[1]))
    plt.plot(x, y)
    plt.xlabel('Primes size (bits)')
    plt.ylabel('Time (s)')
    plt.title('Average time to generate primes')
    
    # Create image of the graphic
    plt.savefig('data/primos.png')

# Create graphic of average time to generate and size of primes
graphic_primo('data/primos.txt')
