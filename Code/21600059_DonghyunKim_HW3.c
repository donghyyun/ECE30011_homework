//My program takes approximately 15 minutes!!!
//My program takes approximately 15 minutes!!!
//My program takes approximately 15 minutes!!!
//My program takes approximately 15 minutes!!!
//My program takes approximately 15 minutes!!!
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#define V_UPPER 300
#define W_UPPER 100

typedef int (*Comp) (const void *, const void *);
typedef struct Item* pItem;
typedef struct Item {
    int value;
    int weight;
    float valPerWei;
}Item;

typedef struct Node* pNode;
typedef struct Node {
    int depth;
    int value;
    int weight;
    float bound;
}Node;

typedef struct PriorityQueue* PQ;
typedef struct PriorityQueue {
    pNode* heap;
    int size;
    int capacity;
}PriorityQueue;

pNode newNode(pNode parent, bool left, int maxValue);
void swapNode(pNode a, pNode b);
void enqueue (PQ Q, pNode node);
void dequeue (PQ Q, pNode deq);

int comp(const void* a, const void* b);
void setItems(int i);
void runAlgorithms();

float Gknapsack();
int DPknapsack();
int BBknapsack();
    float calBound(pNode node);


int numOfItems[] = {10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000};
pItem items;
Comp compare = comp;
int numOfItem = 0;
int maxWeight = 0;
clock_t timerStart;
////////////////////////////
//the limitation for time (sec)
float timeLimit = 900.0;
////////////////////////////
FILE* fp;


int main(int argc, char * argv[]){
    srand(time(NULL));

    int N = sizeof(numOfItems) / sizeof(int);
    fp = fopen("output.txt", "w");
    fprintf(fp, "Number of |                       Processing time(ms)/Maximum benefit value                      |\n");
    fprintf(fp, "          |--------------------------------------------------------------------------------------|\n");
    fprintf(fp, "  Items   |           Greedy           |            D.P.            |            B.&B.           |\n");
    fprintf(fp, "----------|--------------------------------------------------------------------------------------|\n");

    printf("*********START PROGRAM*********\n\n");

    //do computation from 10 to 10000
    for (int i = 0; i < 3; i++){
        sleep(1);
        numOfItem = numOfItems[i];
        printf("Number of Items: %d\n", numOfItem);
        fprintf(fp, "%10d|", numOfItem);

        //set 'maxWeight'
        maxWeight = numOfItem * 40;

        //set items and run three algorithms
        setItems(i);
        runAlgorithms();
    }
    free(items);

    fprintf(fp, "--------------------------------------------------------------------------------------------------\n");
    fclose(fp);

    printf("************PROGRAM FINISHED************\n\n");

    return 0;
}


//Comparision function : to sort 'items' according its valPerWei(= value / weight)
int comp(const void* a, const void* b){
    Item first = *(Item *) a;
    Item second = *(Item *) b;

    if (first.valPerWei > second.valPerWei)
        return -1;
    else if (first.valPerWei < second.valPerWei)
        return 1;
    else
        return 0;
}

//Set each items' value and weight
void setItems(int i){
    if (i != 0)
        items = (pItem)realloc(items, sizeof(Item) * numOfItem);
    else
        items = (pItem)malloc(sizeof(Item) * numOfItem);


    for (int j = 0; j < numOfItem; j++){
        items[j].value = (rand() % V_UPPER) + 1.0;
        items[j].weight = (rand() % W_UPPER) + 1.0;
        items[j].valPerWei = (float)items[j].value / (float)items[j].weight;
    }

    qsort(items, numOfItem, sizeof(Item), compare);
}

//This function run specific algorithms and check the time taken
void runAlgorithms(){
    clock_t start, end;
    float timeTaken = 0.0;
    float value = 0.0;
    timerStart = clock();


    printf("  -> Greedy\n");
    start = clock();    value = Gknapsack(start);    end = clock();
    timeTaken = (float) (end - start) / (CLOCKS_PER_SEC / 1000);
    if (value < 0)
        fprintf(fp, "          Time Over         |");
    else
        fprintf(fp, "%10.3fms/%-15.3f|", timeTaken, value);


    printf("    -> D.P\n");
    start = clock();    value = DPknapsack(start);    end = clock();
    timeTaken = (float) (end - start) / (CLOCKS_PER_SEC / 1000);
    if (value < 0)
        fprintf(fp, "          Time Over         |");
    else
        fprintf(fp, "%15.3fms/%-10d|", timeTaken, (int)value);


    printf("      -> B&B\n\n");
    start = clock();    value = BBknapsack(start);    end = clock();
    timeTaken = (float) (end - start) / (CLOCKS_PER_SEC / 1000);
    if (value < 0)
        fprintf(fp, "          Time Over         |\n");
    else
        fprintf(fp, "%15.3fms/%-10d|\n", timeTaken, (int)value);
}

//Solve knapsack problem by Greedy strategy
float Gknapsack(){
    float value = 0;
    float totalWeight = 0;

    for (int i = 0; i < numOfItem; i++){
        if ((float) (clock() - timerStart) / CLOCKS_PER_SEC > timeLimit)
            return -1;

        if (totalWeight + items[i].weight > maxWeight){
            value += (maxWeight - totalWeight) * items[i].valPerWei;
            break;
        }
        value += items[i].value;
        totalWeight += items[i].weight;
    }
    return value;
}

