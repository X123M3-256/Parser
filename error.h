#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#define IS_ERROR(err) ((err).line_number!=0)

typedef struct
{
const char* message;
unsigned short char_number;
unsigned short line_number;
}error_t;

extern error_t error_none;

void print_error(error_t error);

#endif // ERROR_H_INCLUDED
