#ifndef __LOG__
#define __LOG__
#include <iostream>
using namespace std;

namespace tangbao
{
    class Log 
    {
        public:
            static void sese(string fileName, int line, string infoMsg);     
            static void error(string fileName, int line, string errMsg);
            static void recvLog(string fileName, int line, string errMsg);
            static void warn(string fileName, int line, string warnMsg);
            Log();
    };


} // namespace tangbao


#endif