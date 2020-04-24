//
//  b_tree.c
//  b-tree_xcode
//
//  Created by Arthur Senna on 2020-04-22.
//  Copyright © 2020 Arthur Senna. All rights reserved.
//

#include "b_tree.h"

// Init functions
Node initNode(Node oldNode){
    Node node = {
        .id = oldNode.id,
        .parendId = oldNode.parendId,

        .numKeys = 0,
        .numChildren = 0,

        .isLeafNode = oldNode.isLeafNode,
        .leafId = oldNode.leafId,

        .recordSize = oldNode.recordSize,
        .treeOrder = oldNode.treeOrder
    };
    return node;
}

Node initRootNode(int recordSize, int treeOrder){
    Node node = {
        .id = 0,
        .parendId = -1,

        .numKeys = 0,
        .numChildren = 0,

        .isLeafNode = true,
        .leafId = 0,

        .recordSize = recordSize,
        .treeOrder = treeOrder
    };
    return node;
}

void updatesRootId(long newId){
    FILE *rootFile;
    
    rootFile = fopen("root.txt", "w");
    fprintf(rootFile, "%ld", newId);
    fclose(rootFile);
}

long getRootId(void){
    FILE *rootFile;
    long rootId;
    
    rootFile = fopen("root.txt", "r");
    fscanf(rootFile, "%ld", &rootId);
    fclose(rootFile);
    
    return rootId;
}
//Tree functions
//Read functions
Register readRegister(FILE* inputFile, int recordSize){
    Register newRegister = {.key = -1};

    // Gets register's key and skip tab or newline char
    fscanf(inputFile, "%lld\t",&newRegister.key);

    newRegister.records = realloc(NULL, sizeof(char*)*recordSize);
    newRegister.hasRecords = true;

    for (int i = 0; i < recordSize; ++i)
    {
        newRegister.records[i] = readInputString(inputFile,10);
    }

    return newRegister;
}

Register *readRegistersFromLeafFile(FILE *file, Node leaf){
    Register *leafRegisters = realloc(NULL, sizeof(Register)*(leaf.numKeys));

    for (int i = 0; i < leaf.numKeys; ++i)
        {
            leafRegisters[i] = readRegister(file, leaf.recordSize);
        }

    return leafRegisters;
}

