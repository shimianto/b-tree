//
//  b_tree.h
//  b-tree_xcode
//
//  Created by Arthur Senna on 2020-04-22.
//  Copyright © 2020 Arthur Senna. All rights reserved.
//

#ifndef b_tree_h
#define b_tree_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Register{
    unsigned long long int key;
    char **records;
}Register;

typedef struct Node{
    long id;
    long parendId;

    int numKeys;
    int numChildren;

    bool isLeafNode;
    long leafId;

    int numRecords;
    int treeOrder;

    unsigned long long int *registerKeys;
    long *childNodes;
}Node;

// Init Functions
Node initNode(Node oldNode);
Node initRootNode(int numRecords, int treeOrder);

//Tree functions
//Read
Register readRegister(FILE* inputFile, int numRecords);
Register *readRegistersFromLeaf(FILE *file, Node *leaf);
Node getNode(long nodeId);

// Store functions
void storeRegisterOnLeaf(FILE* file, int numRecords, Register regist);
void storeLeafNode(Register newRegist, Node leaf);
void storeNode(Node node);

// Insert functions
void insertRegisterKeyIntoNode(Node *node, unsigned long long int regKey);
void insertChildKeyIntoNode(Node *node, long nodeId);
void insertRegisterIntoTree(Register regst, Node *node, long *newLeafId, long *newNodeId);
void insertRegisterIntoNode(Node *node, Register regst, long *newLeafId, long *newNodeId);
void splitNode(Node *node, long *newLeafId, long *newNodeId);

// Auxiliar functions
char *readInputString(FILE* fp, int size);
char *getFileName(long id, bool isLeafNode);
void freeRegister(Register r, int numRecords);
void sortNodeRegisters(Node *node);


#endif /* b_tree_h */
