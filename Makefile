getValidWords: getValidWords.o problem.o prefixTree.o traversal.o
	gcc -Wall -o getValidWords getValidWords.o problem.o prefixTree.o traversal.o -g

getValidWords.o: getValidWords.c
	gcc -Wall -o getValidWords.o -c getValidWords.c -g

giveHint: giveHint.o problem.o prefixTree.o traversal.o
	gcc -Wall -o giveHint giveHint.o problem.o prefixTree.o traversal.o -g

giveHint.o: giveHint.c
	gcc -Wall -o giveHint.o -c giveHint.c -g

problem.o: problem.h problem.c solutionStruct.c problemStruct.c
	gcc -Wall -o problem.o -c problem.c -g

prefixTree.o: prefixTree.c prefixTree.h
	gcc -Wall -o prefixTree.o -c prefixTree.c -g