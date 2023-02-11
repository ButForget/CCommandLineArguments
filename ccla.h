#ifndef __C_COMMAND_LINE_ARGUMENTS__
#define __C_COMMAND_LINE_ARGUMENTS__

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define ARG_NAME_LENGTH 32
#define ARG_NOTE_LENGTH 128
typedef struct __Arg
{
    char name[ARG_NAME_LENGTH];
    char short_name;
    char note[ARG_NOTE_LENGTH];
    unsigned int id;
    bool found;
    struct __Arg* next;
}__Arg;

typedef struct
{
    /* public */
    char sepa;
    char* buffer;
    size_t arg_size;

    /* private */
    __Arg* __args;
    __Arg* __args_end;
    size_t __buffer_size;
    FILE* __log;
    FILE* __output;
}Args;

/* normal-function*/
Args* ccla_create_args(const char* format);
void ccla_destroy_args(Args* args);
unsigned int ccla_get_id(Args* args, const char* name);

/* config-args-functions */
int ccla_config_log(Args* args, FILE* log);
int ccla_config_output(Args* args, FILE* output);
int ccla_config_buffer_size(Args* args, size_t buffer_size);
int ccla_config_sepa(Args* args, char sepa);

#ifdef __cplusplus
}
#endif

#endif