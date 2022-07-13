#include "_string.h"


vector<string> tangbao::splitString(const string& str, const string& pattern)
{
    int index = 0;
    int pos = 0;

    vector<string> vecStr;
    while(index < str.size() && (pos = str.find(pattern, index)) != string::npos )
    {
        vecStr.push_back( string(str, index, pos-index) );
        index = pos + pattern.size();
    }
    
    vecStr.push_back(string(str, index, str.size())); 

    return vecStr;
}
