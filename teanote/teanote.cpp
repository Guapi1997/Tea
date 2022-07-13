#include "teanote.h"

void Section::_init()
{
    this->rank = 0;
    this->content.clear();
    this->title.clear();
}

Caddyn::Caddyn(const string& path)
{
    ifstream f(path);
    if(!f.good())
    {
        cout << "Error: " << path << " No Exist!" << endl;
        exit(1);
    }

    getNotesFromYaml(f);
}

void Caddyn::getNotesFromYaml(ifstream &f)
{
    cout << "Info. getNotesFromYaml" << endl;
    string line;
    while(getline(f, line) && line != "Caddyn:") ;

    int state = 0;
    string t4 = "    ";
    string t8 = "        ";
    string key;
    
    while(getline(f, line))
    {
        // cout << line << endl;
        if(state == 0 && line.substr(0, 4) == t4 &&  line.substr(0, 8) != t8) // Key
        {
            state = 1;
            key = line.substr(4, line.size()-4);
        }
        else if(state == 1 && line.substr(0, 8) == t8) // value
        {
            string value = line.substr(14, line.size()-14);
            this->notes[key] = TeaNote(value);
            state = 0;
        }
        else
        {
            cout << "Yaml 文件读取结束" << endl;
            break;
        }
    }
}

bool Caddyn::fileIsExist(const string& idx)
{
    return this->notes.count(idx) != 0;
}

string Caddyn::getNotes(const string& idx, string form)
{
    if(fileIsExist(idx) == false)
    {
        cout << "Error. idx not exist" << endl;
        exit(1);
    }

    if(form == "HTML")
        return this->notes[idx].showHtml();
    return this->notes[idx].showMardDown();
}

void Caddyn::createNewNote()
{
    
}


TeaNote::TeaNote()
{

}

TeaNote::TeaNote(const string& notePath)
{
    ifstream f(notePath, ios::in);
    if(f.is_open() != true)
    {
        cout << "Error. note path error." << endl;
        exit(1);
    }

    string line;
    while(getline(f, line))
    {
        globalContext += line + "\n";
    }

}

void TeaNote::parseTeaMd()
{
    string::iterator iter = globalContext.begin();

    int flag = 1;
    bool isCodeArea = false;
    /* 
        1 标题# 
        2 标题title
        3 内容 
    */
    Section curSection;
    curSection._init();
    while(iter != globalContext.end())
    {
        switch (flag)
        {
        case 1:
            if(*iter == '#') {
                curSection.rank++;
                break;
            }
            else {
                flag = 2;
                continue;
            }

        case 2:
            if(*iter != '\n') {
                curSection.title.push_back(*iter);
                break;
            }
            else {
                flag = 3;
                break;
            }
        case 3:
            if( !isCodeArea && (curSection.content.length() == 0 && *iter == '#') || 
                (curSection.content.length() && curSection.content.back() == '\n' && *iter == '#')) {
                this->sections.push_back(curSection);
                curSection._init();
                isCodeArea = false;
                flag = 1;
                continue;
            }
            else {
                curSection.content.push_back(*iter);
                if(*iter == '`' && isCodeArea == false) {
                    isCodeArea = true;
                }
                else if(*iter == '`' && isCodeArea) {
                    isCodeArea = false;
                }
                break;
            }

        }

        iter++;
    }
}

string TeaNote::showHtml()
{
    string html = "<!DOCTYPE html>\n<html>\n<head><meta charset=\"UTF-8\"><title>Code Tea</title></head>";
    html += string("<body>\n");
    this->parseTeaMd();

    for(vector<Section>::iterator it=sections.begin(); it != sections.end(); it++)
    {
        
        html += "<h" + to_string(it->rank) + ">" + it->title + "</h" + to_string(it->rank) + ">\n";
        html += it->content;   
    }
    html += "</body>\n</html>\n";

    return html;
}

string TeaNote::showMardDown()
{
    return this->globalContext;
}

void TeaNote::createPattern_Md(const string& patternPath, const string& desPath)
{
    struct stat buf;
    if(stat(desPath.c_str(), &buf) == 0)
    {
        cout << "Warn. " << desPath << " exist and be cleard and over." << endl;
    }

    ifstream ifs_patternFile(patternPath);
    ofstream ofs_desFile(desPath);
    string line;

    if(!ifs_patternFile.good())
    {
        cout << "Error. Pattern Markdown Note cannot be found!" << endl;
        return ;
    }
    
    while(getline(ifs_patternFile, line))
    {
        ofs_desFile << line << endl;
    }
    ofs_desFile.close();
    ifs_patternFile.close();
}

