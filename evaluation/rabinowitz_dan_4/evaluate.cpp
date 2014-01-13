#include "vbCode.cpp"
#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <locale>
#include <list>
#include <algorithm>
#include <string.h>
#include <stdlib.h> 
using namespace std;

vector<struct document> docs;
vector<struct term> postings;
vector<struct dictEntry> dict;
vector<struct queryEntry> queries;

int findDictTerm(string find){
	int loc = dict.size()/2;
	int gap = loc;
	int lastInd = 0;
	int sameWord = 0;
	while (dict[loc].term!=find){
		if (dict[loc].term > find){
			loc -= (gap/2);
		}else if (dict[loc].term < find){
			loc += (gap/2);
		}
		gap = ceil((double)gap/2.0);
		if (lastInd==loc) return -1;
		lastInd = loc;
	}
	return loc;
}

bool docCompare(document a, document b) {return a.weight > b.weight; }

int main(int argc, char* argv[]){
	//vbCode test(1);
	string stopWords[10] = {"THE", "TO", "A", "OF", "AND", "IN", "THAT", "IS", "WAS", "HE"};
	//cout << test.toStr()<<endl;
	int pos = 0;
	cout <<"start reading"<<endl;
	char temp[256];
	ifstream in (argv[1], ifstream::in);
	while (in.good()){
		in.getline(temp, 256);
		//Read in words
		char *word = strtok(temp, " "); 
		//cout <<word <<endl;
		if (word!=NULL){
			string curStr(word);
			dictEntry *curTerm = new dictEntry();
			curTerm->term = curStr;
			//curTerm->termLen = strlen(word);
			word = strtok(NULL," ");
			curTerm->docs = atoi(word);
			word = strtok(NULL, " ");
			curTerm->postRef = atoi(word);
			//cout << curTerm->term <<" "<<curTerm->docs<<" "<<curTerm->postRef<<endl;
			dict.push_back(*curTerm);
		}
	}
	in.close();
	ifstream inPost (argv[2], ifstream::in);
	while (inPost.good()){
		inPost.getline(temp, 256);
		//Read in words
		char *word = strtok(temp, " "); 
		//cout <<word <<endl;
		if (word!=NULL){
			string curStr(word);
			term *curTerm = new term();
			curTerm->term = curStr;
			//curTerm->termLen = strlen(word);
			word = strtok(NULL," ");
			curTerm->doc = atoi(word);
			word = strtok(NULL, " ");
			curTerm->pos = atoi(word);
			//cout << curTerm->term <<" "<<curTerm->doc<<" "<<curTerm->pos<<endl;
			postings.push_back(*curTerm);
		}
	}
	cout <<"Half way done"<<endl;
	inPost.close();
	ifstream inQuery (argv[3], ifstream::in);
	bool firstEntry = true;
	queryEntry *curEntry;
	while (inQuery.good()){
		inQuery.getline(temp, 256);
		char *word = strtok(temp, " ");
		if (strstr(temp, "*FIND")!=NULL){
			word = strtok(NULL," ");
			if (!firstEntry) {
				queries.push_back(*curEntry);
			}
			firstEntry = false;
			int queryNum = atoi(word);
			curEntry = new queryEntry();
			curEntry->num = queryNum;
		}else{
			while (word != NULL){
				string cur(word);
				//cout << cur<<endl;
				bool duplicate = false;
				for (vector<struct queryTerm>::iterator it = curEntry->terms.begin() ; it != curEntry->terms.end(); ++it){
					if (it->term == cur){
						it->qtf++;
						//cout<<"Term: "<<it->term<<", freq: "<<it->qtf<<endl;
						duplicate = true;
					}
				}
				if (!duplicate){
					//Take out quotes
					if (cur[0]=='"'){
						cur = cur.substr(1);
					}else if (cur[cur.size()-1]=='"'){
						cur = cur.substr(0, cur.size()-1);
					}
					bool isStopWord = false;
					//Stop Words
					for (int i=0; i<10;i++){
						if (strcmp(cur.c_str(),stopWords[i].c_str())==0){	
							isStopWord = true;
						}
					}
					if (!isStopWord){
						queryTerm *curTerm = new queryTerm();
						curTerm->term = cur;
						//cout << cur <<endl;
						curTerm->qtf = 1;
						curEntry->terms.push_back(*curTerm);
					}
				}
				word = strtok(NULL, " ");
			}
		}
	}
	queries.push_back(*curEntry);
	//Read in Relevant Documents
	ifstream inRel(argv[4], ifstream::in);
	while (inRel.good()){
		inRel.getline(temp, 256);
		char *word = strtok(temp, " ");
		if (word!=NULL){
			int query = atoi(word);
			word = strtok(NULL, " ");
			int doc = atoi(word);
			for (int i=0;i<queries.size();i++){
				if (queries[i].num==query){
					queries[i].relevant.push_back(doc);
				}
			}
		}
	}
	cout << "Finished Reading"<<endl;	
	//Calculate Document Weights
	//Find highest doc id number
	list<int> docIds;
	for (int i=0;i<postings.size();i++){
		docIds.push_back(postings[i].doc);
	}
	docIds.sort();
	docIds.unique();
	for (list<int>::iterator it = docIds.begin();it!=docIds.end();it++){
		document *newDoc = new document();
		newDoc->id = *it;
		newDoc->weight = 0.0;
		docs.push_back(*newDoc);
	}
	float totalRecall =0;
	float totalPrecision = 0;
	//cout << postings[dict[findDictTerm("KENNEDY")].postRef].pos<<endl;
	for (vector<struct queryEntry>::iterator entry = queries.begin();entry!=queries.end();entry++){
		for (vector<struct queryTerm>::iterator cur = entry->terms.begin();cur!=entry->terms.end();cur++){
			cout << cur->term << " ";
			int dictLoc = findDictTerm(cur->term);
			if (dictLoc !=-1){	
				double df = (double)dict[dictLoc].docs;
				double numDocs = (double)docIds.size();
				double idf = log10(numDocs/df);
				int postRef = dict[dictLoc].postRef;
				int curDoc = postings[postRef].doc;
				while (postings[postRef].term == cur->term){
					int docPos = 0;
					for (int i=0;i<docs.size();i++){
						if (docs[i].id==curDoc) docPos = i;
					}
					int dtf = 0;
					while (postings[postRef].doc == curDoc){
						dtf++;
						postRef++;
					}
					//cout << "Adding "<<(idf*dtf*cur->qtf) <<" weight to doc "<<docs[docPos].id<<endl;
					docs[docPos].weight += (idf * dtf * cur->qtf);
					curDoc = postings[postRef].doc;
				}
			}
		}
		cout <<endl;
		//Sort and Print Results
		sort (docs.begin(), docs.end(), docCompare);
		int relevant = 0;
		for (int i=0; i<10;i++){
			char *rel = "N";
			for (int n=0;n<entry->relevant.size();n++){
				if (entry->relevant[n]==docs[i].id){ 
					rel="R";
					relevant++;
				}
			}
			cout << docs[i].id<<" "<<rel<<endl;
		}
		float recall = (float) relevant/ (float) entry->relevant.size();
		float precision = (float)relevant/10.0;
		cout << "The recall is "<<recall<<" and the precision is "<<precision<<endl;
		totalRecall += recall;
		totalPrecision += precision;
		for (int i=0;i<docs.size();i++){
			docs[i].weight = 0.0;
		}
	}
	totalRecall = totalRecall/queries.size();
	totalPrecision = totalPrecision/queries.size();
	cout << "The average recall is "<<totalRecall<<" and the average precision is "<<totalPrecision<<endl;
	return 0;
}
