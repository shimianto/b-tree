//
//  queue.c
//  b-tree_xcode
//
//  Created by Arthur Senna on 2020-04-22.
//  Copyright © 2020 Arthur Senna. All rights reserved.
//

#include "queue.h"

bool isEmpty(queue* q) {
    if(q->rear == -1)
        return true;
    else
        return false;
}

queue* createQueue() {
    queue* q = malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    q->size = 0;
    q->items = realloc(NULL, sizeof(long));
    return q;
}

void enqueue(queue* q, long value){
    if(q->front == -1)
        q->front = 0;
    q->rear++;
    q->items = realloc(q->items, sizeof(long)*(++q->size));
    q->items[q->rear] = value;
}

long dequeue(queue* q){
    long item;
    if(isEmpty(q)){
//        printf("Queue is empty\n");
        item = -1;
    }
    else{
        item = q->items[q->front];
        q->front++;
        if(q->front > q->rear){
//            printf("Resetting queue\n");
            free(q->items);
            q->items = realloc(NULL, sizeof(long));
            q->size = 0;
            q->front = q->rear = -1;
        }
    }
    return item;
}

