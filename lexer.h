#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED
#include<stdio.h>
#include "error.h"

#define MAX_LINE_SIZE 256

typedef enum
{
TOKEN_NONE,
TOKEN_TYPE,
TOKEN_FUNCTION,
TOKEN_MAP,
TOKEN_IDENTIFIER,
TOKEN_INTEGER,
TOKEN_FLOAT,
TOKEN_OPEN_PAREN,
TOKEN_CLOSE_PAREN,
TOKEN_OPEN_BRACE,
TOKEN_CLOSE_BRACE,
TOKEN_OPEN_SQUARE,
TOKEN_CLOSE_SQUARE,
TOKEN_OPEN_ANGLE,
TOKEN_CLOSE_ANGLE,
TOKEN_PIPE,
TOKEN_COMMA,
TOKEN_SEMICOLON
}token_type_t;

typedef struct
{
    union
    {
    char* str;
    int int_value;
    double float_value;
    }data;
token_type_t type;
unsigned short char_number;
unsigned short line_number;
}token_t;


typedef enum
{
LEXER_START,
LEXER_IDENTIFIER_ALPHA,
LEXER_IDENTIFIER_SYMBOL,
LEXER_MINUS,
LEXER_INTEGER,
LEXER_FLOAT,
LEXER_READ_PERIOD
}lexer_state_t;




typedef struct
{
FILE* file;
unsigned short char_number;
unsigned short line_number;
token_t next_token;
char line[MAX_LINE_SIZE];
}lexer_t;


void token_destroy(token_t* token);

error_t lexer_create(lexer_t* lexer,const char* filename);
void lexer_destroy(lexer_t* lexer);
error_t lexer_lex_token(lexer_t* lexer,token_t* token);



#endif // LEXER_H_INCLUDED
