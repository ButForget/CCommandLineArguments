#ifndef __C_COMMAND_LINE_ARGUMENTS__
#define __C_COMMAND_LINE_ARGUMENTS__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ARG_NAME_LENGTH 32
 
extern FILE* ccla_log; 

typedef struct __Arg
{
    char name[ARG_NAME_LENGTH];
    unsigned int id;
    bool found;
    struct __Arg* next;
}__Arg;

typedef struct
{
    __Arg* args;
    __Arg* args_end;
    char sepa;
    char* buffer;
    size_t buffer_size;
    size_t arg_size;
}Args;

Args* ccla_create_args(size_t arg_size, char sepa);
int ccla_add_arg(Args* args, const char* name, unsigned int id);
void ccla_destroy_args(Args* args);
unsigned int ccla_get_id(Args* args, const char* name);


#ifdef __cplusplus
}
#endif

#endif