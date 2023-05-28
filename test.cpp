#pragma GCC optimize("O2")
#include<iostream>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<vector>
#include <unordered_map>
#include<string>
#include<string.h>
using namespace std;

unordered_map<std::string, vector<int>> vocab;

void _corpus_tokenize(char* FILE);
void solve(char* query);

int main(int argc, char* argv[]) {
    /*
    solution:
        1. 先把所有的corpus跑完，將哪些字存在在哪些id的line存到vocab中 (vocab為全域變數)
        2. 對query逐行跑，對vocab轉換，去交集即是輸出。
    */
    _corpus_tokenize(argv[1]); //get vocab
    //cout<<"_corpus_tokenize done"<<endl;
    solve(argv[2]);

    return 0;
}

void _corpus_tokenize(char* FILE) {
    ifstream infile(FILE);
    string line;
    while(getline(infile, line)) {
        int number=0, t=0;
        for(unsigned int i=0; i<line.length(); i++) { //取得前面id
            if(line[i] == ',') {
                t = i+3;
                break;
            }
            number = number*10 + (line[i] - 48);
        }
        string word_buffer = "";
        line = line.substr(t, line.size()-1);
        vector<string> clear_line;

        for(unsigned int i=0; i<line.length(); i++) {
            bool end = 0;
            if(line[i+1] == ' ' || i == line.length()-1) end = 1; //輸出字元
            
            
            if(line[i] >= 'A' && line[i] <= 'Z') {      //lowercase
                word_buffer += (line[i]+32);
                continue;
            }
            if((line[i] >'Z' && line[i] < 'a') || (line[i] < 'A') || line[i] > 'z'); //特殊字元
            else word_buffer += line[i];

            if(end == 1) {
                clear_line.push_back(word_buffer);
                word_buffer = "";
                end = 0;
                if(i == line.length()-1) break;
                continue;
            }

        }
        //cout<<"start of a corpus\n";
        for(auto token : clear_line) {
            //cout<<token<<'\n';
            //cout<<token<<'\n';
            if(vocab.find(token) != vocab.end()) {
                int same = 0;
                for(auto v : vocab[token]) { //避免同一個corpus有相同的字;
                    if(v == number)  {
                        same = 1;
                        break;
                    }
                }
                if(same == 1) continue;
            }
            vocab[token].push_back(number);
        }
        clear_line.clear();
        //cout<<"corpus_"<<number<<"ok"<<endl;
    }
    infile.close();
}

void solve(char* query) {
    ifstream infile(query);
    string line;
    while(getline(infile, line)) {
        //preprocess
        //cout<<line<<endl;
        string word_buffer = "";
        vector<string> clear_line;
        for(unsigned int i=0; i<line.length(); i++) {
            bool end = 0;
            if(line[i+1] == ' ' || i == line.length()-1) end = 1; //輸出字元
            
            
            if(line[i] >= 'A' && line[i] <= 'Z') {      //lowercase
                word_buffer += (line[i]+32);
                continue;
            }
            if((line[i] >'Z' && line[i] < 'a') || (line[i] < 'A') || line[i] > 'z'); //特殊字元
            else word_buffer += line[i];

            if(end == 1) {
                clear_line.push_back(word_buffer);
                word_buffer = "";
                end = 0;
                if(i == line.length()-1) break;
                continue;
            }
        }


        //initialization.
        vector<int> inter;
        vector<int> buffer;
        //cout<<clear_line.front()<<endl;
        inter = vocab[clear_line.front()];
        for(auto token : clear_line) {
            //for(auto v:inter) cout<<v<<' ';
            //cout<<endl;
            for(auto id: vocab[token]) {
                if(find(inter.begin(), inter.end(), id) != inter.end()) { //找到詞
                    buffer.push_back(id);
                }
                //沒找到就不要理他
            }
            inter = buffer;
            buffer.clear();
        }
        clear_line.clear();
        //輸出
        if(inter.empty()) {
            cout<<"-1"<<endl;
            continue;
        }
        sort(inter.begin(), inter.end());
        for(auto i=inter.begin(); i!=inter.end(); i++) {
            if(i != inter.end()-1) cout<<*i<<' ';
            else cout<<*i<<endl;
        }
        inter.clear();
    }
    infile.close();
}