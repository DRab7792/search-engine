#include "vbCode.h"
using namespace std;

vbCode::vbCode(long val){
	if (val>0){
		int len = (int)ceil(log(val)/log(2));
		if (val==1) len = 1;
		int numBytes = ceil(len/7.0);
		size = numBytes;
		int adjIndex = (numBytes*7)-1;
		for (int i=0;i<numBytes;i++){
			bitset<8> cur;
			if (i==(numBytes-1)) cur.set(7);
			for (int n=6;n>-1;n--, adjIndex--){
				if (val>=pow(2,adjIndex)){
					val -= pow(2, adjIndex);
					cur.set(n);
				}
				//cout << adjIndex<<": "<<cur[n]<<endl;
			}
			bytes.push_back(cur); 
		}
	}else{
		bitset<8> cur;
		cur.set(7);
		cur.set(0);
		bytes.push_back(cur);
		size = 1;
	}
}

int vbCode::getLen(){
	return size;
}

string vbCode::toStr(){
	string final = "";
	for (int i=0;i<bytes.size();i++){
		final += bytes.at(i).to_string<char,string::traits_type,string::allocator_type>();
	}
	return final;
}
