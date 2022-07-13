#include "http.h"
#include "../utils/_string.h"

/* 复刻 TinyHttp 
1. fork、 pipi  和 shell 进行交互
2. 套接字部分 read
3. 多线程部分
4. 重写一个 TinyHttp, 更改为 cpp 文件, 整理结构
5. 编写 JS 交互 ,  CSS 渲染, Post交互
6. 重新组织

*/

using tangbao::HttpDataPacket;
using tangbao::HttpDataPacket_Request;
using tangbao::HttpDataPacket_Respone;
using tangbao::TCPSocketConnection;

int HttpDataPacket::getContentLength()
{
    if(this->mss_headers.count("ContentLength") == 1)
        return stoi(this->mss_headers["ContentLength"].c_str());
    else
        return -1;
}

void HttpDataPacket_Request::setRequestLine(string line) 
{
    vector<string> vec_str = tangbao::splitString(line, " ");
    setMethod(vec_str[0]);
    setURL(vec_str[1]);
    setVersion(vec_str[2]);
}

void HttpDataPacket_Respone::build(string contentType, string content)
{
    clearContent();
    setContent(content);

    setHeader_ContentLength();
    setHeader_ContentType(contentType);  
    setHeader_Date();   
}

void HttpDataPacket_Respone::setHeader_ContentType(const string& type)
{
    this->mss_headers[string("Content-Type")] = string(type) + string("; charset=utf-8");
}

void HttpDataPacket_Respone::setHeader_Date() {

    this->mss_headers[string("Date")] = string("Wed, 08 Apr 2022 11:29:22 GMT");
}

void HttpDataPacket_Respone::setHeader_ContentLength()
{
    this->mss_headers["Content-Length"] =  to_string(this->s_content.size());
}

string HttpDataPacket_Respone::toPacket() {

    string msg;
    msg = this->s_version + " " + to_string(this->i_stateCode) + "\r\n";
    for(const auto& it : mss_headers)
        msg += it.first + ": " + it.second + "\r\n";

    msg += "\r\n"; // 中间行

    msg += s_content;
    return msg;
}

TCPSocketConnection::TCPSocketConnection(int fd, pthread_t pid)
{
    this->i_fd = fd;
    this->pt_id = pid;
}

int TCPSocketConnection::getFd()
{
    return this->i_fd;
}

pthread_t* TCPSocketConnection::getPidPtr()
{
    return &this->pt_id;
}

int tangbao::init_tcp(ushort* port)
{
    int fd_server = socket(PF_INET, SOCK_STREAM, 0);
    if(fd_server < 0)
        Log::error(__FILE__, __LINE__, "Socket Create Error!");
    
    struct sockaddr_in name;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = INADDR_ANY;
    name.sin_family = AF_INET;
    
    int on = 1;
    setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(fd_server, (struct sockaddr*)&name, sizeof(name));

    if(*port == 0)
    {
        socklen_t name_l = sizeof(name);
        *port = getsockname(fd_server, (struct sockaddr*)&name, &name_l);
    }
    listen(fd_server, 10);
    Log::sese(__FILE__, __LINE__, "ServerSocket OK.");
    return fd_server;
}

int tangbao::read_line(int fd, string& msg,  int readStep, int readSize)
{
    /*读取一行 \n\0结束 \r 继续*/
    char ch= 'A';
    msg.clear();

    if(readSize <= 0)
        return 0;
    
    while(readSize-- && read(fd, &ch, readStep) > 0)
    {
        if(ch == '\n')
        {
            msg.push_back(ch);
            break;
        }
        else if(ch == '\r')
            continue;
        else 
            msg.push_back(ch);
    }
    
    Log::recvLog(__FILE__, __LINE__, msg);
    
    return msg.size();
}

void tangbao::send_httpacket(int fd, const string& msg)
{
    // Log::sese(__FILE__, __LINE__, "msg - " + msg + "- msg");
    write(fd, msg.c_str(), msg.size());
}

void tangbao::handle_POST()
{
    
}

void tangbao::read_httpPacket(int fd, HttpDataPacket_Request* request)
{
    string buffer;
    int readStep = 1;
    int readSize = 1024;

    int flag = 1;
    while(flag > 0 && tangbao::read_line(fd, buffer, readStep, readSize) > 0)
    {
        switch(flag)
        {
            case 0:
                return;
            case 1: // 读取包头
            {
                request->setRequestLine(buffer);
                

                if(strcasecmp(request->getMethod().c_str(), "GET") == 0)
                    flag = 2;
                else if(strcasecmp(request->getMethod().c_str(), "POST") == 0) 
                    flag = 4;
                else
                    Log::error(__FILE__, __LINE__, "Not GET or Not POST");
                break;
            }
            case 2: // 读取 Get 包头关键字
            {
                if(buffer == "\n") {
                    flag = 3;
                    readSize = 0;
                    break;
                }
                
                vector<string> strs2 = tangbao::splitString(buffer, ":");
                request->setHeader(strs2[0], strs2[1]);
                break;
            }

            case 3:
                // Warn. 这里可能需要额外读两个字符
                Log::warn(__FILE__, __LINE__,  "Read Get Header Over. Http Parse Over.");
                flag = 0;
                break;  

            case 4: // 读取 POST 包头关键字, 暂时忽略关键字
            {
                if(buffer == "\n") {
                    flag = 5;
                    break;
                }
                vector<string> strs3 = tangbao::splitString(buffer, ":");
                request->setHeader(strs3[0], strs3[1]);
                break;
            }
            
            case 5: 
                // 100-continue
                Log::warn(__FILE__, __LINE__,  "Wait Send 100-continue");
                readSize = request->getContentLength();
                flag = 0;
                break;
            default:
                Log::error(__FILE__, __LINE__, "Pack Error.");
        }
    }
}

void tangbao::handle_GET(TCPSocketConnection *connection, HttpDataPacket_Request *request)
{
    Log::sese(__FILE__, __LINE__, "handle_GET.");
    
    // 资源定位文件 url2file
    ifstream urlToFile;
    urlToFile.open("/root/code/Tea/conf/url2file");

    string line;
    string desPath;
    while(getline(urlToFile, line)) {
        vector<string> result = tangbao::splitString(line, " # ");
        if(result.size() != 0 && result[0] == request->getURL()) {
            desPath = result[1];
            break;
        }
    }
    Log::warn(__FILE__, __LINE__, "desPath = " + desPath);
    
    HttpDataPacket_Respone* respone;
    if(desPath.size() == 0)
        respone = new HttpDataPacket_Respone(404);
    else
        respone = new HttpDataPacket_Respone(200);

    string contentType;
    if(desPath.find(".css") != string::npos)
        contentType = "text/css";
    if(desPath.find(".html") != string::npos)
        contentType = "text/html";    

    if(desPath.find("favicon.ico") != string::npos)
        return;

    string content;
    if(desPath.find("*.png") != string::npos) {
        contentType = "image/png";
        FILE *fp = fopen(desPath.c_str(), "rb");
        char readBuf[4096];
        int readLen = fread(readBuf, sizeof(char), 4096, fp);
        
        content =  string(readBuf, 0, readLen);
    }
    else {
        ifstream file;
        file.open(desPath.c_str());
        line.clear();
        content.clear();
        while(getline(file, line))
            content += line + "\n";
        
    }
    
    respone->build(contentType, content);
    

    send_httpacket(connection->getFd(), respone->toPacket());
    free(respone);
    Log::sese(__FILE__, __LINE__, "Success Find File. Send Respone.");
}

void* tangbao::handle_request(void *arg)
{
    /* 处理收到的信息
    如果收到的信息不符合规范，会造成 read() 阻塞 */
    Log::sese(__FILE__, __LINE__, "Accept_Request Pthread Run.");
    
    TCPSocketConnection *connection = (TCPSocketConnection*)arg;

    HttpDataPacket_Request request;  
    // 读取 Http Package
    read_httpPacket(connection->getFd(), &request);

    if(strcasecmp(request.getMethod().c_str(), "GET") == 0)
        tangbao::handle_GET(connection, &request);
    else if(strcasecmp(request.getMethod().c_str(), "POST") == 0)
        tangbao::handle_POST();

    close(connection->getFd());
    Log::sese(__FILE__, __LINE__, "Close TCP. Close pthread.");

    free(connection);
    connection = NULL;
    return NULL;
}

int tangbao::run_http()
{
    Log::sese(__FILE__, __LINE__, "Http Start.");
    
    ushort port = 80;
    int fd_server = init_tcp(&port);
    struct sockaddr_in name;
    socklen_t name_l = sizeof(name);
    while(1)
    {
        Log::sese(__FILE__, __LINE__, "Wait Connection.");

        int fd = accept(fd_server, (struct sockaddr*)&name, &name_l);
        if(fd == -1)
            Log::error(__FILE__, __LINE__, "Socket Open Fail.");
            
        TCPSocketConnection *connection = new TCPSocketConnection(fd, 0);
        
        if(pthread_create(connection->getPidPtr(), NULL, tangbao::handle_request, (void*)connection) != 0)
            Log::error(__FILE__, __LINE__, "Create Pthread Fail.");
    }
    return 0;
}
