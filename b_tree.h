//
//  b_tree.h
//  TP1
//
//  Created by Arthur Senna on 2016-04-24.
//  Copyright © 2016 Arthur Senna. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Register{
    unsigned long long int key;
    char **records;
}Register;

typedef struct Node{
    int name;
    int numKeys;
    int parendNode;
    bool hasChildNode;
	int leafId;
	int numRegStored;

    unsigned long long int *registerKeys;
    int *nextNodes;

    // int leaf;
    // int numRegLeaf;
    
    // unsigned long long int *r;
    
    // int next[MAX_PAG+1];
    // int *next;
}Node;

// Init Functions
Node initNode(int leafId);

//Tree functions
Register readRegister(FILE* inputFile, int numRecords);
void freeRegister(Register r, int numRecords);
void sortNodeRegisters(Node *node);
void insertRegisterKeyIntoNode(Node *node, Register reg);
void storeRegisterOnLeaf(FILE* file, int numRecords, Register regist);
void storeLeafNode(Register newRegist, int numRecords, Node *leaf, int treeOrder);



// Auxiliar functions
char *readInputString(FILE* fp, int size);
char *getFileName(int id);