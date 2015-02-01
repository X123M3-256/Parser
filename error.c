#include <stdio.h>
#include "error.h"

error_t error_none={"No error. Why are you seeing this\n",0,0};

void print_error(error_t error)
{
printf("Error: %s at line %d, char %d\n",error.message,error.line_number,error.char_number);
}
