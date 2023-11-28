import argparse

def parse_arguments():
    parser = argparse.ArgumentParser(description='Vigenere Cipher Decryption')
    parser.add_argument('-i', '--input', type=str, required=True, help='Input file path')
    parser.add_argument('-o', '--output', type=str, required=True, help='Output file path')
    parser.add_argument('-s', '--seed', type=str, required=True, help='Seed string')
    parser.add_argument("-p", "--processes", type=int, default=1, help="Number of processes to spawn (default: 1)")
    return parser.parse_args()

def read_input_file(file_path):
    with open(file_path, 'r') as file:
        return file.read().strip()

def decrypt_letter(letter, rotationValue):
  rotationString  = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ "
  currentPosition = rotationString.find(letter)

  return rotationString[(currentPosition + rotationValue) % 95]

def generate_matrix(input_str, seed_str):
    L = len(input_str)
    matrix = [['' for _ in range(L)] for _ in range(L)]

    seed_index = 0
    for i in range(L):
        for j in range(L):
            matrix[i][j] = seed_str[seed_index]
            seed_index = (seed_index + 1) % len(seed_str)

    return matrix

def process_matrix(matrix):
    rows, cols = len(matrix), len(matrix[0])
    new_matrix = [['' for _ in range(cols)] for _ in range(rows)]

    for i in range(rows):
        for j in range(cols):
            neighbors_sum = sum([0 if cell == 'a' else 1 if cell == 'b' else 2 for cell in get_neighbors(matrix, i, j)])

            if matrix[i][j] == 'a':
                new_matrix[i][j] = 'a' if is_prime(neighbors_sum) else 'b' if neighbors_sum % 2 == 0 else 'c'
            elif matrix[i][j] == 'b':
                new_matrix[i][j] = 'b' if is_prime(neighbors_sum) else 'c' if neighbors_sum % 2 == 0 else 'a'
            elif matrix[i][j] == 'c':
                new_matrix[i][j] = 'c' if is_prime(neighbors_sum) else 'a' if neighbors_sum % 2 == 0 else 'b'

    return new_matrix

def get_neighbors(matrix, i, j):
    neighbors = []
    rows, cols = len(matrix), len(matrix[0])

    for ni in range(i - 1, i + 2):
        for nj in range(j - 1, j + 2):
            if 0 <= ni < rows and 0 <= nj < cols and (ni != i or nj != j):
                neighbors.append(matrix[ni][nj])

    return neighbors

def is_prime(num):
    if num < 2:
        return False
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

def update_cell(cell, neighbors_sum):
    if cell == 'a':
        return 'a' if is_prime(neighbors_sum) else 'b' if neighbors_sum % 2 == 0 else 'c'
    elif cell == 'b':
        return 'b' if is_prime(neighbors_sum) else 'c' if neighbors_sum % 2 == 0 else 'a'
    elif cell == 'c':
        return 'c' if is_prime(neighbors_sum) else 'a' if neighbors_sum % 2 == 0 else 'b'

def decrypt_string(matrix, encrypted_str):
    decrypted_str = ''
    for j in range(len(matrix[0])):
        column_sum = sum([0 if cell == 'a' else 1 if cell == 'b' else 2 for cell in [row[j] for row in matrix]])
        decrypted_char = decrypt_letter(encrypted_str[j], column_sum)
        decrypted_str += decrypted_char

    print("Column Sums:", [sum([0 if cell == 'a' else 1 if cell == 'b' else 2 for cell in [row[j] for row in matrix]]) for j in range(len(matrix[0]))])
    print("Decrypted String (Before joining):", [decrypt_letter(encrypted_str[j], column_sum) for j, column_sum in enumerate([sum([0 if cell == 'a' else 1 if cell == 'b' else 2 for cell in [row[j] for row in matrix]]) for j in range(len(matrix[0]))])])
    print("Decrypted String (After joining):", decrypted_str)

    return decrypted_str

def write_output_file(output_str, file_path):
    with open(file_path, 'w') as file:
        file.write(output_str)

def main():
    args = parse_arguments()
    input_str = read_input_file(args.input)
    matrix = generate_matrix(input_str, args.seed)
    print("Initial Matrix:")
    print_matrix(matrix)
    
    process_matrix(matrix)
    print("Processed Matrix:")
    print_matrix(matrix)
    
    decrypted_str = decrypt_string(matrix, input_str)
    print("Decrypted String:", decrypted_str)
    
    write_output_file(decrypted_str, args.output)

def print_matrix(matrix):
    for row in matrix:
        print(row)

if __name__ == "__main__":
    main()
