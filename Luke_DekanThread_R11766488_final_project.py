import argparse
import os
import multiprocessing

def argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=input, required=True,)
    parser.add_argument('-o', '--output', type=output, required=True,)
    parser.add_argument('-s', '--seed', type=seed, required=True,)
    parser.add_argument("-p", "--processes", type=processes, default=1,)

    args = parser.parse_args()
    return args

def input(file_path):
    if not os.path.isfile(file_path):
        print("Invalid File Input path")
        exit()
    else:
        return file_path

def seed(seed_str):
    valid_chars = {'a', 'b', 'c'}
    if not set(seed_str).issubset(valid_chars):
       print("Seed String Invalid")
       exit()
    else:
        return seed_str

def output(file_path):
    directory = os.path.dirname(file_path)
    if directory and not os.path.exists(directory):
       print("Invalide File Output Path")
       exit()
    else:
        return file_path

def processes(processes):
        processes = int(processes)
        if processes <= 0:
            print("Invalid Process Number Must be Integer")
            exit()
        else:    
         return processes

def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

def create_matrix(input_str, seed_str):

    L = len(input_str)
    matrix = [[0] * L for _ in range(L)]

    seed_index = 0
    for i in range(L):
        for j in range(L):
            if seed_str[seed_index] == 'a':
                matrix[i][j] = 0
            elif seed_str[seed_index] == 'b':
                matrix[i][j] = 1
            else:
                matrix[i][j] = 2

            seed_index = (seed_index + 1) % len(seed_str)

    return matrix

def calculate_matrix(matrix):

    L = len(matrix)
    matrix_copy = [[0] * L for _ in range(L)]

    for i in range(L):
        for j in range(L):
            
            neighbors_sum = sum(get_neighbors(matrix, i, j))

            if matrix[i][j] == 0: 
                matrix_copy[i][j] = 0 if prime_check(neighbors_sum) else 1 if neighbors_sum % 2 == 0 else 2
            elif matrix[i][j] == 1:  
                matrix_copy[i][j] = 1 if prime_check(neighbors_sum) else 2 if neighbors_sum % 2 == 0 else 0
            else:
                matrix_copy[i][j] = 2 if prime_check(neighbors_sum) else 0 if neighbors_sum % 2 == 0 else 1

    for i in range(L):
        for j in range(L):
            matrix[i][j] = matrix_copy[i][j]
    return matrix


def get_neighbors(matrix, i, j):
    neighbors = []
    rows, cols = len(matrix), len(matrix[0])

    for ni in range(i - 1, i + 2):
        for nj in range(j - 1, j + 2):
            if 0 <= ni < rows and 0 <= nj < cols and (ni != i or nj != j):
                neighbors.append(matrix[ni][nj])

    return neighbors

def prime_check(num):
    if num < 2:
        return False
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

def update_matrix_cell(cell, neighbors_sum):
    if cell == 0:  
        return 0 if prime_check(neighbors_sum) else 1 if neighbors_sum % 2 == 0 else 2
    elif cell == 1:  
        return 1 if prime_check(neighbors_sum) else 2 if neighbors_sum % 2 == 0 else 0
    else:  
        return 2 if prime_check(neighbors_sum) else 0 if neighbors_sum % 2 == 0 else 1
    
def decrypt_letter(letter, rotationValue):
  rotationString  = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ "
  currentPosition = rotationString.find(letter)
  return rotationString[(currentPosition + rotationValue) % 95]

def decrypt_string(matrix, encrypted_str):
    decrypted_str = ''
    for j in range(len(matrix[0])):
        column_sum = sum([matrix[i][j] for i in range(len(matrix))])

        decrypted_char = decrypt_letter(encrypted_str[j], column_sum)
        decrypted_str += decrypted_char

    return decrypted_str

def write_output_file(output_str, file_path):
    with open(file_path, 'w') as file:
        file.write(output_str)

def process_matrix_worker(matrix_data):
    matrix, iterations = matrix_data 
    for _ in range(100):
        calculate_matrix(matrix)
    return matrix

def run_multiprocessing(matrix, processes):

    if processes == 1:
        for _ in range(100):
            calculate_matrix(matrix)
        return matrix
    
    iterations_per_process = 100 // processes

 
    with multiprocessing.Pool(processes=processes) as pool:
        result_matrices = pool.map(process_matrix_worker, [(matrix.copy(), iterations_per_process) for _ in range(processes)])

    # Aggregate results
    final_matrix = matrix.copy()
    for result_matrix in result_matrices:
        for i in range(len(matrix)):
            for j in range(len(matrix[0])):
                final_matrix[i][j] = result_matrix[i][j]
    return final_matrix

def main():
    print("Project :: R11766388")
    args = argument_parser()
    input_str = read_file(args.input)
    matrix = create_matrix(input_str, args.seed)
  
    processes = args.processes

    run_multiprocessing(matrix, processes)
    
    decrypted_str = decrypt_string(matrix, input_str)
    print("Decrypted String:", decrypted_str)
    
    write_output_file(decrypted_str, args.output)

def print_matrix(matrix):
    for row in matrix:
        print(row)

if __name__ == "__main__":
    main()
