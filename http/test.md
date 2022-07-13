# pipe
## 匿名管道 
### **定义**  
&emsp;&emsp;Pipe 是匿名管道，是一种进程间通讯方式，**半双工**。pipe 没有名字只能在近邻进程之间使用，比如父子进程。pipe 在内存中开辟一个缓冲区，不属于文件系统，不同操作系统的管道缓冲区大小不同。**pipe 的读写只能在一个方向上流动**，写入后的数据遵循先入先出原则，管道中的数据是一次性的，**读取即被抛弃**。

### **理解**  
&emsp;&emsp;管道是一种特殊的文件，在应用层表现为一组文件描述符（fd[2], fd[0] read，fd[1] write）。读和写只能打开一个口，不能同时打开或者使用。正常情况下，读写过程存在**阻塞**。
<im src="https://img-blog.csdnimg.cn/img_convert/cb2fa8fb05dc06457683ef951e5f88de.png" />

---
## Pipe 和进程  
### **代码实例**：
`CodeTea/CPP/pipe_easy/pipe.c`
```cpp
int main()
{
    int status;
    char *command = "./cacl.sh";

    int shell_input[2]; // 输入管道
    int shell_output[2]; // 输出管道

    pipe(shell_input);    
    pipe(shell_output);

    pid_t pid = fork();
    
    if(pid == 0) // child
    {  
        //重定向
        dup2(shell_output[1], STDOUT); 
        dup2(shell_input[0], STDIN);
        
        close(shell_output[0]); // 单工
        close(shell_input[1]);
        
        execl(command, NULL); // 执行脚本
        exit(0);
    }
    else // parent
    {
        close(shell_output[1]); // 单工
        close(shell_input[0]);
        
        char *a = "100\n"; // 注意这个回车
        
        write(shell_input[1], a, sizeof(a)); // 向脚本中输入

        char c; // 从脚本中输出
        while(read(shell_output[0], &c, 1) > 0)
            printf("%c", c);

        close(shell_output[0]);
        close(shell_input[1]);
        waitpid(pid, &status, 0);
    }
    return 0;
}
```
### **重定向的图示**
pass

---
## Pipe 和线程  
*等待补充*

---
## 参考
> 【1】Pipe https://blog.csdn.net/weixin_34430692/article/details/113453811

---
## KG-Infomation
```yaml
{
    Key: Pipe
    Key: 管道
    key: 匿名管道
    Time: 2022/4/7
    Time: 2022/4/9
}
```



```python
# 测试

```