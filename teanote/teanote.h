#ifndef __TEANOTE__
#define __TEANOTE__
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <regex>
#include <vector>
#include <map>
#include <ctime>
#include <cstdio>
#include <sys/stat.h>

using namespace std;

typedef struct {
    int rank; // 标题等级
    string title; // 标题名称
    string content; // 内容
    void _init();
} Section;

class TeaNote
{
private:
    string idx;
    string path;
    string globalContext;
    vector<Section> sections; // md 格式
    vector<struct tm> modifys;

public:
    TeaNote();
    TeaNote(const string&);
    void parseTeaMd();
    
    void createPattern_Md(const string& , const string&);
    
    string showMardDown();
    string showHtml();
};


class Caddyn
{
private:
    map<string, TeaNote> notes;
    void getNotesFromYaml(ifstream &f);
    
public:
    Caddyn(const string&);
    bool fileIsExist(const string& idx);
    string getNotes(const string& idx, string form="MarkDown");
    void createNewNote();
};

#endif