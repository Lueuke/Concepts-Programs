#include <stdio.h>
#include "front.c"
#include "parser.h"
#include "front.h"

void expr();
void statements();
static void error();

int ExitCode;
/**
 * This is the example Recursive-Descent Parser in pp. 181 - 185 in the 
 * textbook 
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 *
 * */

int main() {
     ExitCode = 0;
    printf("DCooke Analyzer ::R11766388\n");
    
    // Open the "front.c" file for reading
    //  if (argc < 2) {
    //     ExitCode = 2;
    //     printf("Exit Code: %d Error - no filename provided\n", ExitCode);
    //     return ExitCode;
    // }
    if ((in_fp = fopen("exit1_1.dc", "r")) == NULL) {
        ExitCode = 3;
        printf("Exit Code: %d Error - cannot open file", ExitCode);

    } else {
        getChar();
        lex();
        statements();
        }
        if (ExitCode == 0) {
            printf("Syntax Validated");
        }
    return ExitCode;
}


void expr() 
{
    
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();     
    }
}


void comparison() {
    expr();  // Parse the left expression

    // Check for a single comparison operator and parse the right expression
    if (nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken == EQUAL_OP ||
        nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP) {
        lex();  // Consume the operator
        expr();  // Parse the right expression
    } else {
        // If no comparison operator is found, it's an error
        error();
        return;
    }
}


void statements() {
    while (1) {
        if (nextToken == UNKNOWN)
        {
            error();
            return;
        }
        if (nextToken == IDENT) {
            lex();
            if (nextToken == ASSIGN_OP) {
                lex();
                expr();
            } else {
                error();
                return;
            }
<<<<<<< HEAD
        } else {
            error();
        }
    } else if (nextToken == KEY_WHILE) {
        // Handle while loop
        lex(); // Consume the "while"
        if (nextToken == LEFT_PAREN) {
            lex(); // Consume the "("
            expr(); // Parse the loop condition
            comparison();
            if (nextToken == RIGHT_PAREN) {
                lex(); // Consume the ")"
                if (nextToken == SEMICOLON) {
                    lex(); // Consume the final semicolon
=======
        } else if (nextToken == KEY_READ) {
            lex();
            if (nextToken == LEFT_PAREN) {
                lex();
                if (nextToken == IDENT) {
                    lex();
>>>>>>> 1fc747d689794ed27354941885f8177b04e9729e
                } else {
                    error();
                    return;
                }
                if (nextToken == RIGHT_PAREN) {
                    lex();
                } else {
                    error();
                    return;
                }
            } else {
                error();
                return;
            }
<<<<<<< HEAD
        } else {
            error();
        }
    }  if (nextToken == IDENT) {
        lex(); // Consume the identifier
        if (nextToken = INC_OP || nextToken == DEC_OP)
        {
=======
        } else if (nextToken == KEY_WRITE) {
>>>>>>> 1fc747d689794ed27354941885f8177b04e9729e
            lex();
            if (nextToken == LEFT_PAREN) {
                lex();
                expr();
                if (nextToken == RIGHT_PAREN) {
                    lex();
                } else {
                    error();
                    return;
                }
            } else {
                error();
                return;
            }
        } else if (nextToken == KEY_DO) {
            lex();
            if (nextToken == LEFT_CBRACE) {
                lex();
                statements();  // Parse statements inside the loop
                if (nextToken == RIGHT_CBRACE) {
                    lex();
                    if (nextToken == KEY_WHILE) {
                        lex();
                        if (nextToken == LEFT_PAREN) {
                            lex();
                            comparison();
                            if (nextToken == RIGHT_PAREN) {
                                lex();
                            }
                             else {
                                error();
                                return;
                            }
                        } else {
                            error();
                            return;
                        }
                    } else {
                        error();
                        return;
                    }
                } else {
                    error();
                    return;
                }
            } else {
                error();
                return;
            }
        } else if (nextToken == SEMICOLON) {
            lex();  // Consume the semicolon
        } 
        
        else {
        
            break;  // Break out of the loop if none of the conditions match
        }
    }
}





/* term
 * Parses strings in the language generated by the rule:
 * <term> -> <factor> {(* | /) <factor>)
 */
void term() 
{

    /* Parse the first factor */
    factor();
    /* As long as the next token is * or /, get the
    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) 
    {
            lex();
            factor();
    }


} /* End of function term */


<<<<<<< HEAD

void factor() 
{

     if (nextToken == IDENT || nextToken == INT_LIT) {
        lex(); /* Get the next token */
=======
void factor() {
    if (nextToken == IDENT) {
        lex(); // Get the next token
        // Check for increment or decrement
        if (nextToken == INC_OP || nextToken == DEC_OP) {
            lex(); // Consume INC_OP or DEC_OP
        }
    } else if (nextToken == INT_LIT) {
        lex(); // Get the next token
>>>>>>> 1fc747d689794ed27354941885f8177b04e9729e
    } else if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        } else {
            error();
            return;
        }
    } else {
        error();
        return;
    }
}

static void error() {

     printf("Error encounter: The next lexeme was %s and the next token was %s", lexeme, getTokenName(nextToken));
    ExitCode = 1;
    return;
}
