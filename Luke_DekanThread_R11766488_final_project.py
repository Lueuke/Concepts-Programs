import argparse
import os
import multiprocessing
import math 

# Function to Take command Arguments 
def argument_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=input, required=True,)
    parser.add_argument('-o', '--output', type=output, required=True,)
    parser.add_argument('-s', '--seed', type=seed, required=True,)
    parser.add_argument("-p", "--processes", type=processes, default=1,)

    args = parser.parse_args()
    return args

# Validate Input Path 
def input(file_path):
    if not os.path.isfile(file_path):
        print("Invalid File Input path")
        exit()
    else:
        return file_path
    
# Validate Seed String 
def seed(seed_str):
    valid_chars = {'a', 'b', 'c'}
    if not set(seed_str).issubset(valid_chars):
       print("Seed String Invalid")
       exit()
    else:
        return seed_str

# Validate Output Path
def output(file_path):
    directory = os.path.dirname(file_path)
    if directory and not os.path.exists(directory):
       print("Invalide File Output Path")
       exit()
    else:
        return file_path

# Validate Processes
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

def calculate_matrix(matrix, Indextuples):
    L = len(matrix)
    matrix_copy = [[None] * L for _ in range(L)]
    
    for n in Indextuples:
        
        neighbors_sum = sum(get_neighbors(matrix, n[0], n[1]))
    
        if matrix[n[0]][n[1]] == 0: 
            matrix_copy[n[0]][n[1]] = 0 if prime_check(neighbors_sum) else 1 if neighbors_sum % 2 == 0 else 2
        elif matrix[n[0]][n[1]] == 1:  
            matrix_copy[n[0]][n[1]] = 1 if prime_check(neighbors_sum) else 2 if neighbors_sum % 2 == 0 else 0
        else:
            matrix_copy[n[0]][n[1]] = 2 if prime_check(neighbors_sum) else 0 if neighbors_sum % 2 == 0 else 1
  
    return matrix_copy   


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

def process_matrix_worker(mp_data):
   matrix, Indextuples = mp_data
   matrix = calculate_matrix(matrix,Indextuples)
   return matrix

def run_multiprocessing(matrix, processes,IndexList): 
    
        IndexPerProcess = math.ceil(len(matrix)*len(matrix)/ processes)

        for _ in range(100):
            with multiprocessing.Pool(processes=processes) as pool:
                matrices = pool.map(process_matrix_worker, [(matrix.copy(),IndexList[(IndexPerProcess*(i)):((len(matrix)*len(matrix)) if IndexPerProcess*(i+1) > len(matrix)*len(matrix) else IndexPerProcess*(i+1))]) for i in range(processes)])        
            
            for poolMatrix in matrices:
                for row in range(len(matrix)):
                    for col in range(len(matrix)):
                        if poolMatrix[row][col] is not None:
                            matrix[row][col] = poolMatrix[row][col]
        return matrix
    

def main():
    print("Project :: R11766388")
    args = argument_parser()
    input_str = read_file(args.input)
    matrix = create_matrix(input_str, args.seed)
    
    processes = args.processes
    IndexList = []
        
    for i in range(len(matrix)):
        for j in range(len(matrix)):
            IndexList.append((i,j))
    if processes == 1: 
        for _ in range(100):
            matrix = calculate_matrix(matrix,IndexList)
    else:
        run_multiprocessing(matrix, processes,IndexList)
    print_matrix(matrix)    
    decrypted_str = decrypt_string(matrix, input_str)
    print("Decrypted String:", decrypted_str)
    
    write_output_file(decrypted_str, args.output)

def print_matrix(matrix):
    for row in matrix:
        print(row)

if __name__ == "__main__":
    main()