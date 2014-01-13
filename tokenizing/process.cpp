#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

struct document{
	int id;
	string date;
	int page;
};

struct term{
	int doc;
	string term;
	int pos;
};

vector<struct term> postings;
vector<string> dict;

bool cmp(term a, term b){
	if (a.term != b.term){
		return a.term < b.term;
	}else if (a.doc != b.doc){
		return a.doc < b.doc;
	}else{
		return a.pos < b.pos;
	}
}


int main(){
	char temp[256];
	document *cur = new document();
	int pos = 0;
	ifstream in ("dataP1.txt", ifstream::in);
	while (in.good()){
		in.getline(temp, 256);
		if (strstr(temp,"*TEXT")!=NULL){
			string header (temp, 256);
			cur->id = atoi(header.substr(6, 3).c_str());
			cur->page = atoi(header.substr(24,3).c_str());
			cur->date = header.substr(10,9);
			pos = 1;	
			//cout << "Id: "<<cur->id<<", Page: "<<cur->page<<", Date: "<<cur->date<<endl;
		}else{
			//Read in words
			char *word = strtok(temp, " ,)");
			while (word !=NULL){
				//cout <<word <<endl;
				string curStr(word);
				if (curStr.find("(")!=string::npos) curStr = curStr.replace(0,1,"");
				term *curTerm = new term();
				curTerm->term = curStr;
				//curTerm->termLen = strlen(word);
				curTerm->doc = cur->id;
				curTerm->pos = pos;
 				//Gets rid of quotes, or periods as word
				if (strlen(word)>1 || isalnum(word[0])){
					//cout << (find(dict.begin(), dict.end(), curStr)!=dict.end() || dict.begin()==dict.end());
					if (find(dict.begin(), dict.end(), curStr)==dict.end() || dict.begin()==dict.end()  ){
						dict.push_back(curStr);
						//sort (dict.begin(), dict.end());
					}
					postings.push_back(*curTerm);
					pos++;
				}else{
					delete curTerm;
				}
				word = strtok(NULL, " ,)");
			}
		}
		
	}
	ofstream postFile("postings.txt", ofstream::out);
	ofstream dictFile("dictionary.txt", ofstream::out);
	sort (dict.begin(), dict.end());
	sort(postings.begin(), postings.end(), cmp);
	for (int i=0; i<postings.size();i++){ 
		
		postFile << postings[i].term<<" "<< postings[i].doc<<" "<<postings[i].pos<<endl;
	}
	for (int i=0; i<dict.size();i++){
		dictFile << dict[i]<<endl;
	}
	dictFile.close();
	postFile.close();

}
