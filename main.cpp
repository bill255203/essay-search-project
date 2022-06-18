#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>
#include<set>
#include<time.h>
#include<ctype.h>
using namespace std;
// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先將要切割的字串從string型別轉換為char*型別
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字串轉換為string型別
		res.push_back(s); //存入結果陣列
		p = strtok(NULL, d);
	}

	return res;
}
const int ALPHABET_SIZE = 26;
struct TrieNode{
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
};
// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void){
    struct TrieNode *pNode =  new TrieNode;

    pNode->isEndOfWord = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index;
        if(key[i]>='a'&&key[i]<='z') index=key[i] - 'a';
        else if(key[i]>='A'&&key[i]<='Z') index=key[i] - 'A';
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}

bool searchExact(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index;
        if(key[i]>='a'&&key[i]<='z') index=key[i] - 'a';
        else if(key[i]>='A'&&key[i]<='Z') index=key[i] - 'A';
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }

    return (pCrawl->isEndOfWord);
}
bool searchPrefix(struct TrieNode *root, string key)
{
    struct TrieNode *pCrawl = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index;
        if(key[i]>='a'&&key[i]<='z') index=key[i] - 'a';
        else if(key[i]>='A'&&key[i]<='Z') index=key[i] - 'A';
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }
    return true;
}

bool searchSuffix(struct TrieNode *revroot, string key)
{
    struct TrieNode *pCrawl = revroot;

    for (int i = 0; i < key.length(); i++)
    {
        int index;
        if(key[i]>='a'&&key[i]<='z') index=key[i] - 'a';
        else if(key[i]>='A'&&key[i]<='Z') index=key[i] - 'A';
        if (!pCrawl->children[index])
            return false;

        pCrawl = pCrawl->children[index];
    }
    return true;
}

void traverse(TrieNode*cur){
    for(int i = 0; i < ALPHABET_SIZE; i ++){
        if(cur -> children[i]) {
            cout << (char)(i+'a') << " ";
            traverse(cur -> children[i]);
        }
    }
}
//how to reverse string
void reverse(string& str){
    int n = str.length();
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

int main(int argc, char *argv[]){
    clock_t start,end;
    start=clock();

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

    int num=0;
	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string,QUERY,TITLES;
    vector<struct TrieNode *> ROOTS,revROOTS;
	// for every read in files we do what
    for(int i = 0;;i++){
		string file_name = data_dir + to_string(i) + ".txt";
		fi.open(file_name,ios::in);
		if (!fi.good()){
			break;
		}
		num++;
        //cout<<num<<endl;
		//for all the titles
        getline(fi, title_name);
        TITLES.push_back(title_name);

        tmp_string = split(title_name, " ");
        vector<string> title = word_parse(tmp_string);
        //put all the title name in TITLES

        struct TrieNode *root = getNode();
        struct TrieNode *revroot = getNode();
        for(auto &word : title){
            insert(root, word);
            reverse(word);
            insert(revroot,word);
        }
        //for all the lines in content scan line by line
        while(getline(fi, tmp)){
            tmp_string = split(tmp, " ");
            vector<string> content = word_parse(tmp_string);

            for(auto &word : content){
                insert(root, word);
                reverse(word);
                insert(revroot,word);
            }
        }
        ROOTS.push_back(root);
            revROOTS.push_back(revroot);
        fi.close();
    }
    //traverse(ROOTS[0]);
///////////////////////////////////////////////////////////////////////////////////////////////
    //to deal with query

    fi.open(query,ios::in);
    //for every line of query i get do what
    fstream out;
    out.open(output, ios::out);

    while(getline(fi, tmp)){
        //cout<<tmp<<endl;
        QUERY = split(tmp, " ");
        //cout<<"new line ---------------------------------------------------------"<<endl;
        //the numbers of output files
        set<int> NUM;
        bool state=0;
        for(auto &query : QUERY){
            //cout<<state<<endl;
            if(query[0]=='/') state=0;
            else if(query[0]=='+') state=1;
            else{
                //cout<<" i get lots of fucking strings"<<endl;
                if(state==0){
                    //cout<<"divide"<<endl;
                    if(isalpha(query[0])){
                        //cout<<query<<endl;
                        for(int j=0;j<num;j++)
                            if(searchPrefix(ROOTS[j],query)){
                                NUM.insert( j );
                                //cout<<"gotcha bitch 1"<<endl;
                            }
                    }
                    else if(query[0]=='*'){
                        query = query.substr(1,query.size()-2);
                        reverse(query);
                        //cout<<query<<endl;
                        for(int j=0;j<num;j++){
                            if(searchSuffix(revROOTS[j],query)){
                                NUM.insert( j );
                                //cout<<"gotcha bitch 2"<<endl;
                            }
                        }
                    }
                    else if(query[0]=='"'){
                        query = query.substr(1,query.size()-2);
                        //cout<<query<<endl;
                        for(int j=0;j<num;j++){

                            if(searchExact(ROOTS[j],query)){
                                NUM.insert( j );
                                //cout<<"gotcha bitch 3"<<endl;
                            }
                        }
                    }
                }
                else{
                    //cout<<"add"<<endl;
                    if(isalpha(query[0])){
                        //cout<<query<<endl;
                        set <int> temp_set;
                        temp_set=NUM;
                        for(auto &num:temp_set)
                            if(!searchPrefix(ROOTS[num],query)){
                                NUM.erase(num);
                                //cout<<"gotcha bitch 4"<<endl;
                            }
                    }
                    else if(query[0]=='*'){
                        query = query.substr(1,query.size()-2);
                        reverse(query);
                        //cout<<query<<endl;
                        set<int> temp_set;
                        temp_set=NUM;
                        for(auto &num:temp_set){
                            if(!searchSuffix(revROOTS[num],query)){
                                NUM.erase(num);
                                //cout<<"gotcha bitch 5"<<endl;
                            }
                        }
                    }
                    else if(query[0]=='"'){
                        query = query.substr(1,query.size()-2);
                        //cout<<query<<endl;
                        set<int> temp_set;
                        temp_set=NUM;
                        for(auto &num:temp_set)
                            if(!searchExact(ROOTS[num],query)){
                                NUM.erase(num);
                                //cout<<"gotcha bitch 6"<<endl;
                            }
                    }
                }
            }
            //print out the things in set
            /*for(auto &num : NUM){
                cout<<num<<' ';
            }cout<<endl;*/
        }
        ////////////////////////////////////////////////////////////////
        //to print the titles
        for(auto &num : NUM){
            //cout << TITLES[num] << endl;
            out<<TITLES[num] <<endl;
        }
        if(NUM.size()==0) out<<"Not Found!"<<endl;
    }
    out.close();

    end=clock();
    //cout << (double)((double)(end-start)/CLOCKS_PER_SEC)<<endl;
}

// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
