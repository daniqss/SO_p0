//
// Created by ranicocs on 14/09/23.
//

#include "list.h"

void CreateList(tListC* L){
    L = CNULL;
}
tPosC next(tPosC pos,tListC L){
    return pos->next;
}

tPosC last(tListC L){
    tPosC pos;
    for(pos=L; next(pos,L)!=CNULL; next(pos,L));
    return pos;
}
bool createNodeC(){

}
void InsertElement(tItemC item, tListC* L){

}

void RemoveElement(tItemC item, tListC* L){

}

void DisplayList(tListC* L);