//Solve knapsack problem by Dynimic Programming strategy
int DPknapsack(){
    int * pre_value = (int *)malloc(sizeof(int) * (maxWeight + 1));
    int * now_value = (int *)malloc(sizeof(int) * (maxWeight + 1));
    int * temp;
    int maxValue = 0;

    for (int i = 0; i <= maxWeight; i++){
        pre_value[i] = 0;
        now_value[i] = 0;
    }


    for (int i = 0; i < numOfItem; i++){
        if ((float) (clock() - timerStart) / CLOCKS_PER_SEC > timeLimit)
            return -1;

        for (int w = 1; w <= maxWeight; w++){
            if (items[i].weight <= w){
                if (items[i].value + pre_value[w - items[i].weight] > pre_value[w])
                    now_value[w] = items[i].value + pre_value[w - items[i].weight];
                else
                    now_value[w] = pre_value[w];
            }
            else
                now_value[w] = pre_value[w];
        }

        if (i != numOfItem - 1){
            temp = pre_value;
            pre_value = now_value;
            now_value = temp;
        }
    }
    maxValue = now_value[maxWeight];

    free(pre_value);
    free(now_value);
    return maxValue;
}

//Solve knapsack problem by Branch and Bound strategy
int BBknapsack(){
    pNode root = (pNode)malloc(sizeof(Node));
    root -> value = 0;  root -> weight = 0; root -> depth = 0;  root -> bound = calBound(root);


    PQ Q = (PQ)malloc(sizeof(PriorityQueue));
    Q -> size = 1;  Q -> capacity = 10; Q -> heap = (pNode*)malloc(sizeof(pNode) * Q -> capacity);  Q -> heap[0] = NULL;
    enqueue(Q, root);

    pNode temp;
    pNode right, left;

    int maxValue = 0;

    while (Q -> size > 0){

        if ((float) (clock() - timerStart) / CLOCKS_PER_SEC > timeLimit)
            break;

        temp = (pNode)malloc(sizeof(Node));
        dequeue(Q, temp);

        if (temp -> value > maxValue)
            maxValue = temp -> value;

        left = newNode(temp, true, maxValue);
        right = newNode(temp, false, maxValue);


        if (left != NULL)
            enqueue(Q, left);
        if (right != NULL)
            enqueue(Q, right);

        free(temp);
    }

    if (Q -> size > 0)
        maxValue = -1;

    free(Q -> heap);
    free(Q);

    return maxValue;
}

//Do computation for variable 'bound' in 'Node'
float calBound(pNode node){
    int tot_weight = node -> weight;
    float bound = node -> value;
    int index = node -> depth;

    while (index < numOfItem && tot_weight + items[index].weight < maxWeight) {
        tot_weight += items[index].weight;
        bound += (float)items[index].value;
        index++;
    }

    if (index < numOfItem)
        bound += (float)(maxWeight - tot_weight) * items[index].valPerWei;

    return bound;
}

//Create new 'Node' and set its local variable 
pNode newNode(pNode parent, bool left, int maxValue){
    if (parent -> value < 0 || parent -> depth == numOfItem) {return NULL;}

    pNode node = (pNode)malloc(sizeof(Node));
    node -> depth = parent -> depth + 1;
    node -> value = parent -> value;
    node -> weight = parent -> weight;

    if (left){
        node -> value += items[parent -> depth].value;
        node -> weight += items[parent -> depth].weight;
        node -> bound = parent -> bound;
        if (node -> weight > maxWeight) {free(node); return NULL;}
    }
    else{
        node -> bound = calBound(node);
        if (node -> bound <= maxValue) {free(node); return NULL;}
    }

    return node;
}

//Swap all value of variable between two 'Nodes'
void swapNode(pNode a, pNode b){
    pNode temp = (pNode)malloc(sizeof(Node));
    memcpy(temp, a, sizeof(Node));
    memcpy(a, b, sizeof(Node));
    memcpy(b, temp, sizeof(Node));
    free(temp);
}

//Enqueue 'Node' in 'PriorityQueue'
void enqueue(PQ Q, pNode node){
    if (Q -> capacity == Q -> size){
        Q -> capacity *= 2;
        Q -> heap = (pNode*)realloc(Q -> heap, sizeof(pNode) * Q -> capacity);
    }

    Q -> heap[Q -> size] = (pNode)malloc(sizeof(Node));
    memcpy(Q -> heap[Q -> size], node, sizeof(Node));
    free(node);

    int child = Q -> size;
    int parent = (Q -> size) / 2;

    while (child > 1 && Q -> heap[child] -> bound > Q -> heap[parent] -> bound){
        swapNode(Q -> heap[child], Q -> heap[parent]);
        child = parent;
        parent = (parent) / 2;
    }

    Q -> size ++;
}

//Dequeue 'Node' from 'PriorityQueue'
void dequeue (PQ Q, pNode deq){
    Q -> size--;

    if (Q -> size <= 0){
        deq -> value = -1;
        return;
    }

    if (Q -> size == 1){
      memcpy(deq, Q -> heap[1], sizeof(Node));
      free(Q -> heap[Q -> size]);
      return;
    }

    memcpy(deq, Q -> heap[1], sizeof(Node));
    memcpy(Q -> heap[1], Q -> heap[Q -> size], sizeof(Node));
    free(Q -> heap[Q -> size]);
    
    int curr = 1;
    int left = curr * 2;
    int right = curr * 2 + 1;
    int maxIndex = curr;

    while (left < Q -> size){
        if (Q -> heap[maxIndex] -> bound < Q -> heap[left] -> bound)
            maxIndex = left;
        if (right < Q -> size && Q -> heap[maxIndex] -> bound < Q -> heap[right] -> bound)
            maxIndex = right;

        if (maxIndex == curr)
            break;
        else {
            swapNode(Q -> heap[curr], Q -> heap[maxIndex]);
            curr = maxIndex;
            left = curr * 2;
            right = curr * 2 + 1;
        }
    }
}
