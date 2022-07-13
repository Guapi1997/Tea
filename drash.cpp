void show_help()
{
    cout << "TeaNote 轻型笔记管理工具\n"
            "命令使用如下:\n"
            "tea [-Pvalue] <file>"
            "tea -D: 创建一个默认模板的笔记\n"
            "tea -U: 更新笔记文档\n"
            "tea -L: 列出笔记树\n"
            "tea -H: Help\n"
            "tea -A : 将文件 <file> 加入笔记树\n"
            "tea -I : 显示一个笔记的信息\n"
            "tea -H : 渲染一个笔记\n"
            "tea -W : 显示一个笔记\n" << endl;

}

void opera_createPattern()
{
    Log::sese(__FILE__, __LINE__, "opera_createPattern");
    Caddyn caddyn("/root/code/Tinyhttpd/CodeTea/caddyn.yaml");
}

void handle_teanote(string op)
{
    if(op == "create")
    {
        opera_createPattern();
        Log::sese(__FILE__, __LINE__, "文件创建成功.");
    }

}

void test()
{
    Caddyn caddyn("/root/code/Tinyhttpd/CodeTea/caddyn.yaml");
    caddyn.getNotes("TeaNote_001");
    exit(1);
}


void goto_task(Arg *args, int argsLen)
{
    for(int i=0; i<argsLen; i++)
    {
        switch(args[i].cmd[0])
        {
            case 'S': // 打开服务器
                handle_server(args[i].parm);
                break;
            case 'T': // TeaNote
                handle_teanote(args[i].parm);
        }   
    }
}

void opera_openServer()
{
    run_http();
}
// void handle_100C(SockVisitor *visitor, HttpRequest *request)
// {
//     HttpRespone respone;
//     respone._init();
//     respone.version = (char*)malloc(sizeof(char)*10);
//     sprintf(respone.version, "HTTP/1.1");
//     respone.stateCode = (char*)malloc(sizeof(char)*10);
//     sprintf(respone.stateCode, "100 Continue");
//     respone.expect = (char*)malloc(sizeof(char)*50);
//     sprintf(respone.expect, "Expect: 100-continue");
//     respone.contentLength = (char*)malloc(sizeof(char)*20);
//     sprintf(respone.contentLength, "Content-Length: 0");
//     send_respone(visitor, &respone);
//     respone._free();
//     info("Success Send 100 Continue.");
// }

// if(desPath.find(".md") != string::npos)
    //     type = "text/html";
    // if(desPath.find("TeaNote_") != string::npos)
    // {
    //     // TODO: 需要该帧
    //     type = "text/html";
    //     Caddyn caddyn("/root/code/Tinyhttpd/CodeTea/caddyn.yaml");
    //     if(caddyn.fileIsExist(desPath))
    //         respone.content = caddyn.getNotes(desPath, "HTML");
    //     else {
    //         cout << "Info. File Not Exist." << endl;
    //     }
    // }

    //读取需要的文件
