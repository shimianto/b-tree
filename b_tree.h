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
#include "queue.h"

typedef struct Register{
    unsigned long long int key;
    bool hasRecords;
    char **records;
}Register;

typedef struct Node{
    long id;
    long parendId;

    int numKeys;
    int numChildren;

    bool isLeafNode;
    long leafId;

    int recordSize;
    int treeOrder;

    unsigned long long int *registerKeys;
    long *childNodes;
}Node;

// Init Functions
Node initNode(Node oldNode);
Node initRootNode(int recordSize, int treeOrder);
void updatesRootId(long newId);
long getRootId(void);

//Tree functions
//Read
Register readRegister(FILE* inputFile, int recordSize);
Register *readRegistersFromLeafFile(FILE *file, Node leaf);
Node getNode(long nodeId);
Register getRegisterFromLeaf(unsigned long long int rKey, Node leafNode);

// Store functions
void storeRegisterOnFile(FILE* file, int recordSize, Register regist);
void storeLeafNode(Register newRegist, Node leaf);
void storeNode(Node node);

// Insert functions
void insertRegisterKeyIntoNode(Node *node, unsigned long long int regKey);
void insertChildKeyIntoNode(Node *node, long nodeId);
void insertRegisterIntoTree(Register regst, Node *node, long *newLeafId, long *newNodeId);
void insertRegisterIntoNode(Node *node, Register regst, long *newLeafId, long *newNodeId);
void splitNode(Node *node, long *newLeafId, long *newNodeId);
void printRegisterOnFile(FILE *outputFile, Register r, int recordSize);

// Search functions
Register searchRegisterInTree(unsigned long long int regstKey, Node node);
void dump(FILE *outputFile, long rootId, long numNodes);

// Auxiliar functions
char *readInputString(FILE* fp, int size);
char *getFileName(long id, bool isLeafNode);
void freeRegister(Register r, int recordSize);
void freeNode(Node *node);
void sortNodeRegisters(Node *node);
void sortNodesChildren(Node *node);
void dropDatabase(long numNodes, long numLeafs);


#endif /* b_tree_h */
