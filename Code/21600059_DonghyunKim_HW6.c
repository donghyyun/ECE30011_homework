#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define VERTEX_NUM 10
#define INF 99999


//node of adjacency list
typedef struct Node* pNode;
typedef struct Node{
    int nameIdx;
    int weight;
    pNode next;
}Node;

typedef struct Vertex* pVertex;
typedef struct Vertex{
    int nameIdx;
    bool extracted;
    int dist;
}Vertex;

typedef struct PriorityQueue* PQ;
typedef struct PriorityQueue {
    int* heap;
    int size;
}PriorityQueue;

int weightArray [VERTEX_NUM][VERTEX_NUM];
char cities [VERTEX_NUM][15];
pNode adjList[VERTEX_NUM];
pVertex verList[VERTEX_NUM];

void readFile();
void setAdjList();
    void insertLast(pNode node, int idx, int weight);
    pNode newNode(int idx, int weight);
void setVertices();

void initSource(int idx);
void relax(pVertex curr, pVertex parent, int weight);
void printGrid();

void runDijkstra();
    void enqueue(PQ Q, int idx);
    int dequeue(PQ Q);
    bool isContain(PQ Q, int idx);

void runBellmanFord();
void runFloyd();



int main(int argc, char * argv[]){
    clock_t start, end;
    printf("*********START PROGRAM*********\n\n");

    readFile();
    setAdjList();
    setVertices();


    printf(">>>Dijkstra's\n");
    start = clock();    runDijkstra();  end = clock();
    printf("\n<<<It took %.3f ms to compute shortest path between cities", (float)(end - start)); 
    printf(" with Dijkstra's algorithm as above\n\n\n");
    
   
    printf(">>>Bellman-Ford\n");
    start = clock();    runBellmanFord();  end = clock();
    printf("\n<<<It took %.3f ms to compute shortest path between cities", (float)(end - start)); 
    printf(" with Bellman-Ford algorithm as above\n\n\n");


    printf(">>>Floyd's\n");
    start = clock();    runFloyd();  end = clock();
    printf("\n<<<It took %.3f ms to compute shortest path between cities", (float)(end - start)); 
    printf(" with Floyd's algorithm as above\n\n");


    printf("\n*********PROGRAM FINISHED*********\n");
    return 0;
}

void readFile() {
    FILE *fp = fopen("hw6_2019.data", "r");
    char str[15];

    for (int i = 0; i < VERTEX_NUM; i++){
        fscanf(fp, "%s", cities[i]);
        //printf("%s\n", cities[i]);
    }

    for (int i = 0; i < VERTEX_NUM; i++){
        fscanf(fp, "%s", str);
        for (int j = 0; j < VERTEX_NUM; j++){
            fscanf(fp, "%s", str);
            if (i == j)
                weightArray[i][j] = 0;
            else if (!strcmp(str, "INF"))
                weightArray[i][j] = INF;
            else {
                weightArray[i][j] = atoi(str);
            }
        }
    }

    /*for (int i = 0; i < VERTEX_NUM; i++){
        for (int j = 0; j < VERTEX_NUM; j++)
            printf("%-5d ", weightArray[i][j]);
        printf("\n");
    }*/

    fclose(fp);
}


void insertLast(pNode node, int idx, int weight){
    //printf("\t>insert last %s|%d|%d\n", node -> name, n, weight);
    if (node -> next == NULL){
        //printf("\t>node -> next == NULL\n");
        node -> next = newNode(idx, weight);
        return;
    }

    pNode curr = node;
    while (curr -> next != NULL) {curr = curr -> next;}
    curr -> next = newNode(idx, weight);
}

pNode newNode(int idx, int weight){
    pNode node = (pNode)malloc(sizeof(Node));
    node -> nameIdx = idx;
    node -> weight = weight;
    node -> next = NULL;
    
    return node;
}


void setAdjList(){
    for (int i = 0; i < VERTEX_NUM; i++){
        adjList[i] = (pNode)malloc(sizeof(Node));
        adjList[i] -> nameIdx = i;
        adjList[i] -> next = NULL;
        //printf("after strcpy(adjList[i] -> name, cities[i]);\n");
        //printf("%s\n", adjList[i] -> name);

        for (int j = 0; j < VERTEX_NUM; j++){
            if (weightArray[i][j] > 0 && weightArray[i][j] < INF)
                insertLast(adjList[i], j, weightArray[i][j]);
        }
    }
}

void setVertices(){
    for (int i = 0; i < VERTEX_NUM; i++){
        verList[i] = (pVertex)malloc(sizeof(Vertex));
        verList[i] -> nameIdx = i;
        verList[i] -> dist = INF;
        verList[i] -> extracted = false;
    }
}



void initSource(int idx){
    for (int i = 0; i < VERTEX_NUM; i++){
        if (i != idx)
            verList[i] -> dist = INF;
        else
            verList[i] -> dist = 0;
        
        verList[i] -> extracted = false;
    }
        
}

void relax(pVertex curr, pVertex parent, int weight){
    if (curr -> dist > parent -> dist + weight)
        curr -> dist = parent -> dist + weight;
}

