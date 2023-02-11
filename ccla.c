#include "ccla.h"
#include <string.h>
#include <stdbool.h>
#define CCLA_BUFFER_SIZE 256
#define CCLA_SEPA '='
static __Arg* __get_arg(Args* args, const char* name);
static bool __whether_arg(Args* args, const char* name, char short_name, unsigned int id);
static __Arg* __add_arg(Args* args, const char* name, const char* note, char short_name,unsigned int id);
static int __init_args(Args* args, const char* format);

static __Arg* __add_arg(Args* args, const char* name, const char* note, char short_name,unsigned int id)
{
    if(args == NULL)
    {
        fprintf(args->__log, "[Error] Args is NULL.\n");
        return NULL;
    }
    if(name == NULL)
    {
        fprintf(args->__log, "[Error] Args name is NULL.\n");
        return NULL;
    }
    if(__whether_arg(args, name, short_name, id) == true)
    {
        fprintf(args->__log, "[Warn] Argument:%s or its id:%u has been defined.\n", name, id);
        return NULL;
    }

    __Arg* new = (__Arg*)malloc(sizeof(__Arg));
    if(new == NULL)
    {
        fprintf(args->__log, "[Error] Alloc memory error.\n");
        return NULL;
    }

    if(args->__args == NULL)
        args->__args = new;
    if(args->__args_end != NULL)
        args->__args_end->next = new;
    args->__args_end       = new;

    new->found      = false;
    new->id         = id;
    new->next       = NULL;
    new->short_name = short_name;
    strcpy(new->name, name);
    strcpy(new->note, note);

    return new;
}

static int __init_args(Args* args, const char* format)
{

    int  arg_count                    = 0;
    int  ch_count                     = 0;
    int  state                        = 0;
    char short_name                   = '\0';
    char name_buffer[ARG_NAME_LENGTH] = { 0 };
    char note_buffer[ARG_NOTE_LENGTH] = { 0 };

    __Arg* current_arg = NULL;
    char*  current_stream = name_buffer;
    
    for(int i = 0; format[i] != '\0'; ++i)
    {
        switch(format[i])
        {
            case ':':
                short_name = format[++i];
                break;
            case '#':
                current_stream = note_buffer;
                state = 1;
                ch_count = 0;
                break;
            case ' ':
                if(ch_count == 0) break;
                current_arg = __add_arg(args, name_buffer, note_buffer, short_name, ++arg_count);
                memset(name_buffer, 0, ARG_NAME_LENGTH * sizeof(char));
                memset(note_buffer, 0, ARG_NOTE_LENGTH * sizeof(char));
                current_stream = name_buffer;
                short_name     = '\0';
                ch_count       = 0;
                state          = 0;
                break;
            default:
                if(ch_count > (state == 0? ARG_NAME_LENGTH: ARG_NOTE_LENGTH))
                    goto error_length;
                current_stream[ch_count++] = format[i];
        }
    }
    __add_arg(args, name_buffer, note_buffer, short_name, ++arg_count);
    return 0;

    error_length:
    fprintf(args->__log, "[Error] The Argument length is too long.\n");
    error:
    ccla_destroy_args(args);
    return -1;
}

Args* ccla_create_args(const char* format)
{
    Args* args       = (Args*)malloc(sizeof(Args));
    if(args == NULL)
        return NULL;

    args->__args        = NULL;
    args->__args_end    = NULL;
    args->buffer        = (char*)malloc(CCLA_BUFFER_SIZE * sizeof(char));
    args->__buffer_size = CCLA_BUFFER_SIZE;
    args->arg_size      = 0;
    args->sepa          = CCLA_SEPA;
    args->__log         = stderr;
    args->__output      = stdout;
    if(__init_args(args, format) == 0)
        return args;
    free(args);
    return NULL;
}


static bool __whether_arg(Args* args, const char* name, char short_name, unsigned int id)
{
    __Arg* current = args->__args;

    while(current != NULL)
    {
        if
        (
            current->id == id
            || strcmp(name, current->name) == 0
            || (current->short_name == short_name && current ->short_name != '\0')
        ) break;
        current = current->next;
    }

    return current == NULL? false: true;
}
static __Arg* __get_arg(Args* args, const char* name)
{
    __Arg* current = args->__args;

    while(current != NULL)
    {
        if(current->found == true)
        {
            current = current->next;
            continue;
        }
        if(strcmp(current->name, name) == 0) break;
        current = current->next;
    }
    return current == NULL? NULL: current;
}


int ccla_config_log(Args* args, FILE* log)
{
    if(args == NULL)
        return -1;
    args->__log = log;
    
    return 0;
}

int ccla_config_output(Args* args, FILE* output)
{
    if(args == NULL)
        return -1;
    args->__output = output;

    return 0;
}

int ccla_config_buffer_size(Args* args, size_t buffer_size)
{
    if(args == NULL)
        return -1;
    free(args->buffer);

    args->__buffer_size = buffer_size;
    args->buffer = (char*)malloc(buffer_size * sizeof(char));
    if(args->buffer == NULL)
        return -1;
    
    return 0;
}

int ccla_config_sepa(Args* args, char sepa)
{
    if(args == NULL)
        return -1;
    args->sepa = sepa;
    
    return 0;
}

unsigned int ccla_get_id(Args* args, const char* name)
{
    if(args == NULL)
    {
        fprintf(args->__log, "[Error] Args is NULL.\n");
        return 0;
    }

    if(name == NULL)
    {
        fprintf(args->__log, "[Error] Args name is NULL.\n");
        return 0;
    }

    char _name[ARG_NAME_LENGTH] = { 0 };
    int i = 0;
    for(; name[i] != '\0' && i <= ARG_NAME_LENGTH; ++i)
    {
        if(name[i] != args->sepa)
            _name[i] = name[i];
        else break; 
    }

    if(name[i] == '\0')
    {
        fprintf(args->__log, "[Error] Dont find any character is '%c' in: \"%s\".\n", args->sepa, name);
        return 0;
    }

    __Arg* current = __get_arg(args, _name);
    if(current == NULL)
    {
        fprintf(args->__log, "[Error] Invalid argument name:\"%s\".\n", _name);
        return 0;
    }

    memset(args->buffer, 0, args->__buffer_size);

    strcpy(args->buffer, name + i + 1);
    args->arg_size = strlen(name + i + 1);
    current->found = true;

    return current->id;
}

void ccla_destroy_args(Args* args)
{
    __Arg* entry;
    for(__Arg** current = &args->__args; *current != NULL; current = &entry->next)
    {
        entry = *current;
        free(entry);
    }
    free(args->buffer);
    args = NULL;
}