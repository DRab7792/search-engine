#include <math.h>
#include <bitset>
#include <string>
#include <list>
#include <vector>
#include <iostream>
using namespace std;


struct document{
	int id;
	string date;
	int page;
	double weight;
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

struct queryTerm{
	string term;
	int qtf;
};

struct queryEntry{
	int num;
	vector<int> relevant;
	vector<struct queryTerm> terms;
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
