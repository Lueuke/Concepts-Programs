#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */

// Integer and Identifer 
#define INT_LIT 10
#define IDENT 11
// All Comparison Operators 
#define LESSER_OP 12
#define GREATER_OP 13
#define EQUAL_OP 14
#define NEQUAL_OP 15
#define LEQUAL_OP 16
#define GEQUAL_OP 17

// Semicolon 
#define SEMICOLON 18
// Increment and Decrement
#define DEC_OP 19
#define INC_OP 20
// Assingment Operator 
#define ASSIGN_OP 21
// Arithmetic Operators 
#define ADD_OP 22
#define SUB_OP 23
#define MULT_OP 24
#define DIV_OP 25
// Hard and Soft Brackets 
#define LEFT_PAREN 26
#define RIGHT_PAREN 27
#define LEFT_CBRACE 28
#define RIGHT_CBRACE 29
// Keywords
#define KEY_READ 30
#define KEY_WRITE 31
#define KEY_WHILE 32
#define KEY_DO 33




int lex();

#endif
