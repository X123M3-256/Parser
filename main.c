#include <stdio.h>
#include <stdlib.h>
#include "parser.h"



int main()
{
lexer_t lexer;
lexer_create(&lexer,"test.txt");
expression_t* expression;
error_t error=parse_expression(&lexer,&expression);
print_error(error);
return 0;
}
