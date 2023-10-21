# Set up the GCC Compiler set the excutable file name  
CC = gcc
CFlAGS = -Wall -Werror
TARGET = dcooke_analyzer

# Compile the .c files 
all: 
	$(CC) front.c  -o $(TARGET)
# Set up clean to remove the excutable if we run make clean 
clean:
	rm $(TARGET)