//print border of output
void printGrid(){
    printf("+");
    for (int i = 0; i <= VERTEX_NUM; i++)
        printf("----------+");
    printf("\n");
}

//print first border and cities(top row)
void printHead() {
    printGrid();
    printf("|          |");
    for (int i = 0; i < VERTEX_NUM; i++){
        printf("%10s", cities[i]);
        printf("|");
    }
    printf("\n");
}

//print answer of each row
void printRow(int i) {
    printGrid();
        printf("|%10s|", cities[i]);
        for (int j = 0; j < VERTEX_NUM; j++){
            printf("%10d|", verList[j] -> dist);
        }
        printf("\n");
}

void swapVertex(int* a, int* b){
    int* temp = a;
    a = b;
    b = temp;
}

void enqueue(PQ Q, int idx){
    
    Q -> heap[Q -> size] = idx;
    
    int child = Q -> size;
    int parent = (Q -> size) / 2;

    while (child > 1 && verList[Q -> heap[child]] -> dist > verList[Q -> heap[parent]] -> dist){
        swapVertex(&(Q -> heap[child]), &(Q -> heap[parent]));
        child = parent;
        parent = (parent) / 2;
    }

    Q -> size ++;
}

int dequeue(PQ Q){
    Q -> size--;

    if (Q -> size <= 0){
        return -1;
    }

    if (Q -> size == 1){
      return Q -> heap[1];
    }

    
    int toReturn = Q -> heap[1];
    Q -> heap[1] = Q -> heap[Q -> size];
    
    int curr = 1;
    int left = curr * 2;
    int right = curr * 2 + 1;
    int maxIndex = curr;

    while (left < Q -> size){
        if (verList[Q -> heap[maxIndex]] -> dist < verList[Q -> heap[left]] -> dist)
            maxIndex = left;
        if (right < Q -> size && verList[Q -> heap[maxIndex]] -> dist < verList[Q -> heap[right]] -> dist)
            maxIndex = right;

        if (maxIndex == curr)
            break;
        else {
            swapVertex(&(Q -> heap[curr]), &(Q -> heap[maxIndex]));
            curr = maxIndex;
            left = curr * 2;
            right = curr * 2 + 1;
        }
    }

    return toReturn;
}

bool isContain(PQ Q, int idx){
    for (int i = 0; i < Q -> size; i++){
        if (Q -> heap[i] == idx)
            return true;
    }

    return false;
}

void runDijkstra(){
    printHead();

    //SSP on each vertex(|V|)
    for (int i = 0; i < VERTEX_NUM; i++){
        initSource(i);
        
        PQ Q = (PQ)malloc(sizeof(PriorityQueue));
        Q -> size = 1; Q -> heap = (int *)malloc(sizeof(int) * VERTEX_NUM);  Q -> heap[0] = -1;
        enqueue(Q, i);


        while (Q -> size > 0){
            int temp = dequeue(Q);
            if (temp < 0)
                break;
            else
                verList[temp] -> extracted = true;

            pNode curr = adjList[temp] -> next;
            while(curr != NULL){
                relax(verList[curr -> nameIdx], verList[temp], curr -> weight);
                
                if (!verList[curr -> nameIdx] -> extracted && !isContain(Q, curr -> nameIdx))
                    enqueue(Q, curr -> nameIdx);
                    
                curr = curr -> next;
            }
        }

        printRow(i);
    }
    
    //print last border
    printGrid();
}


void runBellmanFord(){
    printHead();

    //SSP on each vertex(|V|)
    for (int i = 0; i < VERTEX_NUM; i++){
        initSource(i);

        //Relaxation: Make |V| - 1 passes
        for (int j = 0; j < VERTEX_NUM - 1; j++){
            
            //Relaxing each edge (the order of edge is determined by BFS order: starting from source)
            for (int k = 0; k < VERTEX_NUM; k++){
                pNode curr = adjList[k] -> next;
                while(curr != NULL){
                    relax(verList[curr -> nameIdx], verList[k], curr -> weight);                    
                    curr = curr -> next;
                }
            }
        }
        
        printRow(i);
    }

    //print last border
    printGrid();
}


void setDistArray(int arr [VERTEX_NUM][VERTEX_NUM]){
    for (int i = 0; i < VERTEX_NUM; i++)
        for (int j = 0; j < VERTEX_NUM; j++)
            arr[i][j] = weightArray[i][j];
}

void runFloyd(){
    int distArray [VERTEX_NUM][VERTEX_NUM];
    setDistArray(distArray);

    printHead();

    for (int k = 0; k < VERTEX_NUM; k++){
        for (int i = 0; i < VERTEX_NUM; i++){
            for (int j = 0; j < VERTEX_NUM; j++){
                if (distArray[i][j] > distArray[i][k] + distArray[k][j])
                    distArray[i][j] = distArray[i][k] + distArray[k][j];
            }
        }
    }

    //for floyd-warshall algorithm I simply printed the value in 'distArray'
    for (int i = 0; i < VERTEX_NUM; i++){
        printGrid();
        printf("|%10s|", cities[i]);

        for (int j = 0; j < VERTEX_NUM; j++)
            printf("%10d|", distArray[i][j]);
        
        printf("\n");
    }
    printGrid();
}