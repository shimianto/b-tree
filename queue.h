//
//  queue.h
//  b-tree_xcode
//
//  Created by Arthur Senna on 2020-04-22.
//  Copyright © 2020 Arthur Senna. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct queue {
    long *items;
    long front;
    long rear;
    long size;
}queue;

queue* createQueue(void);
void enqueue(queue* q, long value);
long dequeue(queue* q);
bool isEmpty(queue* q);

#endif /* queue_h */
