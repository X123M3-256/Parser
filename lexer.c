#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"


void print_token(token_t* token)
{
    switch(token->type)
    {
    case TOKEN_NONE:
    printf("<none>\n");
    break;
    case TOKEN_INTEGER:
    printf("%d\n",token->data.int_value);
    break;
    case TOKEN_FLOAT:
    printf("%f\n",token->data.float_value);
    break;
    case TOKEN_IDENTIFIER:
    printf("%s\n",token->data.str);
    break;
    case TOKEN_OPEN_PAREN:
    printf("(\n");
    break;
    case TOKEN_CLOSE_PAREN:
    printf(")\n");
    break;
    case TOKEN_OPEN_BRACE:
    printf("{\n");
    break;
    case TOKEN_CLOSE_BRACE:
    printf("}\n");
    break;
    case TOKEN_OPEN_SQUARE:
    printf("[\n");
    break;
    case TOKEN_CLOSE_SQUARE:
    printf("]\n");
    break;
    case TOKEN_OPEN_ANGLE:
    printf("<\n");
    break;
    case TOKEN_CLOSE_ANGLE:
    printf(">\n");
    break;
    case TOKEN_TYPE:
    printf("type\n");
    break;
    case TOKEN_FUNCTION:
    printf("function\n");
    break;
    case TOKEN_PIPE:
    printf("|\n");
    break;
    case TOKEN_COMMA:
    printf(",\n");
    break;
    case TOKEN_SEMICOLON:
    printf(";\n");
    break;
    }
}

void token_destroy(token_t* token)
{
    if(token->type==TOKEN_IDENTIFIER)free(token->data.str);
}

error_t lexer_error(lexer_t* lexer,const char* msg)
{
error_t error;
error.message=msg;
error.line_number=lexer->line_number+1;
error.char_number=lexer->char_number+1;
return error;
}

error_t lexer_create(lexer_t* lexer,const char* filename)
{
error_t error=error_none;
//Initialise values
lexer->line_number=0;
lexer->char_number=0;
lexer->next_token.type=TOKEN_NONE;
lexer->next_token.line_number=0;
lexer->next_token.char_number=0;

//Open file
lexer->file=fopen(filename,"rb");
    if(lexer->file==NULL)
    {
    error=lexer_error(lexer,"File not found");
    goto on_error;
    }
//Load first line
    if(fgets(lexer->line,MAX_LINE_SIZE,lexer->file)==NULL)
    {
    error=lexer_error(lexer,"Failed while reading file");
    goto on_error;
    }

//Make next_token contain the first token
    if(IS_ERROR(error=lexer_lex_token(lexer,&(lexer->next_token))))goto on_error;

return error_none;

//Handle errors
on_error:
lexer_destroy(lexer);
return error;
}

void lexer_destroy(lexer_t* lexer)
{
token_destroy(&lexer->next_token);
    if(lexer->file!=NULL)fclose(lexer->file);
}

void lexer_advance(lexer_t* lexer,int characters)
{
lexer->char_number+=characters;
    if(lexer->line[lexer->char_number]==0)
    {
        if(fgets(lexer->line,MAX_LINE_SIZE,lexer->file)==NULL)return;
    lexer->char_number=0;
    lexer->line_number++;
    }
}



int is_num(char cur_char)
{
return cur_char>='0'&&cur_char<='9';
}

int is_alpha(char cur_char)
{
return (cur_char>='a'&&cur_char<='z')||(cur_char>='A'&&cur_char<='Z')||cur_char=='_';
}

int is_alphanum(char cur_char)
{
return is_num(cur_char)||is_alpha(cur_char);
}

int is_symbol(char cur_char)
{
return !is_alphanum(cur_char)&&cur_char!='('&&cur_char!=')'&&cur_char!='['&&cur_char!=']'&&cur_char!='{'&&cur_char!='}'&&cur_char!=','&&cur_char!=';';
}

