/*
=============================================================================
Title : front.c
Description : This is an lexical analyzer Based on the Grammer given to us.
Author : Luke Dekan (R#11766388)
Date : 10/16/2023
Version : 1
Usage : Compile and run this program using the GNU C compiler
Notes : This example program has no requirements.
C Version : 6.3.0
=============================================================================
*/

#include <stdio.h>
#include <ctype.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main() 
{
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("test.dc", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } else {
        getChar();
        do {
            lex();
        
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            getChar();
         if (nextChar == '+') {
            addChar();
            nextToken = INC_OP;
            getChar();
            } else {
            nextToken = ADD_OP;
        }
            break;
        case '-':
            addChar();
            getChar();
            if (nextChar == '-') {
            addChar();
            nextToken = DEC_OP;
            getChar();
        } else {
            nextToken = SUB_OP;
        }
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=':
           addChar();
            getChar();
        if (nextChar == '=') {
            addChar();
            nextToken = EQUAL_OP;
            getChar();
        } else {
            nextToken = ASSIGN_OP;
        }
            break;
        case '<':
            if (nextChar == '=') {
            addChar();
            nextToken = LEQUAL_OP;
            getChar();
        } else {
            nextToken = LESSER_OP;
        }
            break;
        case '>':
            if (nextChar == '=') {
            addChar();
            nextToken = GEQUAL_OP;
            getChar();
        } else {
            nextToken = GREATER_OP;
        }
            break;
        case '!=':
            addChar();
            nextToken = NEQUAL_OP;
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
        case '{':
            addChar();
            nextToken = LEFT_CBRACE;
            break;
        case '}':
            addChar();
            nextToken = RIGHT_CBRACE;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */

