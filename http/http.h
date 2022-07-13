#ifndef __FACSIMILE__
#define __FACSIMILE__
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/file.h>
#include <string.h>

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "../log/log.h"
using namespace std;
using tangbao::Log;

#define STDIN   0
#define STDOUT  1
#define STDERR  2
#define PTHRED_SIZE 3

namespace tangbao {

class HttpDataPacket {
protected:
    string s_version;
    map<string, string> mss_headers;
    string s_content;

public:
    HttpDataPacket(string version = "HTTP/1.1") { s_version = version; }
    virtual int getContentLength();
};

class HttpDataPacket_Request: public HttpDataPacket {
private:
    string s_method;
    string s_url;

public:
    HttpDataPacket_Request() {}
    void setRequestLine(string line);
    void setMethod(string method) { this->s_method = method; }
    void setURL(string url) { this->s_url = url; }
    void setVersion(string version) { this->s_version = version; }
    string getMethod() { return this->s_method; }
    string getURL() { return this->s_url; }
    void setHeader(string key, string value) { this->mss_headers[key] = value; }

};

class HttpDataPacket_Respone: public HttpDataPacket {
private:
    int i_stateCode;

public:
    HttpDataPacket_Respone(int stateCode) { this->i_stateCode = stateCode; }
    
    void setHeader_Date();
    void setHeader_ContentType(const string& type);
    void setHeader_ContentLength();
    void clearContent() { this->s_content.clear(); }
    void build(string contentType, string content);
    string toPacket();
    void setContent(string content) { this->s_content = content; }
};

class TCPSocketConnection {
private:
    int i_fd;
    pthread_t pt_id;

public:
    TCPSocketConnection(int fd, pthread_t pid);
    int getFd();
    pthread_t* getPidPtr();

};

int run_http();

int init_tcp(ushort* port);

void* handle_request(void *arg);

void read_httpPacket(int fd, HttpDataPacket_Request* request);

int read_line(int fd, string& msg,  int readStep, int readSize);

void handle_GET(TCPSocketConnection *visitor, HttpDataPacket_Request *request);

void handle_POST();

void send_httpacket(int fd, const string& msg);

}
#endif