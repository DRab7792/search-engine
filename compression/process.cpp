#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "vbCode.h"
using namespace std;

vector<struct term> postings;
vector<struct dictEntry> dict;

bool cmp(term a, term b){
	if (a.term != b.term){
		return a.term < b.term;
	}else if (a.doc != b.doc){
		return a.doc < b.doc;
	}else{
		return a.pos < b.pos;
	}
}

bool cmpEntry(dictEntry a, dictEntry b){
	return a.term < b.term;
}

int main(){
	char temp[256];
	document *cur = new document();
	vector<string> list;
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
					if (find(list.begin(), list.end(), curStr)==list.end() || list.begin()==list.end()  ){
						dictEntry *entry = new dictEntry();
						entry->term = curStr;
						entry->postRef = 1;
						dict.push_back(*entry);
						list.push_back(curStr);
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
	sort (dict.begin(), dict.end(), cmpEntry);
	sort(postings.begin(), postings.end(), cmp);
	vector<struct dictEntry>::iterator it = dict.begin();
	int line = 1;
	for (vector<struct term>::iterator cur=postings.begin(); cur!=postings.end();cur++){
		if (cur->term > it->term){
			it++;
			it->docs = 1;
			it->postRef = line;
		}else if (cur->term == it->term){
			it->docs++;
		}
		line++;
		
	}
	for (int i=0; i<postings.size();i++){ 
		
		postFile << postings[i].term<<" "<< postings[i].doc<<" "<<postings[i].pos<<endl;
	}
	for (int i=0; i<dict.size();i++){
		dictFile << dict[i].term<< " " << dict[i].docs << " "<< dict[i].postRef<<endl;
	}
	dictFile.close();
	postFile.close();

}
