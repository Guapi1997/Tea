#include "arg.h"


// 指令解析代码 C语言
// author: tangbao
// date: 2022/4/12

int run_argv_v1(int argc, char* argv[], Arg *args, int argsSize)
{
    /* C. 参数解析
        argc:
        argv:
        args: Arg 数组
        argsSize: args 数组大小
        Arg Struct:
            cmd 命令
            parm 参数
        0 操作目标
        1-N 操作参数 参数值
        main -Rhtml file 
    */
    if(argc <= 1)
        return -1;
    
    strcpy(args[0].cmd, "DEST");
    strcpy(args[0].parm, argv[argc-1]);

    int index = 1;
    for(int i=1; i<argc-1; i++)
    {
        if(index >= argsSize)
            return -1;
        if(argv[i][0] == '-')
        {
            strncpy(args[index].cmd, argv[i]+1, 1);
            if(strlen(argv[i]) > 2)
                strncpy(args[index].parm, argv[i]+2, strlen(argv[i])-2);
            // printf("# %s %s\n",args[index].cmd, args[index].parm);
            index++;
        }
        else
            return -1;    
    }

    return index;
}
