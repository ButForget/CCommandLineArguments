# CCommandLineArguments
A project to help developers easily use command line arguments in command line program development.
## 用法
### 示例
_example.c_
``` C
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

```
编译(以gcc为例)

    gcc example.c ccla.c -o example  -O2
  
输入

    .\example foooooooo
    
输出

    [Error] Dont find any character is '=' in: "foooooooo".
    
输入

    \.example foo_1 foo_2 ...
    
输出

    [Error] Dont find any character is '=' in: "foo_1".
    
输入

    .\example --file

输出

    [Error] Dont find any character is '=' in: "--file".
    
输入

    .\example --file=
    
 输出
 
    '\0'
    0
    
输入

    .\example --file=foo --output=foo_2 --run=
    
输出

    foo
    3
    foo_2
    5
    '\0'
    0
    
### 说明

`Args` 是该库的主要结构体,定义如下:

```C
typedef struct
{
    char* buffer;
    size_t arg_size;

    char sepa;
    __Arg* args;
    __Arg* args_end;
    size_t buffer_size;
}Args;

```

其中:

`buffer`是`Args`的缓冲区, 每当解析完一个参数, 参数内容会被放置在该缓冲区中, 同时清除上一次解析所留下的缓冲内容.

`arg_size`是缓冲内容的长度, 可以配合`srtcpy()`或`memcpy()`将缓冲区内容拷贝到其他变量中; 需要注意的是, `arg_size`的单位是`sizeof(char)`而不是*byte*.

剩下的结构体成员一般情况下无需访问.


`Args* ccla_create_args(size_t buffer_size, char sepa)`中:

`buffer_size` 不仅是缓冲区长度, 同时也是命令行参数解析时所能接受的最大长度(不包括参数名称, 即`--foo=`部分).

`sepa`是指定的分隔符, 一般是'=', 如果指定为' ', 将会发生逻辑上的错误.


`int ccla_add_arg(Args* args, const char* name, unsigned int id)`用于添加参数到参数表, `name`和`id`都不能出现重复. 请不要为id赋值为0, 这是为了保证`ccla_get_id的错误码正常抛出.

`unsigned int ccla_get_id(Args* args, const char* name)`用于解析命令行参数, 返回值是先前指定的id, 若出现错误, 返回0, 同时会将参数内容放置于Args中的buffer.

`void ccla_destroy_args(Args* args)`销毁并回收分配的内存资源, `args`会被置为NULL.
