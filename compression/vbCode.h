#include <math.h>
#include <bitset>
#include <string>
#include <vector>
#include <iostream>
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

struct dictEntry{
	string term;
	int docs;
	long postRef;
};



class vbCode{
	private:
		vector<bitset<8> > bytes;
		int size;
	public:
		vbCode(long val);
		string toStr();
		int getLen();
};
