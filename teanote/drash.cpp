/* 多线程
void accept_request(void *arg)
{
    char ch = *((char*)arg);
    printf("新进程");
    for(int i=0; i<10; i++)
    {
        printf("%c %d\n", ch, i);
        sleep(i%5);
    }
    printf("%c Thread Close\n", ch);
    
}

int main()
{
    char ch[11] = "ABCDEFGHIK";
    pthread_t pids[PTHRED_SIZE];

    for(int i=0; i<PTHRED_SIZE; i++)
    {
        printf("创建线程\n");
        if(pthread_create(&pids[i], NULL, (void *)accept_request, (void*)&ch[i]))
            printf("error pthread");
    }
    
    for(int i=0; i<PTHRED_SIZE; i++)
        pthread_join(pids[i], NULL);
    return 0;
}

*/

/* Socket部分
void error_die(const char * msg){
    perror(msg);
    exit(1);
}


int startup(ushort *port)
{
    int on = 1;
    int max_connection = 5;
    // server.sin_family = TCP
    int fd_server = 0; // 先初始化
    fd_server = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in name;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_family = AF_INET;
    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(fd_server, (struct sockaddr*)&name, sizeof(name));
    
    if(*port == 0) // 自主分配端口
    {
        socklen_t sl = sizeof(name);
        *port = getsockname(fd_server, (struct sockaddr*)&name, &sl);
    }
    listen(fd_server, max_connection);
    return fd_server;
}

int readn(int fd, char* buf, int bufsize)
{
    /* read 在不超过指定长度情况下，会一直读取，如果没有达到长度就会阻塞。
    bufsize 需要留一个位置为 '\0'
    
    int count = 0;
    while(count < bufsize-1 && read(fd, buf, 1) > 0)
    {
        if(*buf == '\0')
            break;
        buf++;
        count++;
    }

    if(count == bufsize-1)
        buf[count] = '\0';
    
    return count;
}

int writen(int fd, char *buf, int bufsize)
{
    int count = 0;
    count = write(fd, buf, bufsize);
    return count;
}

int main()
{
    ushort port = 80;
    int fd_server = 0;
    fd_server = startup(&port);
    char buf[1024];
    int bufsize = 1024;
    
    struct sockaddr_in client;
    socklen_t client_l = sizeof(client);
    int fd_client = accept(fd_server, (struct sockaddr*)&client, &client_l);

    int count = 0;
    while(1)
    {
        readn(fd_client, buf, bufsize);
        printf("R.Msg: %s\n", buf);
        if(strcmp(buf, "close") == 0)
            break;
        else
            printf("S.Msg:");

        scanf("%s", buf);
        writen(fd_client, buf, strlen(buf)+1);
        // printf("send ok.");
    }

    close(fd_client);
    close(fd_server);

    return 0;
} */

/* 管道 和 CGI 部分
    int status;
    char *command = "./cacl.sh";

    int shell_input[2];
    int shell_output[2];

    if(pipe(shell_input) == -1)
        error_die("pipe error");
    if(pipe(shell_output) == -1)    
        error_die("pipe error");

    pid_t pid = fork();
    if(pid == -1)
        error_die("fork error");
    
    if(pid == 0) // child
    {  
        dup2(shell_output[1], STDOUT);
        dup2(shell_input[0], STDIN);
        
        close(shell_output[0]);
        close(shell_input[1]);
        
        execl(command, NULL);
        exit(0);
    }
    else // parent
    {
        close(shell_output[1]);
        close(shell_input[0]);
        
        char *a = "100\n";
        
        write(shell_input[1], a, sizeof(a));

        char c;
        while(read(shell_output[0], &c, 1) > 0)
            printf("%c", c);

        close(shell_output[0]);
        close(shell_input[1]);
        waitpid(pid, &status, 0);
    }

*/