/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by 
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#define numVertices numVerticies

/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */
#define MIN(a, b) (a > b)? b:a
#define MAX(a, b) (a > b)? a:b
static Dendrogram makeNewNode(int n, Dendrogram left, Dendrogram right){
        Dendrogram tmp = malloc (sizeof(DNode));
        tmp->vertex = n;
        tmp->left = left;
        tmp->right = right;
        return tmp;
}
double findClosest (double **dist, int *removed, int size, int *small, int *large){
    double minDist = 1.7e+308;
    int closest_i = size;
    int closest_j = size - 1;
    for (int i = 0; i < size; i++){
        if (removed[i])
            continue;
        for (int j = 0; j < i; j++){
            if (removed[j])
                continue;
            if (dist[i][j] <= minDist){
                minDist = dist[i][j];
                closest_i = i;
                closest_j = j;
            }
        }
    }
    *small = MIN(closest_i, closest_j);
    *large = MAX(closest_i, closest_j);
    return minDist;
}
/*
static void showAll (double **dist, int *removed, int size){
    for (int i = -1; i < size; i++){
        for (int j = -1; j < size; j++){
            if (i == -1 && j == -1){
                printf("     ");
                for (int k = 0; k < size; k++){
                    printf("%4d  ", k);
                }
                printf("\n");
                printf("     ");
                for (int k = 0; k < size; k++){

                    if (removed[k] == 1){
                        printf("   x  ");
                    } else {
                        printf("      ");
                    }
                }
            } else if (j == -1){
                printf("%3d  ", i);
                if (removed[i] == 1){
                    printf("x  ");
                } else {
                    printf("   ");
                }
            } else {
                if (!removed[i] && !removed[j]){
                    if (dist[i][j] == 1.7e+307){
                        printf("INF  ");
                    } else {
                        printf("%3.1lf  ", dist[i][j]);
                    }
                } else {
                    printf("      ");
                }
            }
            
        }
        printf("\n");
    }
}
*/
Dendrogram LanceWilliamsHAC(Graph g, int method) {
    int nV = numVerticies(g);
    double **distArray = malloc (sizeof(double *) * nV);
    int *removed = malloc (sizeof(int) * nV);
    Dendrogram *treeNodeList = malloc(sizeof(Dendrogram) * nV);
    for (int i = 0; i < nV; i++){
        removed[i] = 0;
        distArray[i] = malloc (sizeof(double) * nV);
        treeNodeList[i] =makeNewNode(i, NULL, NULL);
        for (int j = 0; j < nV; j++){
            distArray[i][j] =  1.7e+307;
        }
    }
    AdjList tmpList;
    for (int i = 0; i < nV; i++){
        tmpList = outIncident(g, i);
        while(tmpList != NULL){ 
            distArray[i][tmpList->w] = 
                MIN((double)distArray[i][tmpList->w], 1/(double)tmpList->weight );
            distArray[tmpList->w][i] = 
                MIN((double)distArray[tmpList->w][i], 1/(double)tmpList->weight );
            
            tmpList = tmpList->next;
        }
    }
    int n = nV;
    int small;
    int large;
    Dendrogram retval = NULL;
    while (n > 1){
        findClosest (distArray, removed, nV, &small, &large);
        treeNodeList[small] = makeNewNode(small, treeNodeList[small], treeNodeList[large]);
        retval = treeNodeList[small];
        treeNodeList[large] = NULL;
        removed[large] = 1;
        for (int k = 0;k < nV; k++){
            if (removed[k]) 
                continue;
            switch (method){
                case 1:
                    //single
                    distArray[small][k] = MIN(distArray[small][k], distArray[large][k]);
                    distArray[k][small] = distArray[small][k];
                    break;
                case 2:
                    //complete
                    distArray[small][k] = MAX(distArray[small][k], distArray[large][k]);
                    distArray[k][small] = distArray[small][k];
                    break;
            } 
        }
        free(distArray[large]);
        distArray[large] = NULL;
        n --;
    }
    for (int i = 0; i < nV; i++){
        if (distArray[i] != NULL){
            free(distArray[i]);
        }
    }
    free(distArray);
    free(removed);
    free(treeNodeList);
    return retval;
}


void freeDendrogram(Dendrogram d) {
    if (d == NULL){
        return;
    }
     freeDendrogram(d->left);
     freeDendrogram(d->right);
     free(d);
}



