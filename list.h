//
// Created by ranicocs on 14/09/23.
//

#ifndef NASHE_LIST_H
#define NASHE_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_COMMANDS 4096
#define CNULL NULL


typedef char *tItemC;
typedef struct tNodeC *tPosC;

struct tNodeC{
    tItemC data;
    tPosC next;
};

typedef tPosC tListC;


void CreateList(tListC* L);

tPosC next(tPosC pos,tListC L);

tPosC last(tListC L);

void InsertElement(tItemC item, tListC* L);

void RemoveElement(tItemC item, tListC* L);

void DisplayList(tListC* L);









#endif //NASHE_LIST_H
