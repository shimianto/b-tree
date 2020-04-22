//
//  main.c
//  TP1
//
//  Created by Arthur Senna on 2016-04-24.
//  Copyright © 2016 Arthur Senna. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "b_tree.h"

void insertNode(FILE *outputFile, FILE *inputFile, int numRecords, Node *node, int *numLeafs, int treeOrder){
	Register newRegister;
	newRegister = readRegister(inputFile, numRecords);
	
	if (node->hasChildNode)
	{
		/* code */
	}

	insertRegisterKeyIntoNode(node, newRegister);

	// If it is a leaf node store register in memory in memory
	if (!node->hasChildNode)
	{
		storeLeafNode(newRegister, numRecords, node, treeOrder);
	}

	// if (/* condition */)
	// {
	// 	/* code */
	// }

	freeRegister(newRegister, numRecords);
}

int main(int argc, const char * argv[]){

	FILE *outputFile, *inputFile;
	int treeOrder, numRecords, numLeafs = 0;
	char *operation;
	char c;
	unsigned long long int searchKey;

	// Init root node
	Node root = initNode(numLeafs);

	// Gets output file
	outputFile=fopen(argv[1], "w");
    fclose(outputFile);

    // Opens input file
    inputFile=fopen(argv[2], "r");

    // Gets tree order and the number of records per register
    treeOrder=atoi(argv[3]);
    numRecords=atoi(argv[4])-1; // Minus the key

    operation = readInputString(inputFile, 5);

    while(operation[0] == 'a' || operation[0] == 's' || operation[0] == 'd'){
    	if(operation[0] == 'a'){
    		insertNode(outputFile, inputFile, numRecords, &root, &numLeafs, treeOrder);
    	}
    	else if (operation[0]=='s')
    	{
    		// Get key from register to be searchef
			fscanf(inputFile, "%lld%c",&searchKey,&c);

    	}
    	else
    	{
    		printf("Unidentified operation: '%s'. Ending procedure.\n", operation);
    	}

    	free(operation);
		operation = readInputString(inputFile, 5);
    }

    free(operation);

    fclose(inputFile);
	return 0;
}











