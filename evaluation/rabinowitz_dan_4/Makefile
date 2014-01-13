rabinowitz: process compress evaluate

process: process.cpp
	g++ process.cpp -o process

compress: compress.cpp vbCode.cpp vbCode.h
	g++ compress.cpp vbCode.h -o compress

evaluate: evaluate.cpp vbCode.cpp vbCode.h
	g++ evaluate.cpp vbCode.h -o evaluate

clean:
	rm submission process dictionary.txt postings.txt evaluate
