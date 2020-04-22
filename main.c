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
#include <string.h>
#include "b_tree.h"


int main(int argc, const char * argv[]){

    FILE *outputFile, *inputFile;
    int treeOrder, numRecords;
    long newNodeId = 0, newLeafId = 0;
    char *operation;
    char c;
    unsigned long long int searchKey;
    Node root;

    // Gets output file
    outputFile=fopen(argv[1], "w");
    fclose(outputFile);

    // Opens input file
    inputFile=fopen(argv[2], "r");

    // Gets tree order and the number of records per register
    treeOrder=atoi(argv[3]);
    numRecords=atoi(argv[4])-1; // Minus the key

    // Init root node
    root = initRootNode(numRecords, treeOrder);

    // Get first operation
    operation = readInputString(inputFile, 5);

    while(operation[0] == 'a' || operation[0] == 's' || operation[0] == 'd'){
        if(operation[0] == 'a'){
            Register newRegister;
            newRegister = readRegister(inputFile, root.numRecords);

            insertRegisterIntoTree(newRegister, &root, &newLeafId, &newNodeId);

            freeRegister(newRegister, root.numRecords);
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
