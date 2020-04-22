//
//  b_tree.c
//  b-tree_xcode
//
//  Created by Arthur Senna on 2020-04-22.
//  Copyright © 2020 Arthur Senna. All rights reserved.
//

#include "b_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Init functions
Node initNode(Node oldNode){
    Node node = {
        .id = oldNode.id,
        .parendId = oldNode.parendId,

        .numKeys = 0,
        .numChildren = 0,

        .isLeafNode = oldNode.isLeafNode,
        .leafId = oldNode.leafId,

        .numRecords = oldNode.numRecords,
        .treeOrder = oldNode.treeOrder
    };
    return node;
}

Node initRootNode(int numRecords, int treeOrder){
    Node node = {
        .id = 0,
        .parendId = -1,

        .numKeys = 0,
        .numChildren = 0,

        .isLeafNode = true,
        .leafId = 0,

        .numRecords = numRecords,
        .treeOrder = treeOrder
    };
    return node;
}
//Tree functions
//Read functions
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

Register *readRegistersFromLeaf(FILE *file, Node *leaf){
    Register *leafRegisters = realloc(NULL, sizeof(Register)*(leaf->numKeys));

        for (int i = 0; i < leaf->numKeys; ++i)
        {
            leafRegisters[i] = readRegister(file, leaf->numRecords);
        }

    return leafRegisters;
}

Node getNode(long nodeId)
{
    Node node = {.numKeys = 0, .numChildren = 0};
    int numKeys, numChildren;
    FILE *nodeFile;
    char *fileName = getFileName(node.id, false);

    nodeFile = fopen(fileName, "r");

    // Gets node info
    fscanf(nodeFile, "%ld\t%ld\t%d\t%d\t%ld\t%d\t%d", &node.id, &node.parendId, &numKeys, &numChildren, &node.leafId, &node.numRecords, &node.treeOrder);
    node.isLeafNode = (node.leafId != -1);


    for (int i = 0; i < numKeys; ++i)
    {
        unsigned long long int rKey;
        fscanf(nodeFile, "%lld", &rKey);
        insertRegisterKeyIntoNode(&node, rKey);
    }

    // Gets children
    if (!node.isLeafNode && numChildren > 0)
    {
        for (int i = 0; i < numChildren; ++i)
        {
            long childNodeId;
            fscanf(nodeFile, "%ld", &childNodeId);
            insertChildKeyIntoNode(&node, childNodeId);
        }
    }

    fclose(nodeFile);
    free(fileName);

    return node;
}

// Store functions
void storeRegisterOnLeaf(FILE* file, int numRecords, Register regist){
    fprintf(file, "%lld\n",regist.key);
    for (int i = 0; i < numRecords; ++i)
    {
        fprintf(file, "%s\t",regist.records[i]);
    }

    fprintf(file, "\n");
}

void storeLeafNode(Register newRegist, Node leaf){
    FILE *oldLeafFile, *newLeafFile;
    char *newFileName = "new_leaf_file.txt";
    char *fileName = getFileName(leaf.leafId, true);
    
    newLeafFile = fopen(newFileName,"w");
    if (leaf.numKeys <= 1)
    {
        storeRegisterOnLeaf(newLeafFile, leaf.numRecords, newRegist);
    }
    else{ // Read old leaf file and inserts new register in the right order
        oldLeafFile = fopen(fileName,"r");
        Register *leafRegisters = realloc(NULL, sizeof(Register)*(leaf.numKeys));

        leafRegisters[0] = newRegist;

        for (int i = 1; i < leaf.numKeys; ++i)
        {
            leafRegisters[i] = readRegister(oldLeafFile, leaf.numRecords);
        }
        // Sorts register leafs
        for(int i=0;i < leaf.numKeys;i++){
            for (int j=i+1; j < leaf.numKeys; j++) {
                Register r;

                if (leafRegisters[i].key > leafRegisters[j].key) {
                    r=leafRegisters[i];
                    leafRegisters[i]=leafRegisters[j];
                    leafRegisters[j]=r;
                }
            }
        }

        // Stores registers on leaf file
        for (int i = 0; i < leaf.numKeys; ++i)
        {
            storeRegisterOnLeaf(newLeafFile, leaf.numRecords, leafRegisters[i]);
        }

        free(leafRegisters);
        fclose(oldLeafFile);
    }
    fclose(newLeafFile);

    remove(fileName);
    rename(newFileName,fileName);
    free(fileName);
}

void storeNode(Node node){
    FILE *nodeFile;
    char *fileName = getFileName(node.id, false);

    nodeFile = fopen(fileName, "w");

    // Stores node info
    fprintf(nodeFile, "%ld\t%ld\t%d\t%d\t%ld\t%d\t%d\n", node.id, node.parendId, node.numKeys, node.numChildren, node.leafId, node.numRecords, node.treeOrder);

    // Stores node keys
    for (int i = 0; i < node.numKeys; ++i)
    {
        fprintf(nodeFile, "%lld\t", node.registerKeys[i]);
    }fprintf(nodeFile, "\n");

    // Stores children
    if (!node.isLeafNode && node.numChildren > 0)
    {
        for (int i = 0; i < node.numChildren; ++i)
        {
            fprintf(nodeFile, "%ld\t", node.childNodes[i]);
        }
    }

    fclose(nodeFile);
    free(fileName);
}