error_t lexer_lex_token(lexer_t* lexer,token_t* token)
{
token_t new_token;

//Strip whitespace
    while(lexer->line[lexer->char_number]==' '||lexer->line[lexer->char_number]=='\t'||lexer->line[lexer->char_number]=='\n')lexer_advance(lexer,1);


new_token.type=TOKEN_NONE;
new_token.char_number=lexer->char_number+1;
new_token.line_number=lexer->line_number+1;

    //Return nothing if there are no more tokens
    if(lexer->line[lexer->char_number]==0)
    {
    *token=lexer->next_token;
    lexer->next_token=new_token;
    return error_none;
    }

//Find single character tokens
    switch(lexer->line[lexer->char_number])
    {
    case '(':
    new_token.type=TOKEN_OPEN_PAREN;
    break;
    case ')':
    new_token.type=TOKEN_CLOSE_PAREN;
    break;
    case '{':
    new_token.type=TOKEN_OPEN_BRACE;
    break;
    case '}':
    new_token.type=TOKEN_CLOSE_BRACE;
    break;
    case '[':
    new_token.type=TOKEN_OPEN_SQUARE;
    break;
    case ']':
    new_token.type=TOKEN_CLOSE_SQUARE;
    break;
    case '<':
    new_token.type=TOKEN_OPEN_ANGLE;
    break;
    case '>':
    new_token.type=TOKEN_CLOSE_ANGLE;
    break;
    case '|':
    new_token.type=TOKEN_PIPE;
    break;
    case ',':
    new_token.type=TOKEN_COMMA;
    break;
    case ';':
    new_token.type=TOKEN_SEMICOLON;
    break;
    }

    if(new_token.type!=TOKEN_NONE)
    {
    lexer_advance(lexer,1);
    *token=lexer->next_token;
    lexer->next_token=new_token;
    return error_none;
    }

const char* token_str=lexer->line+lexer->char_number;

//FSM that distinguishes numeric tokens from alphanumeric ones
lexer_state_t state=LEXER_START;
int token_length=0;
int offset=0;

int lexing=1;

    while(lexing)
    {
    char cur_char=token_str[offset];
        switch(state)
        {
        case LEXER_START:
            if(is_num(cur_char))state=LEXER_INTEGER;
            else if(is_alpha(cur_char))state=LEXER_IDENTIFIER_ALPHA;
            else if(cur_char=='-')state=LEXER_MINUS;
	    else if(is_symbol(cur_char))state=LEXER_IDENTIFIER_SYMBOL;
            else lexing=0;
        break;
        case LEXER_MINUS:
	new_token.type=TOKEN_IDENTIFIER;
	token_length=offset;
            if(is_num(cur_char))state=LEXER_INTEGER;
	    else if(is_symbol(cur_char))state=LEXER_IDENTIFIER_SYMBOL;
            else lexing=0;
        break;
        case LEXER_INTEGER:
        new_token.type=TOKEN_INTEGER;
        token_length=offset;
            if(is_num(cur_char))state=LEXER_INTEGER;
            else if(cur_char=='.')state=LEXER_READ_PERIOD;
            else lexing=0;
        break;
        case LEXER_FLOAT:
        new_token.type=TOKEN_FLOAT;
        token_length=offset;
        case LEXER_READ_PERIOD:
            if(is_num(cur_char))state=LEXER_FLOAT;
            else lexing=0;
        break;
        case LEXER_IDENTIFIER_ALPHA:
        new_token.type=TOKEN_IDENTIFIER;
        token_length=offset;
            if(is_alphanum(cur_char))state=LEXER_IDENTIFIER_ALPHA;
            else lexing=0;
        break;
        case LEXER_IDENTIFIER_SYMBOL:
        new_token.type=TOKEN_IDENTIFIER;
        token_length=offset;
            if(is_symbol(cur_char))state=LEXER_IDENTIFIER_SYMBOL;
            else lexing=0;
        break;
}
    offset++;
    }

    if(new_token.type==TOKEN_NONE)return lexer_error(lexer,"Unrecognized token");

//Check for special tokens that would otherwise be valid identifiers
    if(new_token.type==TOKEN_IDENTIFIER)
    {
        if(token_length==4&&strncmp("type",token_str,4)==0)new_token.type=TOKEN_TYPE;
        else if(token_length==8&&strncmp("function",token_str,8)==0)new_token.type=TOKEN_FUNCTION;
        else if(token_length==4&&strncmp("->",token_str,4)==0)new_token.type=TOKEN_MAP;
    } 

//Extract relevant data from tokens
    if(new_token.type==TOKEN_IDENTIFIER)
    {
    new_token.data.str=malloc(token_length+1);
    memcpy(new_token.data.str,token_str,token_length);
    new_token.data.str[token_length]=0;
    }
    else if(new_token.type==TOKEN_INTEGER)
    {
    new_token.data.int_value=strtol(token_str,NULL,10);
    }
    else if(new_token.type==TOKEN_FLOAT)
    {
    new_token.data.float_value=strtod(token_str,NULL);
    }

lexer_advance(lexer,token_length);

//Return next_token, and make the new next_token the token just lexed. This gives 1 token of lookahead
*token=lexer->next_token;
lexer->next_token=new_token;
return error_none;
}

