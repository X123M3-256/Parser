#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "lexer.h"


/*

typedef enum
{
SYMBOL_VARIABLE,
SYMBOL_FUNCTION,
SYMBOL_ELEMENT
}symbol_type_t;

typedef struct
{
symbol_type_t type;
char* identifier;
    union
    {
    struct variable_s* variable;
    struct function_s* function;
    struct element_s* element;
    }data;
}symbol_t;

typedef struct symbol_table_s
{
struct symbol_table_s* parent;
symbol_t* symbols;
symbol_t* cache;
int num_symbols;
int allocated_symbols;
}symbol_table_t;

*/
struct expression_s;

typedef struct expression_list_s
{
struct expression_s* expression;
struct expression_list_s* next;
}expression_list_t;

typedef struct
{
char* identifier;
expression_list_t* parameters;
}function_call_t;

typedef enum
{
EXPRESSION_INTEGER,
EXPRESSION_FLOAT,
EXPRESSION_IDENTIFIER,
EXPRESSION_FUNCTION_CALL
}expression_type_t;

typedef struct
{
expression_type_t type;
    union
    {
    function_call_t function_call;
    int integer_value;
    float float_value;
    char* identifier;
    }data;
}expression_t;

typedef struct
{

}type_t;

typedef struct type_parameter_list_s
{
char* identifier;
struct type_parameter_list_s* next;
}type_parameter_list_t;

typedef struct
{
token_t identifier;
type_parameter_list_t* parameters;
type_t type;
}type_declaration_t;


error_t parse_expression(lexer_t* lexer,expression_t* expression);

/*

void symbol_table_create(symbol_table_t* symbol_table);
void symbol_table_enter_scope(symbol_table_t* symbol_table,symbol_table_t* new_scope);
void symbol_table_destroy(symbol_table_t* symbol_table);
error_t symbol_table_add_variable(symbol_table_t* symbol_table,variable_t* variable);
error_t symbol_table_add_function(symbol_table_t* symbol_table,function_t* function);
error_t symbol_table_add_element(symbol_table_t* symbol_table,element_t* element);
error_t symbol_table_get_type(symbol_table_t* symbol_table,symbol_type_t* type,const char* identifier);
error_t symbol_table_get_variable(symbol_table_t* symbol_table,variable_t** variable,const char* identifier);
error_t symbol_table_get_function(symbol_table_t* symbol_table,function_t** function,const char* identifier);
error_t symbol_table_get_element(symbol_table_t* symbol_table,element_t** function,const char* identifier);
int symbol_table_get_scope_variables(symbol_table_t* symbol_table,variable_t** variable);
int symbol_table_get_scope_functions(symbol_table_t* symbol_table,function_t** function);
*/

//error_t parse_file(const char* filename,track_t* track);

#endif // PARSER_H_INCLUDED
