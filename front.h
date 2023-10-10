#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define LESSER_OP 12
#define GREATER_OP 13
#define EQUAL_OP 14
#define NEQUAL_OP 15
#define LEQUAL_OP 16
#define GEQUAL_OP 17
#define SEMICOLON 18
#define INC_OP 19
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define DEC_OP 27 



int lex();

#endif
