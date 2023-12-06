"""
=============================================================================
Title : LukeDekan_R11766388_final_project.py
Description : This program is used to decipher a input string based off a seed both given by the user using multiprocessing to distribute the workload then printing results to an output file.
Author : Luke Dekan (R#11766388)
Date : 12/05/2023
Version : 1.0
Usage : python3 LukeDekan_R11766388_final_project.py -i Input File -s Seed String -o Output File -p # Of processes
Notes : This program must be given and Input File, Output File and Seed String contain only the letters a,b,c.
Python Version: 3.12.0
=============================================================================
"""
# Import all the modules needed for this program
import argparse
import os
import multiprocessing
import math 

# Function to Take command Arguments 
def argument_parser():
    # Take all the input arguments.
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=input, required=True,)
    parser.add_argument('-o', '--output', type=output, required=True,)
    parser.add_argument('-s', '--seed', type=seed, required=True,)
    parser.add_argument("-p", "--processes", type=processes, default=1,)

    arguments = parser.parse_args()
    return arguments

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

# Open and Read Input File
def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

# Fill the Matrix with the Corresponding Values
def create_matrix(input_str, seed_str):

    L = len(input_str)
    matrix = [[0] * L for _ in range(L)]
    
    # Swap the letter values for number values
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

# For the values in the matrix calculate based off the rules given to us
def calculate_matrix(matrix, Indextuples):
    L = len(matrix)
    # Create a copy of the matrix at the same size 
    matrix_copy = [[None] * L for _ in range(L)]
    
    # Loop through the tuples and calculate the sum and swap values based on neighbors 
    for n in Indextuples:
        
        neighbors_sum = sum(get_neighbors(matrix, n[0], n[1]))
        # Based off the values perform the operations 
        if matrix[n[0]][n[1]] == 0: 
            matrix_copy[n[0]][n[1]] = 0 if prime_check(neighbors_sum) else 1 if neighbors_sum % 2 == 0 else 2
        elif matrix[n[0]][n[1]] == 1:  
            matrix_copy[n[0]][n[1]] = 1 if prime_check(neighbors_sum) else 2 if neighbors_sum % 2 == 0 else 0
        else:
            matrix_copy[n[0]][n[1]] = 2 if prime_check(neighbors_sum) else 0 if neighbors_sum % 2 == 0 else 1
    # Return final matrix
    return matrix_copy   

# Find if a number is a prime value 
def prime_check(num):
    if num < 2:
        return False
    # Check if the number is divisible evenly 
    for i in range(2, int(num**0.5) + 1):
        if num % i == 0:
            return False
    return True

# Get the neighboring values 
def get_neighbors(matrix, i, j):
    neighbors = []

    rows, cols = len(matrix), len(matrix[0])
    # Find every neighbor above and below and to the sides of the values 
    for ni in range(i - 1, i + 2):
        for nj in range(j - 1, j + 2):
            if 0 <= ni < rows and 0 <= nj < cols and (ni != i or nj != j):
                # Add the elements to the neighbor list
                neighbors.append(matrix[ni][nj])
    return neighbors

# Decrypt the String with the Decrpyt Function 
def decrypt_string(matrix, encrypted_str):
    # Create an empty string
    decrypted_str = ''
    for j in range(len(matrix[0])):
        # Sum up all the columns 
        column_sum = sum([matrix[i][j] for i in range(len(matrix))])
        # Find the decrypted letter and Add each decrypted letter to the decrypted string
        decrypted_letter = decrypt_letter(encrypted_str[j], column_sum)
        decrypted_str += decrypted_letter

    return decrypted_str

# Decrypt Letter Function give to us 
def decrypt_letter(letter, rotationValue):
  rotationString  = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ "
  currentPosition = rotationString.find(letter)
  return rotationString[(currentPosition + rotationValue) % 95]

# Open the output file and write the final decrypted string to it 
def write_output_file(output_str, file_path):
    with open(file_path, 'w') as file:
        file.write(output_str)

# Taking the data we calculate in the multiprocessing function calculate the matrix with multiprocessing 
def mp_process_matrix(data):
   # Set the matrix and tuple data 
   matrix, Indextuples = data
   # Calcuate the final matrix 
   matrix = calculate_matrix(matrix,Indextuples)
   return matrix

# Using multiprocessing divide we divide up the process by rows and use a tuple inside a list 
def run_multiprocessing(matrix, processes,IndexList): 

        # Calculate how many processes per index we are going to need 
        IndexPerProcess = math.ceil(len(matrix)*len(matrix)/ processes)

        # Loop through the multiprocessing pool 100 
        for _ in range(100):
            with multiprocessing.Pool(processes=processes) as pool:
                # Using the calculation we get the Start and end index for the tuple 
                matrices = pool.map(mp_process_matrix, [(matrix.copy(),IndexList[(IndexPerProcess*(i)):((len(matrix)*len(matrix)) if IndexPerProcess*(i+1) > len(matrix)*len(matrix) else IndexPerProcess*(i+1))]) for i in range(processes)])        
            
            # Place the results into the final matrix 
            for poolMatrix in matrices:
                for row in range(len(matrix)):
                    for col in range(len(matrix)):
                        if poolMatrix[row][col] is not None:
                            matrix[row][col] = poolMatrix[row][col]
        return matrix
    
# Main function used to call our functions 
def main():

    print("Project :: R11766388")
    # Call our parsing argurmets and our input text
    args = argument_parser()
    input = read_file(args.input)
    # Create out matrix 
    matrix = create_matrix(input, args.seed)
    # Find the number of processes we have 
    processes = args.processes

    # Create and empy list 
    IndexList = []

    # Fill that list with the tuples of our matrix 
    for i in range(len(matrix)):
        for j in range(len(matrix)):
            IndexList.append((i,j))

    # If its just one process run serially anything else we run multiprocessing 
    if processes == 1: 
        for _ in range(100):
            matrix = calculate_matrix(matrix,IndexList)
    else:
        run_multiprocessing(matrix, processes,IndexList) 

    # Decrypt the string 
    decrypted_str = decrypt_string(matrix, input)
    print("Decrypted String:", decrypted_str)

    # Write the final output
    write_output_file(decrypted_str, args.output)

if __name__ == "__main__":
    main()