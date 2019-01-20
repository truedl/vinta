#include <iostream>
#include <fstream>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

map<string, string> memS;
map<string, int> memI;
map<string, float> memF;
map<string, string> mem;
bool inComment = false;

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

string between(string text, string f, string s){
    return text.substr(text.find(f), text.find(s));
}

vector<string> split(string text, string bychar, string m=""){
    vector<string> lst;
    for(int i = 0; i < text.length(); i++){
        if(string(1, text.at(i)) == bychar){
            lst.push_back(m);
            m = "";
        } else {
            m += string(1, text.at(i));
        }
    }
    if(m != ""){
        lst.push_back(m);
    }
    return lst;
}

void cmpLine(string line){
    string a;
    string b = "";
    int q;
    if(line.substr(0, 2) == "*/"){
        inComment = false;
    }
    if(line.substr(0, 2) == "/*"){
        inComment = true;
    }
    if(line.substr(0, 2) == "//" || inComment){
        return;
    }
    vector<string> list, ot, lmt;
    string command;
    list = split(line, " ");
    for(int i = 0; i < list.size(); i++){
        if(i == 0){
            command = list[0];
        } else {
            ot.push_back(list[i]);
        }
    }
    if(command == "ins"){
        cout << join(ot) << endl;
    } else if(command == "set"){
        q = 0;
        for(int i = 0; i < ot.size(); i++){
            if(ot[i] != " "){
                if(q == 0){
                    a = ot[i];
                    q++;
                } else if(q == 1 && ot[i] == "="){
                    q++;
                } else if(q == 2){
                    if(i+1 != ot.size()){
                        if(b != ""){
                            b += ot[i];
                        } else {
                            b += " " + ot[i];
                        }
                    } else {
                        if(b == ""){
                            b = ot[i];
                        } else {
                            b += " " + ot[i];
                        }
                        if(b.substr(0, 1) == " "){
                            b = b.substr(1, b.size());
                        }
                        if(b.substr(0, 1) == "\"" && b.substr(b.size()-1, b.size()) == "\"" || b.substr(0, 1) == "'" && b.substr(b.size()-1, b.size()) == "'"){
                            memS[a] = b.substr(1, b.size()-2);
                            mem[a] = "s";
                        } else if(b.find(".") != string::npos){
                            memF[a] = stof(b);
                            mem[a] = "f";
                        } else {
                            memI[a] = stoi(b);
                            mem[a] = "i";
                        }
                        break;
                    }
                }
            }
        }
    } else if(command == "varcheck"){
        if(mem.find(ot[0]) != mem.end()){
            if(mem[ot[0]] == "s"){
                cout << "[VAR " << ot[0] << "] " << memS[ot[0]] << " (STRING)" << endl;
            } else if(mem[ot[0]] == "f"){
                cout << "[VAR " << ot[0] << "] " << memF[ot[0]] << " (FLOAT)" << endl;
            } else if(mem[ot[0]] == "i"){
                cout << "[VAR " << ot[0] << "] " << memI[ot[0]] << " (INT)" << endl;
            }
        } else {
            rerr("Variable not found!", "varcheck");
        }
    } else if(command == "pause"){
        cout << join(ot);
        getch();
    }
}

int main(int argc, char* argv[]){
    string l;
    if(argc <= 1){
        rerr("Not Enough Arguments Provided");
    }
    ifstream fc;
    fc.open(argv[1]);
    if(!fc){
        rerr("Unable to open file", argv[1]);
    } else {
        while(getline(fc, l)){
            cmpLine(l);
        }
    }
    return 0;
}