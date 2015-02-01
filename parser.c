#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"



error_t parser_error(token_t* token,const char* msg)
{
error_t error;
error.char_number=token->char_number;
error.line_number=token->line_number;
error.message=msg;
return error;
}

char* string_copy(const char* source)
{
char* copy=malloc(strlen(source)+1);
strcpy(copy,source);
return copy;
}

error_t parse_expression_list(lexer_t* lexer,expression_list_t** list_ptr)
{
error_t error;
token_t token;
    if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;

    if(token.type==TOKEN_IDENTIFIER)
    {
    expression_list_t* list=malloc(sizeof(expression_list_t));
        if(IS_ERROR(error=parse_expression(lexer,&(list->expression))))return error;
        if(IS_ERROR(error=parse_expression_list(lexer,&(list->expression))))return error;
    }else *list_ptr=NULL;
}

error_t parse_expression(lexer_t* lexer,expression_t* expression)
{
error_t error;
token_t token;
    if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;

    switch(token.type)
    {
    case TOKEN_INTEGER:
    expression->type=EXPRESSION_INTEGER;
    expression->data.integer_value=token.data.int_value;
    break;
    case TOKEN_FLOAT:
    expression->type=EXPRESSION_FLOAT;
    expression->data.float_value=token.data.float_value;
    break;
    case TOKEN_IDENTIFIER:
        if(lexer->next_token.type==TOKEN_OPEN_PAREN)
        {
        expression->type=EXPRESSION_FUNCTION_CALL;
        expression->data.function_call.identifier=token.data.str;
            if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;

            if(IS_ERROR(error=parse_expression_list(lexer,&(expression->data.function_call.parameters))))return error;

            if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;
            if(token.type!=TOKEN_CLOSE_BRACE)return parser_error(&token,"Expected ')'");
        }
        else
        {
        expression->type=EXPRESSION_IDENTIFIER;
        expression->data.identifier=token.data.str;
        }
    }
return error_none;
}

/*
error_t parse_declaration(lexer_t* lexer,symbol_table_t* symbol_table)
{
error_t error;
token_t token;

variable_t* variable=malloc(sizeof(variable_t));
variable->flags=0;
variable->initial_value=NULL;
    if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;
//Check that first token is variable/constant
    if(token.type!=TOKEN_KEYWORD||!(token.data.subtype==KEYWORD_CONSTANT||token.data.subtype==KEYWORD_VARIABLE))
    {
    error=parser_error(&token,"Expected 'variable' or 'constant' declaration");
    goto on_error;
    }
//Set constant
    if(token.data.subtype==KEYWORD_CONSTANT)variable->flags|=VARIABLE_CONSTANT;

token_destroy(&token);

//Check second token is type
    if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;
    if(token.type!=TOKEN_TYPE)
    {
    error=parser_error(&token,"Expected variable type specifier");
    goto on_error;
    }
//Set type
    if(token.data.subtype==TYPE_REAL)variable->type=REAL;
    else variable->type=VECTOR;

token_destroy(&token);

//Check third token is identifier
    if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;
    if(token.type!=TOKEN_IDENTIFIER)
    {
    error=parser_error(&token,"Expected identifier");
    goto on_error;
    }
    variable->identifier=string_copy(token.data.str);
//Add variable to symbol table
    if(IS_ERROR(error=symbol_table_add_variable(symbol_table,variable)))
    {
    error=parser_error(&token,error.message);
    token_destroy(&token);
    goto on_error;
    }
token_destroy(&token);


//Read fourth token
    if(IS_ERROR(error=lexer_lex_token(lexer,&token)))return error;
    if(token.type!=TOKEN_SEMICOLON)
    {
    error=parser_error(&token,"Expected semicolon");
    return error;
    }
token_destroy(&token);
return error_none;

on_error:
free(variable);
token_destroy(&token);
return error;
}


error_t parse_file(const char* filename,track_t* track)
{
error_t error;
lexer_t lexer;
//Create lexer
    if(IS_ERROR(error=lexer_create(&lexer,filename)))return error;
//Parse track
    if(IS_ERROR(error=parse_track(&lexer,track)))goto on_error;
//Destroy lexer
lexer_destroy(&lexer);
return error_none;
//Handle errors
on_error:
lexer_destroy(&lexer);
return error;
}
*/

