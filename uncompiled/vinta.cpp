#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

map<string, char> mem;
map<string, string> memS;
map<string, int> memI;
map<string, float> memF;
bool isComment = false;

void rerr(string err, string additional=""){
    if(additional == ""){
        cout << "[ERROR] " << err << endl;
    } else {
        cout << "[ERROR] (" << additional << ") " << err << endl;
    }
    exit(1);
}

string join(vector<string> list, string delim=" ", string m=""){
    if(list.size() == 1){
        return list[0];
    } else {
        for(int i = 0; i < list.size(); i++){
            if(i+1 != list.size()){
                m += list[i] + delim;
            } else {
                m += list[i];
            }
        }
        return m;
    }
}

vector<string> split(string text, char bychar, string m=""){
    vector<string> lst;
    for(int i = 0; i < text.size(); i++){
        if(text[i] == bychar){
            lst.push_back(m);
            m = "";
        } else {
            m += text[i];
        }
    }
    if(m != ""){
        lst.push_back(m);
    }
    return lst;
}

vector<string> splitSkipS(string text, char bychar, bool isStr = false, string m=""){
    vector<string> lst;
    for(int i = 0; i < text.size(); i++){
        if(!isStr){
            if(text[i] == bychar){
                lst.push_back(m);
                m = "";
            } else if(text[i] == '"'){
                if(text[i-1] != '\\'){
                    isStr = true;
                }
                m += text[i];
            } else {
                m += text[i];
            }
        } else {
            if(text[i] == '"'){
                if(text[i-1] != '\\'){
                    isStr = false;
                }
                m += text[i];
            } else {
                m += text[i];
            }
        }
    }
    if(m != ""){
        lst.push_back(m);
    }
    return lst;
}

vector<string> fsplit(string text, char bychar, string m="", string mm="", bool skp = false){
    vector<string> lst;
    for(int i = 0; i < text.size(); i++){
        if(!skp){
            if(text[i] == bychar){
                skp = true;
            } else {
                m += text[i];
            }
        } else {
            mm += text[i];
        }
    }
    lst.push_back(m);
    lst.push_back(mm);
    return lst;
}

string wsf(string line, bool inStr = false, string rt=""){
    for(int i = 0; i < line.size(); i++){
        if(line[i] == '"'){
            rt += line[i];
            if(inStr){
                if(line[i-1] != '\\'){
                    inStr = false;
                }
            } else {
                inStr = true;
            }
        } else if(line[i] != ' ' || inStr){
            rt += line[i];
        }
    }

    return rt;
}

string stvp(string text, string b=""){
    vector<string> v = splitSkipS(text, '.');
    for(int i = 0; i < v.size(); i++){
        if(v[i].at(0) == '"' && v[i].back() == '"'){
            b += v[i].substr(1, v[i].size()-2);
        } else {
            if(mem.find(v[i]) != mem.end()){
                if(mem[v[i]] == 's'){
                    b += memS[v[i]];
                } else if(mem[v[i]] == 'f'){
                    b += to_string(memF[v[i]]);
                } else if(mem[v[i]] == 'i'){
                    b += to_string(memI[v[i]]);
                }
            } else {
                rerr("Variable not found", v[i]);
            }
        }
    }
    return b;
}

void cmpline(string text){
    vector<string> v;
    string b;
    if(text == ""){
        return;
    }
    vector<string> vec = fsplit(text, ' ');
    vec[1] = wsf(vec[1]);
    if(vec[0] == "ins"){
        cout << stvp(vec[1]) << endl;
    } else if(vec[0] == "set"){
        v = split(vec[1], '=');
        if(v[1].at(0) == '"' && v[1].back() == '"'){
            mem[v[0]] = 's';
            memS[v[0]] = stvp(v[1]);
        } else if(v[0].find('.') != string::npos){
            mem[v[0]] = 'f';
            memF[v[0]] = stof(v[1]);
        } else {
            mem[v[0]] = 'i';
            memI[v[0]] = stoi(v[1]);
        }
    } else if(vec[0] == "pause"){
        cout << stvp(vec[1]);
        getch();
    } else {
        rerr("Unknowen command", vec[0]);
    }
}

int main(int argc, char* argv[]){
    ifstream fc;
    string l;
    if(argc <= 1){
        rerr("Not enough arguments provided");
    }
    fc.open(argv[1]);
    if(!fc){
        rerr("Could not open the file", argv[1]);
    }
    while(getline(fc, l)){
        if(l.substr(0, 2) == "/*"){
            isComment = true;
        }
        if(l.substr(0, 2) != "//" && !isComment){
            cmpline(l);
        }
        if(l.substr(0, 2) == "*/"){
            isComment = false;
        }
    }
    return 0;
}