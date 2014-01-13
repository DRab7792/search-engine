rabinowitz: process compress

process: process.cpp
	g++ process.cpp -o process

compress: compress.cpp vbCode.cpp vbCode.h
	g++ compress.cpp vbCode.h -o submission

clean:
	rm submission process dictionary.txt postings.txt
