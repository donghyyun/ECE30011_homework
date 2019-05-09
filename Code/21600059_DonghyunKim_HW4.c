#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LENGTH 7

typedef struct Node* pNode;
typedef struct Node {
    char name;
    pNode next;
}Node;

int adjArray [LENGTH][LENGTH];


void readFile();
void setAdjList(pNode adjList[], bool isTranspose);
    void insertLast(pNode node, int n);
        pNode newNode(int n);

void printAdjList(pNode adjList[]);
void freeNodes(pNode adjList[]);


int main(int argc, char * argv[]){
    printf("*********START PROGRAM*********\n\n");
    pNode orgList[LENGTH];
    pNode transList[LENGTH];

    readFile();
    
    printf("\n\t####BEFORE TRANSPOSE####\n");
    setAdjList(orgList, false);
    printAdjList(orgList);
    freeNodes(orgList);
    
    printf("\n\n\t####AFTER TRANSPOSE####\n");
    setAdjList(transList, true);
    printAdjList(transList);
    freeNodes(transList);
    
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
        int index = 0;
        for (int j = 0; j < LENGTH; j++){
            fscanf(fp, "\t%c" , &ch);
            if (ch == ' ')
                continue;
            adjArray[i][j] = (int)(ch - '0');
        }
    }
    fclose(fp);
}


pNode newNode (int n){
    pNode node = (pNode)malloc(sizeof(Node));
    node -> name = (char)('A' + n);
    node -> next = NULL;
    return node;
}

void insertLast(pNode node, int n){
    if (node -> next == NULL){
        node -> next = newNode(n);
        return;
    }

    pNode curr = node;
    while (curr -> next != NULL) {curr = curr -> next;}
    curr -> next = newNode(n);
}

void setAdjList(pNode adjList[], bool isTranspose){
    for (int i = 0; i < LENGTH; i++){
        adjList[i] = newNode(i);
        for (int j = 0; j < LENGTH; j++){
            if ((!isTranspose && adjArray[i][j] == 0) || (isTranspose && adjArray[j][i] == 0))
                continue;

            insertLast(adjList[i], j);
        }
    }
}

void printAdjList(pNode adjList[]){
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

void freeNodes(pNode adjList[]){
    for (int i = 0; i < LENGTH; i++){
        pNode toFree = adjList[i];
        pNode next = toFree -> next;

        while (next != NULL){
            free(toFree);
            toFree = next;
            next = next -> next;
        }
        free(toFree);
    }
}
