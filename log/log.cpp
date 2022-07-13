#include "log.h"

tangbao::Log::Log() 
{

}

void tangbao::Log::sese(string fileName, int line, string infoMsg)
{
    cout << fileName << ":" << line << " Info.  " << infoMsg << endl;
}


void tangbao::Log::error(string fileName, int line, string errorTip)
{
    cout << fileName << ":" << line << " Error.  " << errorTip << endl;
    exit(1);
}


void tangbao::Log::recvLog(string fileName, int line, string revMsg)
{
    cout << fileName << ":" << line << " Recv_Line. ##" << revMsg << "##" << endl;
    
}

void tangbao::Log::warn(string fileName, int line, string warnMsg)
{
    cout << fileName << ":" << line << " Warn. " << warnMsg << endl;
}
