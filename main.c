//
//  main.c
//  b-tree_xcode
//
//  Created by Arthur Senna on 2020-04-22.
//  Copyright © 2020 Arthur Senna. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "b_tree.h"
#include "queue.h"

int main(int argc, const char * argv[]){

    FILE *outputFile, *inputFile;
    int treeOrder, recordSize;
    long newNodeId = 0, newLeafId = 0;
    char *operation;
    char c;
    unsigned long long int searchKey;
    Node root;

    // Gets output file
    outputFile=fopen(argv[1], "w");

    // Opens input file
    inputFile=fopen(argv[2], "r");

    // Gets tree order and the number of records per register
    treeOrder=atoi(argv[3]);
    recordSize=atoi(argv[4])-1; // Minus the key

    // Init root node
    root = initRootNode(recordSize, treeOrder);
    updatesRootId(root.id);

    // Get first operation
    operation = readInputString(inputFile, 5);

    while(operation[0] == 'a' || operation[0] == 's' || operation[0] == 'd'){
        if(operation[0] == 'a'){
            // Gets register and insert it into tree
            Register newRegister = readRegister(inputFile, root.recordSize);
            insertRegisterIntoTree(newRegister, &root, &newLeafId, &newNodeId);
            freeRegister(newRegister, root.recordSize);
            
            // Updates root
            long rootId = getRootId();
            root = getNode(rootId);
        }
        else if (operation[0] == 's')
        {
            Register readRegister;
            // Get key from register to be searchef
            fscanf(inputFile, "%lld%c",&searchKey,&c);
            readRegister = searchRegisterInTree(searchKey, root);
            
            printRegisterOnFile(outputFile, readRegister, recordSize);
            
            freeRegister(readRegister, recordSize);
        }
        else if (operation[0] == 'd'){
            fprintf(outputFile, "dump\n");
            dump(outputFile, root.id, newNodeId+1);
            fprintf(outputFile, "dump\n");
        }
        else
        {
            printf("Unidentified operation: '%s'.\n", operation);
        }

        free(operation);
        operation = readInputString(inputFile, 5);
    }

    free(operation);

    fclose(inputFile);
    fclose(outputFile);
    
    dropDatabase(newNodeId+1, newLeafId+1);
    
    return 0;
}