Node getNode(long nodeId)
{
    Node node = {.numKeys = 0, .numChildren = 0};
    int numKeys, numChildren;
    FILE *nodeFile;
    char *fileName = getFileName(nodeId, false);

    nodeFile = fopen(fileName, "r");

    // Gets node info
    fscanf(nodeFile, "%ld\t%ld\t%d\t%d\t%ld\t%d\t%d", &node.id, &node.parendId, &numKeys, &numChildren, &node.leafId, &node.recordSize, &node.treeOrder);
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

Register getRegisterFromLeaf(unsigned long long int rKey, Node leafNode){
    FILE *leafFile;
    Register r = {.key = -1}, *leafRegisters;
    char *fileName = getFileName(leafNode.leafId, true);
    
    leafFile = fopen(fileName, "r");
    
    leafRegisters = readRegistersFromLeafFile(leafFile, leafNode);
    
    for (int i = 0; i < leafNode.numKeys; ++i) {
        if (leafRegisters[i].key == rKey) {
            r = leafRegisters[i];
            break;
        }
    }
    
    fclose(leafFile);
    free(fileName);
    free(leafRegisters);
    return r;
}

// Store functions
void storeRegisterOnFile(FILE* file, int recordSize, Register regist){
    fprintf(file, "%lld\t",regist.key);
    for (int i = 0; i < recordSize; ++i)
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
        storeRegisterOnFile(newLeafFile, leaf.recordSize, newRegist);
    }
    else{ // Read old leaf file and inserts new register in the right order
        oldLeafFile = fopen(fileName,"r");
        Register *leafRegisters = realloc(NULL, sizeof(Register)*(leaf.numKeys));

        leafRegisters[0] = newRegist;

        for (int i = 1; i < leaf.numKeys; ++i)
        {
            leafRegisters[i] = readRegister(oldLeafFile, leaf.recordSize);
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
            storeRegisterOnFile(newLeafFile, leaf.recordSize, leafRegisters[i]);
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
    fprintf(nodeFile, "%ld\t%ld\t%d\t%d\t%ld\t%d\t%d\n", node.id, node.parendId, node.numKeys, node.numChildren, node.leafId, node.recordSize, node.treeOrder);

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
    freeNode(&childNode);
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
    
    for (int i = 0; i < node->numChildren; ++i)
    {
        if (i < breakPoint)
        {
            insertChildKeyIntoNode(&leftNode, node->childNodes[i]);
        }
        else if (i == breakPoint){
            insertChildKeyIntoNode(&leftNode, node->childNodes[i]);
            insertChildKeyIntoNode(&rightNode, node->childNodes[i]);
        }
        else
        {
            insertChildKeyIntoNode(&rightNode, node->childNodes[i]);
        }
    }

    // If leaf node stores in memory
    if (node->isLeafNode)
    {
        Register *leafRegisters;
        FILE *nodeFile, *leftNodeFile, *rightNodeFile;
        char *nodeFileName, *leftNodeFileName, *rightNodeFileName;
        
        // Update nodes
        rightNode.leafId = ++*newLeafId;
        
        nodeFileName = getFileName(node->leafId, true);
        leftNodeFileName = getFileName(leftNode.leafId, true);
        rightNodeFileName = getFileName(rightNode.leafId, true);

        // Get registers current stored
        nodeFile = fopen(nodeFileName, "r");
        leafRegisters = readRegistersFromLeafFile(nodeFile, *node);
        fclose(nodeFile);

        leftNodeFile = fopen(leftNodeFileName,"w");
        rightNodeFile = fopen(rightNodeFileName,"w");

        for (int i = 0; i < node->numKeys; ++i)
        {
            if (i < breakPoint)
            {
                storeRegisterOnFile(leftNodeFile, leftNode.recordSize, leafRegisters[i]);
            }
            else
            {
                storeRegisterOnFile(rightNodeFile, rightNode.recordSize, leafRegisters[i]);
            }
        }

        fclose(leftNodeFile);
        fclose(rightNodeFile);
        free(leafRegisters);
        free(nodeFileName);
        free(leftNodeFileName);
        free(rightNodeFileName);
    }
    else{
        for (int i = 0; i < rightNode.numChildren; i++) {
            Node childNode = getNode(rightNode.childNodes[i]);
            childNode.parendId = rightNode.id;
            storeNode(childNode);
            freeNode(&childNode);
        }
    }

    // Update parent node
    // If Root tree grows
    if (node->parendId == -1)
    {
        node->id = ++*newNodeId;
        
        freeNode(node);
        
        node->isLeafNode = false;
        node->leafId = -1;

        insertChildKeyIntoNode(node, leftNode.id);
        insertChildKeyIntoNode(node, rightNode.id);

        leftNode.parendId = node->id;
        rightNode.parendId = node->id;
        insertRegisterKeyIntoNode(node, rightNode.registerKeys[0]);

        storeNode(leftNode);
        storeNode(rightNode);

        sortNodesChildren(node);
        storeNode(*node);
        updatesRootId(node->id);
    }
    else
    {
        Node parentNode = getNode(node->parendId);
        insertRegisterKeyIntoNode(&parentNode, rightNode.registerKeys[0]);
        insertChildKeyIntoNode(&parentNode, rightNode.id);

        // Stores left and right node
        storeNode(leftNode);
        storeNode(rightNode);
        
        sortNodesChildren(&parentNode);
        storeNode(parentNode);

        // Stores or split parent node
        if (parentNode.numKeys > parentNode.treeOrder)
        {
            splitNode(&parentNode, newLeafId, newNodeId);
        }
    }
    
    freeNode(&leftNode);
    freeNode(&rightNode);
}

void printRegisterOnFile(FILE *outputFile, Register r, int recordSize){
    fprintf(outputFile, "search\n");
    
    if (r.key == -1) {
        fprintf(outputFile, "null\n");
    }
    else {
        storeRegisterOnFile(outputFile, recordSize, r);
    }
    
    fprintf(outputFile, "search\n");
}

// Search functions
Register searchRegisterInTree(unsigned long long int regstKey, Node node){
    Register notFound = {.key = -1};
    Register r;
    
    if (node.isLeafNode){
        for (int i = 0; i < node.numKeys; ++i) {
            if (regstKey == node.registerKeys[i]) {
                return getRegisterFromLeaf(regstKey, node);
            }
        }
    }
    else{
        for (int i = 0; i < node.numChildren; ++i) {
            Node childNode = getNode(node.childNodes[i]);
            r = searchRegisterInTree(regstKey, childNode);
            // Found node
            if (r.key != -1) {
                return r;
            }
        }
    }
    
    return notFound;
}

void dump(FILE *outputFile, long rootId, long numNodes) {

    queue* q = createQueue();
    bool *visited = realloc(NULL, sizeof(bool)*numNodes);
    
    for (int i = 0; i < numNodes; ++i) {
        visited[i] = false;
    }
    
    visited[rootId] = true;
    enqueue(q, rootId);
    
    while(!isEmpty(q)){
        long currentId = dequeue(q);
        Node currentNode =  getNode(currentId);
        
        if (currentNode.id == -1) {
            printf("Error finding node %ld\n", currentId);
            return;
        }
        // Print in output file
        for (int i = 0;i < currentNode.numKeys; ++i) {
            fprintf(outputFile, "%lld,", currentNode.registerKeys[i]);
        }fprintf(outputFile, "\n");
        
        // Get tem vector
        int k = 0, tempSize = currentNode.numChildren;
        long* temp = realloc(NULL, sizeof(long)*(tempSize));
        
        for (int i = 0; i < tempSize; ++i) {
            temp[i] = currentNode.childNodes[i];
        }

       while(k < tempSize) {
            long nextId = temp[k];

            if(visited[nextId] == false){
                visited[nextId] = true;
                enqueue(q, nextId);
            }
           k++;
       }
        
        free(temp);
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

void freeRegister(Register r, int recordSize){
    if (r.hasRecords) {
        for (int i = 0; i < recordSize; ++i)
        {
            free(r.records[i]);
        }
    }
    free(r.records);
}

void freeNode(Node *node){
    free(node->childNodes);
    free(node->registerKeys);
    node->numChildren = 0;
    node->numKeys = 0;
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

void sortNodesChildren(Node *node){
    if (node->numChildren == 0) {
        return;
    }
    // Get all child nodes
    Node *childrenNodes = realloc(NULL, sizeof(Node)*node->numChildren);
    for (int k = 0; k < node->numChildren; ++k) {
        Node n = getNode(node->childNodes[k]);
        childrenNodes[k] = n;
    }
    // Sort child nodes
    for(int i = 0;i<node->numChildren;i++){
        for (int j = i+1; j<node->numChildren; j++) {
            Node aux; long auxId;

            if (childrenNodes[i].registerKeys[0] > childrenNodes[j].registerKeys[0]) {
                aux = childrenNodes[i];
                auxId = node->childNodes[i];

                childrenNodes[i] = childrenNodes[j];
                node->childNodes[i] = node->childNodes[j];

                childrenNodes[j] = aux;
                node->childNodes[j] = auxId;
            }
        }
    }
    free(childrenNodes);
}

void dropDatabase(long numNodes, long numLeafs){
    remove("root.txt");
    
    for (int i=0; i<numLeafs; ++i) {
        char *fileName = getFileName(i, true);
        remove(fileName);
        free(fileName);
    }
    
    for (int i=0; i<numNodes; ++i) {
        char *fileName = getFileName(i, false);
        remove(fileName);
        free(fileName);
    }
    
}
