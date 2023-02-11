# CCommandLineArguments
A project to help developers easily use command line arguments in command line program development.
## 用法
### 示例
_example.c_
``` C
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
```
*powershell*
    
    > gcc example.c ccla.c -o example  -O2
    
    >.\example foooooooo
    [Error] Dont find any character is '=' in: "foooooooo".
    
    >.\example foo_1 foo_2 ...
    [Error] Dont find any character is '=' in: "foo_1".
    
    >.\example --file
    [Error] Dont find any character is '=' in: "--file".
    
    >.\example --file=
    '\0'
    0
    
    >.\example --file=foo --output=foo_2 --run=
    foo
    3
    foo_2
    5
    '\0'
    0
### 说明
#### 结构体
`Args` 是该库的主要结构体,定义如下:

```C
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

```  
</br>
其中:

`buffer`是`Args`的缓冲区, 每当解析完一个参数, 参数内容会被放置在该缓冲区中, 同时清除上一次解析所留下的缓冲内容.

`arg_size`是缓冲内容的长度, 可以配合`srtcpy()`或`memcpy()`将缓冲区内容拷贝到其他变量中; 需要注意的是, `arg_size`的单位是`sizeof(char)`而不是*byte*.

剩下的结构体成员一般情况下无需访问.</br></br>

#### 函数
- `Args* ccla_create_args(const char* format);`:
我们使用格式字符串初始化参数表. 其使用方法如下:
    - 以空格为分隔符, 字符串开头的所有空格都会被忽略, 参数之间超过一个的空格会被忽略.
    - *待续*

- `unsigned int ccla_get_id(Args* args, const char* name)`用于解析命令行参数, 返回值是先前指定的id, 若出现错误, 返回0, 同时会将参数内容放置于Args中的buffer.

- `void ccla_destroy_args(Args* args)`销毁并回收分配的内存资源, `args`会被置为NULL.

### 其他

使用config系列函数设置参数表.
```C
int ccla_config_log(Args* args, FILE* log);
//错误日志输出文件流

int ccla_config_output(Args* args, FILE* output);
//帮助信息输出文件流（未启用）

int ccla_config_buffer_size(Args* args, size_t buffer_size);
//参数缓冲区大小

int ccla_config_sepa(Args* args, char sepa);
//分隔符设置
```
## TODO
- [ ] 添加注释.
- [ ] 完善错误处理.
- [ ] 支持子命令.
- [ ] 支持别名.
- [ ] 支持使用空格作为分隔符.

## 更新日志
- 2023/2/11: 简化使用流程, 现在使用格式字符串创建参数表, 不再使用大量的`ccla_add_arg`;后者已被弃用.
- 2023/2/10: 更新README.md, 简化创建参数表流程(未提交).
- 2023/2/6: 简化'ccla_create_args'用法, 现在buffer_size默认给出为256, sepa 默认为'='. 可以使用config系列函数修改它们.
- 2023/2/5: 创建仓库.

## License
BSD 3-Clause License
>
>BSD 3-Clause License
>
>Copyright (c) 2023, CaKEntity
>Redistribution and use in source and binary forms, with or without
>modification, are permitted provided that the following conditions are met:
>
>1. Redistributions of source code must retain the above copyright notice, this
>   list of conditions and the following disclaimer.
>
>2. Redistributions in binary form must reproduce the above copyright notice,
>   this list of conditions and the following disclaimer in the documentation
>   and/or other materials provided with the distribution.
>
>3. Neither the name of the copyright holder nor the names of its
>   contributors may be used to endorse or promote products derived from
>   this software without specific prior written permission.
>
>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
