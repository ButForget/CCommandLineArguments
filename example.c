#include "ccla.h"

int main(int argc, char** argv)
{
    Args* args = ccla_create_args(256, '=');
    ccla_add_arg(args, "--file", 1);
    ccla_add_arg(args, "--output", 2);
    ccla_add_arg(args, "--run", 3);

    while(--argc)
    {
        switch(ccla_get_id(args, *(++argv)))
        {
            case 1:
                printf("%s\n%zu\n", args->buffer, args->arg_size);
                break;
            case 2:
                printf("%s\n%zu\n", args->buffer, args->arg_size);
                break;
            case 3:
                printf("%s\n%zu\n", args->buffer, args->arg_size);
                break;
            default: goto error;
        }
    }

    ccla_destroy_args(args);
    return 0;
    error:
    ccla_destroy_args(args);
    return 0;
}
