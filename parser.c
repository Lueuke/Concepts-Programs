/*
=============================================================================
Title : front.c
Description : This is an parser where we take command line arguments from a file and first set the lexemes then check if it syntactially correct based off the grammer give to us
Author : Luke Dekan (R#11766388)
Date : 11/11/2023
Version : 1
Usage : Compile and run this program using the GNU C compiler it must take in the source code files as command line arguments for it to function properly 
Notes : This  program has no requirements.
C Version : 6.3.0
=============================================================================
*/


// Header Files
#include <stdio.h>
#include "front.c"
#include "parser.h"
#include "front.h"


// Declare Functions
void expr();
void statements();
static void error();

// Intialize our ExitCode variable 
int ExitCode;

// Main to open file and run through parser 
int main(int argc, char *argv[]) {

    // Set Exit Code to Zero and print out Title and R Number
     ExitCode = 0;
    printf("DCooke Analyzer ::R11766388\n");
    
    // Check if the file can be opened if not set right exit code 
    //   if (argc < 2) 
    //   {
    //     ExitCode = 2;
    //      printf("Exit Code: %d Error - no filename provided\n", ExitCode);
    //      return ExitCode;
    //   }

    if ((in_fp = fopen("exit1_1.dc", "r")) == NULL) 
    {
        ExitCode = 3;
        printf("Exit Code: %d Error - cannot open file", ExitCode);

    } 
    // If file can be opened begin parsing 
    else 
    {
        // Get the Next Character
        getChar();
        // Lexical Analyzer 
        lex();
        statements();
    }
    // If Syntax is valid print our correct message 
    if (ExitCode == 0) 
    {
        printf("Syntax Validated");
    }
    return ExitCode;
}

// Parse Expression Function based off Grammer 
void expr() 
{
    // Parse term 
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();     
    }
}

// Comparison function for While statement 
void comparison() 
{
    // Parse the first expression 
    expr();  

    // Check for comparison Operators 
    if (nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken == EQUAL_OP ||
        nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP) 
    {
        lex();  
        expr();  
    } 
    // If not Comparison operator found error out 
    else 
    {
    
        error();
        return;
    }
}

// Statements Function based off Grammer 
void statements() 
{
    while (1) 
    {
        // If Unknown Token Error out 
        if (nextToken == UNKNOWN)
        {
            error();
            return;
        }

        // Parse IDENT 
        if (nextToken == IDENT) 
        {
            lex();
            if (nextToken == ASSIGN_OP) 
            {
                lex();
                expr();
            } 
            else 
            {
                error();
                return;
            }
        } 
        // Parse Read Keyword  based off Grammer 
        else if (nextToken == KEY_READ) 
        {
            lex();
            if (nextToken == LEFT_PAREN) 
            {
                lex();
                if (nextToken == IDENT) 
                {
                    lex();
                } 
                else 
                {
                    error();
                    return;
                }
                if (nextToken == RIGHT_PAREN) 
                {
                    lex();
                } 
                else 
                {
                    error();
                    return;
                }
            } 
            else 
            {
                error();
                return;
            }
        } 
        // Parse Keyword Write based off Grammer 
        else if (nextToken == KEY_WRITE) 
        {
            lex();
            if (nextToken == LEFT_PAREN) 
            {
                lex();
                expr();
                if (nextToken == RIGHT_PAREN) 
                {
                    lex();
                } 
                else 
                {
                    error();
                    return;
                }
            } 
            else 
            {
                error();
                return;
            }
        // Parse Keyword Do based off Grammer
        } 
        else if (nextToken == KEY_DO) 
        {
            lex();
        if (nextToken == LEFT_CBRACE) 
        {
            lex();
            // Parse Statements inside Curly Brakets 
            statements(); 
        if (nextToken == RIGHT_CBRACE) 
        {
            lex();
            
        if (nextToken == KEY_WHILE) 
        {
            lex();
        if (nextToken == LEFT_PAREN) 
        {
            lex();
            // Find Comparision Operators in the While brackets
            comparison();
        if (nextToken == RIGHT_PAREN) 
        {
            lex();
        }
        // Error For LEFT_PAREN 
            else 
            {
                error();
                return;
            }
        } 
        // Error For WHILE 
            else 
            {
                error();
                return;
            }
        } 
        // Error For RIGHT_CBRACE
            else 
            {
                error();
                return;
            }
        } 
        // Error For RIGHT_CBRACE
            else 
            {
                error();
                return;
            }
        } 
        // Error For KEY_DO
        else 
        {
            error();
            return;
        }
        } 
        // PARSE Semicolon based off Grammer 
        else if (nextToken == SEMICOLON) 
        {
            lex();  // Consume the semicolon
            if (nextToken == RIGHT_CBRACE)
            {
                error();
                return;
            }
            
        } 
        // Break from statement if none of the cases match 
        else 
        {
            break;  
        }
    }
}




// Term function to parse based off Grammer 
void term() 
{

    // Parse Factor
    factor();
    while (nextToken == MULT_OP || nextToken == DIV_OP) 
    {
            lex();
            factor();
    }


} 

// Factor function to parse based off Grammer 
void factor() {
    // Parse IDENT Token
    if (nextToken == IDENT) 
    {
        lex(); 
    // If INC_OP or DEC_OP comes after factor parse 
        if (nextToken == INC_OP || nextToken == DEC_OP) 
        {
            lex(); 
        }
        if(nextToken == KEY_WRITE || nextToken == KEY_DO || nextToken == KEY_READ || nextToken == KEY_WHILE)
        {
            error();
            return;
        }
    } 
    // Parse INT_LIT 
    else if (nextToken == INT_LIT) 
    {
        lex(); 
    } 
    // Parse LEFT_PAREN
    else if (nextToken == LEFT_PAREN) 
    {
        lex();
        expr();
    // Parse RIGHT_PAREN
        if (nextToken == RIGHT_PAREN) 
        {
            lex();
        } 
        // Error for RIGHT_PAREN
        else 
        {
            error();
            return;
        }
    } 
    // Error for LEFT_PAREN
    else 
    {
        error();
        return;
    }
}

// Error Function to print out error message and set ExitCode
static void error() {
     // Print Error
    printf("Error encounter: The next lexeme was %s and the next token was %s", lexeme, getTokenName(nextToken));
    // Set ExitCode
    ExitCode = 1;
    return;
}
