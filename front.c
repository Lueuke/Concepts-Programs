/*
=============================================================================
Title : front.c
Description : This is an lexical analyzer based on the Grammer given in the Project file.
Author : Luke Dekan (R#11766388)
Date : 10/17/2023
Version : 1
Usage : Compile and run this program using the GNU C compiler
Notes : This  program has no requirements.
C Version : 6.3.0
=============================================================================
*/

/*Header Files*/ 
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
int main(int argc, char *argv[]) 
{
    // Print the Name and R Number for the grader to Identify 
    printf("DCooke Analyzer ::R11766388\n");
        
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
    // Have a switch statment for each charcter case and print out the lexeme with the lemexe token.
    switch (ch) {
        // Soft Brakets case
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            printf("%s      LEFT_PAREN\n", lexeme);
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            printf("%s      RIGHT_PAREN\n", lexeme);
            break;
        // Operators 
        case '+':
            addChar();
            getChar();
        // Check if the next charcter is the same and change the token 
         if (nextChar == '+') {
            addChar();
            nextToken = INC_OP;
            printf("%s     INC_OP\n", lexeme);
        // Else statement to set nextToken and remove the  unwanted character
        } else {
            nextToken = ADD_OP;
            printf("%s      ADD_OP\n", lexeme);
            ungetc(nextChar,in_fp);
        }
            break;
        case '-':
            addChar();
            getChar();
            // Check if the next charcter is the same and change the token 
            if (nextChar == '-') {
            addChar();
            nextToken = DEC_OP;
            printf("%s     DEC_OP\n", lexeme);
             // Else statement to set nextToken and remove the  unwanted character
        } else {
            nextToken = SUB_OP;
            printf("%s    SUB_OP\n", lexeme);
            ungetc(nextChar,in_fp);
        }
        break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            printf("%s      MULT_OP\n", lexeme);
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            printf("%s    DIV_OP\n", lexeme);
            break;
        case '=':
           addChar();
           getChar();
           // Check if the next charcter is the same and change the token
        if (nextChar == '=') {
            addChar();
            printf("%s     EQUAL_OP\n", lexeme);
           // Else statement to set nextToken and remove the  unwanted character
        } else {
            nextToken = ASSIGN_OP;
            printf("%s      ASSIGN_OP\n", lexeme);
            ungetc(nextChar,in_fp);
        }
            break;
        case '<':
            addChar();
            getChar();
            // Check if the next charcter is the equalop and change the token
            if (nextChar == '=') {
            addChar();
            nextToken = LEQUAL_OP;
            printf("%s    LEQUAL_OP\n", lexeme);
            // Else statement to set nextToken and remove the  unwanted character
        } else {
            nextToken = LESSER_OP;
            printf("%s    LESSER_OP\n", lexeme);
            ungetc(nextChar,in_fp);
        }
            break;
        case '>':
            addChar();
            getChar();
            // Check if the next charcter is the equalop and change the token
            if (nextChar == '=') {
            addChar();
            nextToken = GEQUAL_OP;
            printf("%s    GEQUAL_OP\n", lexeme);
            // Else statement to set nextToken and remove the  unwanted character
        } else {
            nextToken = GREATER_OP;
            printf("%s    GREATER_OP\n", lexeme);
            ungetc(nextChar,in_fp);
        }
            break;
        case '!':
            addChar();
            getChar();
            // Check if the next charcter is the equalop and change the token
            if(nextChar == '='){
            addChar();
            nextToken = NEQUAL_OP;
            printf("%s    NEQUAL_OP\n", lexeme);
            }
            // Else statement to set nextToken and remove the  unwanted character
            else {
                nextToken = UNKNOWN;
                ungetc(nextChar, in_fp);
            }
            break;
            //Set Semicolon token 
        case ';':
            addChar();
            nextToken = SEMICOLON;
            printf("%s      SEMICOLON\n", lexeme);
            break;
            // Set Hard Brackets Token
        case '{':
            addChar();
            nextToken = LEFT_CBRACE;
            printf("%s      LEFT_CBRACE\n", lexeme);
            break;
        case '}':
            addChar();
            nextToken = RIGHT_CBRACE;
            printf("%s      RIGHT_CBRACE\n", lexeme);
            break;
            // Unknown Characters 
        default:
            addChar();
            nextToken = UNKNOWN;
            printf("%s      UNKNOWN\n",lexeme);
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
            if (lexeme [0] == 'w' && lexeme[1] =='h' && lexeme[2]== 'i' && lexeme[3] == 'l' && lexeme[4] == 'e')
            {
                nextToken = KEY_WHILE;
                printf("%s  KEY_WHILE\n", lexeme);
            }
             else if(lexeme [0] == 'w' && lexeme[1] =='r' && lexeme[2]== 'i' && lexeme[3] == 't' && lexeme[4] == 'e')
            {
                nextToken = KEY_WRITE;
                printf("%s    KEY_WRITE\n", lexeme);
                 
            }
             else if(lexeme [0] == 'd' && lexeme[1] =='o' )
            {
                nextToken = KEY_DO;
                printf("%s     KEY_DO\n", lexeme);
            }
             else if(lexeme [0] == 'r' && lexeme[1] =='e' && lexeme[2]== 'a' && lexeme[3] == 'd')
            {
                nextToken = KEY_READ;
                 printf("%s   KEY_READ\n", lexeme);
            }
            // Else statment if any other patern set it to an identifer token
            else
            {
                nextToken = IDENT;
                printf("%s      IDENT\n", lexeme);
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
            printf("%s      INT_LIT\n", lexeme);
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

