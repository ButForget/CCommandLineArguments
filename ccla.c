#include "ccla.h"
#include <string.h>
#include <stdbool.h>

FILE* ccla_log;

static __Arg* __get_arg(Args* args, const char* name);
static bool __whether_arg(Args* args, const char* name, unsigned int id);

static bool __whether_arg(Args* args, const char* name, unsigned int id)
{
    __Arg* current = args->args;
    
    while(current != NULL)
    {
        if(current->id == id || strcmp(name, current->name) == 0) break;
        current = current->next;
    }

    return current == NULL? false: true;
}
static __Arg* __get_arg(Args* args, const char* name)
{
    __Arg* current = args->args;

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

Args* ccla_create_args(size_t buffer_size, char sepa)
{
    Args* args        = (Args*)malloc(sizeof(Args));
    args->args        = NULL;
    args->args_end    = NULL;
    args->buffer      = (char*)malloc(buffer_size * sizeof(char));
    args->buffer_size = buffer_size;
    args->arg_size    = 0;
    args->sepa        = sepa;
    ccla_log          = stderr;
    return args;
}

int ccla_add_arg(Args* args, const char* name, unsigned int id)
{
    if(args == NULL)
    {
        fprintf(ccla_log, "[Error] Args is NULL.\n");
        return 1;
    }
    if(name == NULL)
    {
        fprintf(ccla_log, "[Error] Args name is NULL.\n");
        return 1;
    }
    if(__whether_arg(args, name, id) == true)
    {
        fprintf(ccla_log, "[Warn] Argument:%s or its id:%u has been defined.", name, id);
        return 1;
    }

    __Arg* new = (__Arg*)malloc(sizeof(__Arg));
    if(new == NULL)
    {
        fprintf(ccla_log, "[Error] Alloc memory error.\n");
        return 1;
    }

    if(args->args == NULL)
        args->args = new;
    if(args->args_end != NULL)
        args->args_end->next = new;
    args->args_end       = new;
    new->found           = false;
    new->id              = id;
    new->next            = NULL;
    strcpy(new->name, name);

    return 0;
}

unsigned int ccla_get_id(Args* args, const char* name)
{
    if(args == NULL)
    {
        fprintf(ccla_log, "[Error] Args is NULL.\n");
        return -1;
    }

    if(name == NULL)
    {
        fprintf(ccla_log, "[Error] Args name is NULL.\n");
        return -1;
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
        fprintf(ccla_log, "[Error] Invalid argument name:\"%s\".\n", _name);
        return -1;
    }

    __Arg* current = __get_arg(args, _name);
    if(current == NULL)
    {
        fprintf(ccla_log, "[Error] Invalid argument name:\"%s\".\n", _name);
        return -1;
    }

    memset(args->buffer, 0, args->buffer_size);

    strcpy(args->buffer, name + i + 1);
    args->arg_size = strlen(name + i + 1);
    current->found = true;

    return current->id;
}

void ccla_destroy_args(Args* args)
{
    __Arg* entry;
    for(__Arg** current = &args->args; *current != NULL; current = &entry->next)
    {
        entry = *current;
        free(entry);
    }
    free(args->buffer);
}