// Insert functions
void insertRegisterKeyIntoNode(Node *node, unsigned long long int regKey){
    if (node->numKeys == 0){
        node->registerKeys = realloc(NULL, sizeof(unsigned long long int)*(node->numKeys+1));
    }
    else{
        node->registerKeys = realloc(node->registerKeys, sizeof(unsigned long long int)*(node->numKeys+1));
    }
    node->registerKeys[node->numKeys++] = regKey;
    sortNodeRegisters(node);
}

void insertChildKeyIntoNode(Node *node, long nodeId){
    if (node->numChildren == 0){
        node->childNodes = realloc(NULL, sizeof(long)*(node->numChildren+1));
    }
    else{
        node->childNodes = realloc(node->childNodes, sizeof(long)*(node->numChildren+1));
    }
    node->childNodes[node->numChildren++] = nodeId;
    sortNodeRegisters(node);
}

void insertRegisterIntoTree(Register regst, Node *node, long *newLeafId, long *newNodeId){
    if (node->isLeafNode)
    {
        insertRegisterIntoNode(node, regst, newLeafId, newNodeId);
        return;
    }

    Node childNode;
    int k = 0;

    for (k = 0; k < node->numKeys; ++k)
    {
        if (regst.key < node->registerKeys[k])
        {
            break;
        }
    }

    childNode = getNode(node->childNodes[k]);
    insertRegisterIntoTree(regst, &childNode, newLeafId, newNodeId);
}

void insertRegisterIntoNode(Node *node, Register regst, long *newLeafId, long *newNodeId){
    insertRegisterKeyIntoNode(node, regst.key);
    if (node->isLeafNode)
    {
        storeLeafNode(regst, *node);
    }
    storeNode(*node);
    
    // Split node
    if (node->numKeys > node->treeOrder)
    {
        splitNode(node, newLeafId, newNodeId);
    }
}

void splitNode(Node *node, long *newLeafId, long *newNodeId){
    int breakPoint;
    Node leftNode = initNode(*node), rightNode = initNode(*node);
    
    rightNode.id = ++*newNodeId;

    breakPoint = node->numKeys/2 + node->numKeys%2;

    // Insert registers into correct node
    for (int i = 0; i < node->numKeys; ++i)
    {
        if (i < breakPoint)
        {
            insertRegisterKeyIntoNode(&leftNode, node->registerKeys[i]);
        }
        else
        {
            insertRegisterKeyIntoNode(&rightNode, node->registerKeys[i]);
        }
    }

    // If leaf node stores in memory
    if (node->isLeafNode)
    {
        Register *leafRegisters;
        FILE *nodeFile, *leftNodeFile, *rightNodeFile;

        // Update nodes
        rightNode.leafId = ++*newLeafId;

        // Get registers current stored
        nodeFile = fopen(getFileName(node->leafId, true), "r");
        leafRegisters = readRegistersFromLeaf(nodeFile, node);
        fclose(nodeFile);

        leftNodeFile = fopen(getFileName(leftNode.leafId, true),"w");
        rightNodeFile = fopen(getFileName(rightNode.leafId, true),"w");

        for (int i = 0; i < node->numKeys; ++i)
        {
            if (i < breakPoint)
            {
                storeRegisterOnLeaf(leftNodeFile, leftNode.numRecords, leafRegisters[i]);
            }
            else
            {
                storeRegisterOnLeaf(rightNodeFile, rightNode.numRecords, leafRegisters[i]);
            }
        }

        fclose(leftNodeFile);
        fclose(rightNodeFile);
        free(leafRegisters);
    }

    // Update parent node
    // If Root tree grows
    if (node->parendId == -1)
    {
        node->id = ++*newNodeId;
        
        node->numKeys = 0;
        node->numChildren = 0;
        free(node->registerKeys);
        free(node->childNodes);
        
        node->isLeafNode = false;
        node->leafId = -1;

        insertChildKeyIntoNode(node, leftNode.id);
        insertChildKeyIntoNode(node, rightNode.id);

        leftNode.parendId = node->id;
        rightNode.parendId = node->id;
        insertRegisterKeyIntoNode(node, rightNode.registerKeys[0]);

        storeNode(*node);
        storeNode(leftNode);
        storeNode(rightNode);
    }
    else
    {
        Node parentNode = getNode(node->parendId);
        insertRegisterKeyIntoNode(&parentNode, rightNode.registerKeys[0]);

        // Stores left and right node
        storeNode(leftNode);
        storeNode(rightNode);

        // Stores or split parent node
        if (parentNode.numKeys > parentNode.treeOrder)
        {
            splitNode(&parentNode, newLeafId, newNodeId);
        }
        else
        {
            storeNode(parentNode);
        }
    }
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
char *getFileName(long id, bool isLeafNode){
    char *fileName, *c = NULL;
    int nameSize;

    // Ge file name
    if (isLeafNode)
    {
        nameSize = snprintf(c, 0, "leaf_%ld.txt", id) + 1; // Adds endstring char
        fileName = realloc(NULL,sizeof(char)*nameSize);
        snprintf(fileName, nameSize, "leaf_%ld.txt", id);
    }
    else
    {
        nameSize = snprintf(c, 0, "node_%ld.txt", id) + 1; // Adds endstring char
        fileName = realloc(NULL,sizeof(char)*nameSize);
        snprintf(fileName, nameSize, "node_%ld.txt", id);
    }
    
    return fileName;
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

