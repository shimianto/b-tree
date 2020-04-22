//
//  b_tree.c
//  TP1
//
//  Created by Arthur Senna on 2016-04-24.
//  Copyright © 2016 Arthur Senna. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "b_tree.h"

// Init functions
Node initNode(int leafId){
	Node node = {.name = 0, .numKeys = 0, .parendNode = -1, .hasChildNode = false, .leafId = leafId, .numRegStored = 0};
	return node;
}

//Tree functions

Register readRegister(FILE* inputFile, int numRecords){
	Register newRegister;
	char c;

	// Gets register's key and skip tab or newline char
	fscanf(inputFile, "%lld%c",&newRegister.key,&c);

	newRegister.records = realloc(NULL, sizeof(char*)*numRecords);

	for (int i = 0; i < numRecords; ++i)
	{
		newRegister.records[i] = readInputString(inputFile,10);
	}

	return newRegister;
}

void freeRegister(Register r, int numRecords){
	for (int i = 0; i < numRecords; ++i)
	{
		free(r.records[i]);
	}
	free(r.records);
}

void sortNodeRegisters(Node *node){
	// Ordena a lista de registros
	for(int i=0;i<node->numKeys;i++){
		for (int j=i+1; j<node->numKeys; j++) {
			unsigned long long int aux;

			if (node->registerKeys[i]>node->registerKeys[j]) { 
				aux=node->registerKeys[i];
				node->registerKeys[i]=node->registerKeys[j];
				node->registerKeys[j]=aux;
			}
		}
	}
}

void insertRegisterKeyIntoNode(Node *node, Register reg){
	if (node->numKeys == 0){
		node->registerKeys = realloc(NULL, sizeof(unsigned long long int)*(node->numKeys+1));
	}
	else{
		node->registerKeys = realloc(node->registerKeys, sizeof(unsigned long long int)*(node->numKeys+1));
	}
	node->registerKeys[node->numKeys++] = reg.key;
	sortNodeRegisters(node);
}

void storeRegisterOnLeaf(FILE* file, int numRecords, Register regist){
	fprintf(file, "%lld\n",regist.key);
		for (int i = 0; i < numRecords; ++i)
		{
			fprintf(file, "%s\t",regist.records[i]);
		}
            
		fprintf(file, "\n");
}

void storeLeafNode(Register newRegist, int numRecords, Node *leaf, int treeOrder){
	FILE *oldLeafFile, *newLeafFile;
	char *newFileName = "new_leaf_file.txt";
	char *fileName = getFileName(leaf->leafId);
	
	newLeafFile = fopen(newFileName,"w");
	if (leaf->numRegStored == 0)
	{
		storeRegisterOnLeaf(newLeafFile, numRecords, newRegist);
	}
	else{ // Read old leaf file and inserts new register in the right order
		oldLeafFile = fopen(fileName,"r");
		Register *leafRegisters = realloc(NULL, sizeof(Register)*(leaf->numRegStored+1));

		leafRegisters[0] = newRegist;

		for (int i = 1; i <= leaf->numRegStored; ++i)
		{
			leafRegisters[i] = readRegister(oldLeafFile, numRecords);
		}
		// Sorts register leafs
		for(int i=0;i<=leaf->numRegStored;i++){
			for (int j=i+1; j<=leaf->numRegStored; j++) {
				Register r;

				if (leafRegisters[i].key > leafRegisters[j].key) { 
					r=leafRegisters[i];
					leafRegisters[i]=leafRegisters[j];
					leafRegisters[j]=r;
				}
			}
		}

		// Stores registers on leaf file
		for (int i = 0; i <= leaf->numRegStored; ++i)
		{
			storeRegisterOnLeaf(newLeafFile, numRecords, leafRegisters[i]);
		}

		free(leafRegisters);
		fclose(oldLeafFile);
	}
	fclose(newLeafFile);

	leaf->numRegStored++;

	remove(fileName);
	rename(newFileName,fileName);
	free(fileName);
}


// Auxiliar functions

// Read a string of unknown lenght until tap or newline
char *readInputString(FILE* inputFile, int size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    int len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size

    if(!str)return str;
    while(EOF!=(ch=fgetc(inputFile)) && ch!= '\t' && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=10));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}


char *getFileName(int id){
	char *fileName, *c;

	// Ge file name
	int nameSize = snprintf(c, 0, "leaf_%d.txt", id) + 1; // Adds endstring char
	fileName = realloc(NULL,sizeof(char)*nameSize);
	snprintf(fileName, nameSize, "leaf_%d.txt", id);
	return fileName;
}
