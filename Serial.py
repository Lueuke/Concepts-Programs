import threading 
import multiprocessing
import argparse

parser = argparse.ArgumentParser()
args = parser.parse_args()

def validate_seed_string(seed_string):
    if not all(char in 'abc' for char in seed_string):
        raise argparse.ArgumentTypeError("Seed string must consist of lowercase letters a, b, and c.")
    return seed_string

parser.add_argument('-i', '--input', type=validate_file_path, help='Path to input file', required=True)
parser.add_argument('-s', '--seed', type=validate_seed_string, help='Seed string', required=True)
parser.add_argument('-o', '--output', type=validate_output_path, help='Path to output file', required=True)
parser.add_argument('-p', '--processes', type=validate_positive_integer, default=1, help='Number of processes to spawn')

input_file = args.input
seed_string = args.seed


def create_matrix(input_file):
    matrix_size = int((len(input_file)^2))

    # Create a matrix (list of lists)
    matrix = [[0] * matrix_size for _ in range(matrix_size)]

    for i in range(matrix_size):
        for j in range(matrix_size):
            matrix[i][j] = seed_string[seed_index]
            seed_index = (seed_index + 1) % len(seed_string)

    return matrix




#if __name__ == "__main__":
#   main()