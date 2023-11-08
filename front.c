/*
=============================================================================
Title : front.c
Description : This is an lexical analyzer where we take command line arguments from a file and set each lexeme to a Token name 
Author : Luke Dekan (R#11766388)
Date : 10/17/2023
Version : 1
Usage : Compile and run this program using the GNU C compiler it must take in the source code files as command line arguments for it to function properly 
Notes : This  program has no requirements.
C Version : 6.3.0
=============================================================================
*/

/*Header Files*/ 
#include <stdio.h>
#include <ctype.h>
#include "front.h"
#include "parser.h"
#include <string.h>


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
// int main(int argc, char *argv[]) 
// {
//     // Print the Name and R Number for the grader to Identify 
//     printf("DCooke Analyzer ::R11766388\n");
        
//     /* Open the input data file and process its contents */
//     if ((in_fp = fopen("example_1.dc", "r")) == NULL) 
//     {
//         printf("ERROR - cannot open front.in \n");
//     } 
//     else {
//         getChar();
//         do {
//             lex();
        
//         } while (nextToken != EOF);
//     }

//     return 0;
// }

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    // Have a switch statment for each lexeme case and print out the lexeme with the right lemexe token.
    switch (ch) {

        // Soft Brakets case
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;

        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
           
            break;

        // Arithmatic Operators and Increment and Decrement Operators
        case '+':
            addChar();
            getChar();
        // Check if the next charcter is the same and change the token name
         if (nextChar == '+') {
            addChar();
            nextToken = INC_OP;
            
        // Else statement to set nextToken and remove the unwanted character with ungetc
        } else {
            nextToken = ADD_OP;
            
            ungetc(nextChar,in_fp);
        }
            break;

        case '-':
            addChar();
            getChar();
            // Check if the next charcter is the same and change the token name
            if (nextChar == '-') {
            addChar();
            nextToken = DEC_OP;
            
             // Else statement to set nextToken and remove the  unwanted character with ungetc
        } else {
            nextToken = SUB_OP;
            ungetc(nextChar,in_fp);
        }
        break;

        case '*':
            addChar();
            nextToken = MULT_OP;
            
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
           
            break;

        // Equal Operator
        case '=':
           addChar();
           getChar();
           // Check if the next charcter is the same and change the token name
        if (nextChar == '=') {
            addChar();
            
           // Else statement to set nextToken and remove the  unwanted character with ungetc
        } else {
            nextToken = ASSIGN_OP;
            
            ungetc(nextChar,in_fp);
        }
            break;

        //Comparison Operators
        case '<':
            addChar();
            getChar();
            // Check if the next charcter is the EQUALOP and change the token name
            if (nextChar == '=') {
            addChar();
            nextToken = LEQUAL_OP;
            
            // Else statement to set nextToken and remove the  unwanted character with ungetc
        } else {
            nextToken = LESSER_OP;
            
            ungetc(nextChar,in_fp);
        }
            break;

        case '>':
            addChar();
            getChar();
            // Check if the next charcter is the equalop and change the token name
            if (nextChar == '=') {
            addChar();
            nextToken = GEQUAL_OP;
           
            // Else statement to set nextToken and remove the  unwanted character with ungetc
        } else {
            nextToken = GREATER_OP;
            
            ungetc(nextChar,in_fp);
        }
            break;

        case '!':
            addChar();
            getChar();
            // Check if the next charcter is the equalop and change the token name
            if(nextChar == '='){
            addChar();
            nextToken = NEQUAL_OP;
           
            }
            // Else statement to set nextToken and remove the  unwanted character with ungetc
            else {
                nextToken = UNKNOWN;
                ungetc(nextChar, in_fp);
            }
            break;

            //Set Semicolon token 
        case ';':
            addChar();
            nextToken = SEMICOLON;
           
            break;

            // Set Hard Brackets Token
        case '{':
            addChar();
            nextToken = LEFT_CBRACE;
            
            break;

        case '}':
            addChar();
            nextToken = RIGHT_CBRACE;
            
            break;

            // Unknown Characters 
        default:
            addChar();
            nextToken = UNKNOWN;
            
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

            // If the Lexme Pattern Matches the keyword set the right Token to the keyword
            if (strcmp(lexeme,"while") == 0)
            {
                nextToken = KEY_WHILE;
               
            }
             else if(strcmp(lexeme,"write") == 0)
            {
                nextToken = KEY_WRITE;
               
                 
            }
             else if(strcmp(lexeme,"do") == 0)
            {
                nextToken = KEY_DO;
                
            }
             else if(strcmp(lexeme,"read") == 0)
            {
                nextToken = KEY_READ;
                 
            }
            // Else statment if any other patern set it to an identifer token
            else
            {
                nextToken = IDENT;
                
            }
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
            break;
        
    } 
    return nextToken;
} /* End of function lex */

