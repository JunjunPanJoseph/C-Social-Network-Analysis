// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static void listSort(PredNode *Node){
    if (Node == NULL) return;
    PredNode *tmp = Node;
    PQ pq = newPQ();
    ItemPQ tmpItem;
    while (tmp != NULL){
        tmpItem.key = tmp->v;
        tmpItem.value = 2147483647 - tmp->v;
        addPQ(pq, tmpItem);
        
        tmp = tmp->next;
    }    
    tmp = Node;
    while (tmp != NULL){
        tmpItem = dequeuePQ(pq);
        tmp->v = tmpItem.key;
        tmp = tmp->next;
    }
    freePQ(pq);
}
static void listSort2(AdjList Node){
    if (Node == NULL) return;
    AdjList tmp = Node;
    PQ pq = newPQ();
    ItemPQ tmpItem;
    while (tmp != NULL){
        tmpItem.key = tmp->weight;
        tmpItem.value = tmp->w;
        addPQ(pq, tmpItem);
        
        tmp = tmp->next;
    }    
    tmp = Node;
    while (tmp != NULL){
        tmpItem = dequeuePQ(pq);
        tmp->weight = tmpItem.key;
        tmp->w = tmpItem.value;
        tmp = tmp->next;
    }
    freePQ(pq);
}
static PredNode *makePredNode(int v, struct PredNode *next){
    PredNode *tmp = malloc(sizeof(PredNode));
    tmp->v = v;
    tmp->next = next;
    return tmp;
}
ShortestPaths dijkstra(Graph g, Vertex v) {
    listSort(NULL);
    listSort2(NULL);
	ShortestPaths p;
    p.noNodes = numVerticies(g);
    p.src = v;
    p.dist = malloc(p.noNodes * (sizeof(int)));
    int *tmpDist = malloc(p.noNodes * (sizeof(int)));
    p.pred = malloc(p.noNodes * (sizeof(PredNode *)));
    for (int i = 0; i < p.noNodes; i++){
        p.pred[i] = NULL;
        p.dist[i] = 2147483647;
        tmpDist[i] = 2147483647;
    }
    PQ pq = newPQ();
    ItemPQ tmpItem;
    tmpItem.key = v;
    tmpItem.value = 0;
    addPQ(pq, tmpItem);
    p.dist[v] = 0;
    tmpDist[v] = 0;
    AdjList tmpList = NULL;
    int currDist;
    int currNode;
    while (!PQEmpty(pq)){
        tmpItem = dequeuePQ(pq);
        tmpList = outIncident(g, tmpItem.key);
        listSort2(tmpList);
        //currNode = tmpItem.key;
        currDist = tmpItem.value;
        while (tmpList != NULL){
            if (currDist + tmpList->weight < p.dist[tmpList->w]){
                tmpItem.key = tmpList->w;
                tmpItem.value = currDist + tmpList->weight;
                if (p.dist[tmpList->w] == 2147483647){
                    addPQ(pq, tmpItem);
                } else {
                    updatePQ(pq, tmpItem);
                }
                p.dist[tmpList->w] = currDist + tmpList->weight;
            } 
            tmpList = tmpList->next;
        }
    }
    tmpItem.key = v;
    tmpItem.value = 0;
    addPQ(pq, tmpItem);

    while (!PQEmpty(pq)){
        tmpItem = dequeuePQ(pq);
        tmpList = outIncident(g, tmpItem.key);
        currNode = tmpItem.key;
        currDist = tmpItem.value;
        while (tmpList != NULL){
            if (p.dist[tmpList->w] != 2147483647 && 
                currDist + tmpList->weight == p.dist[tmpList->w]){
                //printf("pre = %d i = %d dist = %d\n",currNode, tmpList->w,  currDist + tmpList->weight);
                p.pred[tmpList->w] = makePredNode(currNode, p.pred[tmpList->w]);
            }
            if (currDist + tmpList->weight < tmpDist[tmpList->w]){
                tmpItem.key = tmpList->w;
                tmpItem.value = currDist + tmpList->weight;
                if (tmpDist[tmpList->w] == 2147483647){
                    addPQ(pq, tmpItem);
                } else {
                    updatePQ(pq, tmpItem);
                }
                tmpDist[tmpList->w] = currDist + tmpList->weight;
            } 
            tmpList = tmpList->next;
        }
    }
    for (int i = 0; i < p.noNodes; i++){
        if (p.dist[i] == 2147483647){
            p.dist[i] = 0;
        }
        //listSort(p.pred[i]);
    }
    
    
    free(tmpDist);
    freePQ(pq);
	return p;
}
static void printList(struct PredNode *n){
    if (n == NULL){
        printf("NULL");
        return;
    } else {
        printf("[%d]->", n->v);
        printList(n->next);
    }
}
void showShortestPaths(ShortestPaths paths) {
    printf("Node %d\n", paths.src);
    printf("  Distance\n");
    for (int i = 0; i < paths.noNodes; i++){
        if (paths.pred[i] != NULL){
            printf("    %d : %d\n", i, paths.dist[i]);
        } else {
            printf("    %d : X\n", i);
        }
    }
    printf("  Preds\n");
    for (int i = 0; i < paths.noNodes; i++){
        printf("    %d : ", i);
        printList(paths.pred[i]);
        printf("\n");
    }
}

static void freeList(struct PredNode *n){
    if (n == NULL) return;
    freeList(n->next);
    free(n);
}
void  freeShortestPaths(ShortestPaths paths) {
    if (paths.dist != NULL)
        free(paths.dist);
    for (int i = 0; i < paths.noNodes; i++){
        freeList(paths.pred[i]);
    }
}
