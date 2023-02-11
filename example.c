#include "ccla.h"

int main(int argc, char** argv)
{
    Args* args = ccla_create_args("--file --output --run");

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
