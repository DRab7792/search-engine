#include "vbCode.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h> 
using namespace std;

vector<struct term> postings;
vector<struct dictEntry> dict;

int main(int argc, char* argv[]){
	//vbCode test(1);
	//cout << test.toStr()<<endl;
	int pos = 0;
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
	inPost.close();
	ofstream postFile("new_posting.txt", ofstream::out);
	ofstream readable("readable_posting.txt", ofstream::out);
	ofstream dictFile("new_dict.txt", ofstream::out);
	long lastPoint = 0;
	bool diffDoc = false;
	vector<struct term>::iterator lastPt=postings.begin();
	vector<struct term>::iterator pIt = postings.begin();
	for (vector<struct dictEntry>::iterator dIt = dict.begin(); dIt!=dict.end();dIt++){
		int count = 0;
		int size = 0;
		while(count<dIt->docs){
			vector<int> post;
			size = 0;
			post.push_back(0);
			post.push_back(pIt->doc);
			vbCode docId(pIt->doc);
			size +=docId.getLen();
			
			post.push_back(dIt->docs);
			vbCode tf(dIt->docs);
			size +=tf.getLen();
			int last = 0;
			for (int i=0; i<dIt->docs && lastPt->doc==pIt->doc;i++, pIt++){
				post.push_back(pIt->pos - last);
				vbCode cur(pIt->pos - last);
				//cout <<(pIt->pos - last) << ", " << cur.getLen() <<endl;
				size += cur.getLen();
				count ++;
				last = pIt->pos;
				post[2] = (i+1);
			}
			vbCode lenSize(size);
			size += lenSize.getLen();
			//cout << lenSize.getLen()<<", "<<docId.getLen()<<", "<< dIt->docs<<endl;
			post[0] = size;
			vbCode finalSize(size);
			readable << "(";
			for (int i=0;i<post.size();i++){
				readable << post[i];
				vbCode cur(post[i]);
				postFile << cur.toStr();
				if (i<(post.size()-1)) {readable <<", ";}
			}
			readable<<")";
			readable<<endl;	
			lastPt = pIt;
			lastPoint += size;
		}
		dictFile << dIt->term << " " << dIt->docs << " " << (lastPoint-size)<<endl;
	
		//lastPoint += size;
	}
	postFile.close();
	readable.close();
	dictFile.close();
	return 0;
}
