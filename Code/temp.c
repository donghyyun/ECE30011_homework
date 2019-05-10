#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 7

typedef struct Node* pNode;
typedef struct Node {
    char name;
    pNode next;
}Node;

pNode adjList[LENGTH];

void readFile();
void insertLast(pNode node, char ch);
pNode newNode(char ch);
void printAdjList();
void freeNodes();

int main(int argc, char * argv[]){
    printf("*********START PROGRAM*********\n\n");

    readFile();
    
    printf("\n\t####BEFORE TRANSPOSE####\n");
    printAdjList();

    printf("\n\n\t####AFTER TRANSPOSE####\n");
    transpose();
    //printAdjList();
    
    printf("\n*********PROGRAM FINISHED*********\n");
    return 0;
}


void readFile() {
    FILE *fp = fopen("hw4.data", "r");
    char ch;

    do{
        fscanf(fp, "\t%c" , &ch);
    }while (ch != 'G');

    for (int i = 0; i < LENGTH; i++){
        fscanf(fp, "\t%c" , &ch);
        adjList[i] = newNode(ch);
        for (int j = 0; j < LENGTH; j++){
            fscanf(fp, "\t%c" , &ch);
            if (ch == ' ' || ch == '0')
                continue;
            insertLast(adjList[i] ,(char)('A' + j));
        }
    }
    fclose(fp);
}

pNode newNode (char ch){
    pNode node = (pNode)malloc(sizeof(Node));
    node -> name = ch;
    node -> next = NULL;
    return node;
}

void insertLast(pNode node, char ch){
    if (node -> next == NULL){
        node -> next = newNode(ch);
        return;
    }

    pNode curr = node;
    while (curr -> next != NULL) {curr = curr -> next;}
    curr -> next = newNode(ch);
}

void printAdjList(){
    pNode curr;
    printf("\tadjVertices\n");
    printf("\t   ___\n");

    for (int i = 0; i < LENGTH; i++){
        curr = adjList[i];
        printf("\t%c |  -|-> ", curr -> name);

        while (curr -> next != NULL){
            curr = curr -> next;
            printf("|%c|-|-> ", curr -> name);
        }

        printf("nil\n");
        if (i != LENGTH - 1)
            printf("\t  |---|\n");
    }
    printf("\t   ---\n");
}

void freeNodes(){
    for (int i = 0; i < LENGTH; i++){
        if (adjList[i] -> next == NULL)
            continue;

        pNode toFree = adjList[i] -> next;
        pNode next = toFree -> next;

        while (next != NULL){
            free(toFree);
            toFree = next;
            next = next -> next;
        }
        free(toFree);
    }
}