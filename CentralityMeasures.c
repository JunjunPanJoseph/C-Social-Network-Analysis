// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

NodeValues outDegreeCentrality(Graph g){
	NodeValues retval;
    retval.noNodes = numVerticies(g);
    retval.values = malloc(retval.noNodes * (sizeof(double)));
    AdjList tmpList = NULL;
    for (int i = 0; i < retval.noNodes; i++){
        retval.values[i] = 0;
        tmpList = outIncident(g, i);
        while (tmpList != NULL){
            retval.values[i] += 1.0;
            tmpList = tmpList->next;
        }
    }
	return retval;
}
NodeValues inDegreeCentrality(Graph g){
	NodeValues retval;
    retval.noNodes = numVerticies(g);
    retval.values = malloc(retval.noNodes * (sizeof(double)));
    AdjList tmpList = NULL;
    for (int i = 0; i < retval.noNodes; i++){
        retval.values[i] = 0;
        tmpList = inIncident(g, i);
        while (tmpList != NULL){
            retval.values[i] += 1.0;
            tmpList = tmpList->next;
        }
    }
	return retval;
}
NodeValues degreeCentrality(Graph g) {
	NodeValues in = inDegreeCentrality(g);
	NodeValues out = outDegreeCentrality(g);
    for (int i = 0; i < in.noNodes; i++){
        in.values[i] += out.values[i];
    }
    free(out.values);
	return in;
    
}

NodeValues closenessCentrality(Graph g){
	NodeValues retval;
    retval.noNodes = numVerticies(g);
    retval.values = malloc(sizeof(double) * retval.noNodes);
    ShortestPaths  paths;
    for (int i = 0; i < retval.noNodes; i++){
        retval.values[i] = 0;
        paths = dijkstra(g, i);
        int n = 0;
        for (int j = 0; j < retval.noNodes; j++){
            retval.values[i] += (double)paths.dist[j];
            if (paths.dist[j] != 0){
                n++;
            }
        }
        /*
        printf("i = %d  n = %d  nodes = %d\n", i, n, retval.noNodes);
        for (int k = 0; k < retval.noNodes; k++){
            printf("paths[%d] = %d\n", k, paths.dist[k]);
        }
        printf("\n");
        */
//        retval.values[i] =  (double)(n - 1) * (double)(n-1) / (double)(retval.noNodes - 1) / retval.values[i];
        if (retval.values[i] != 0){
            retval.values[i] =  1 / retval.values[i];
            retval.values[i] = (n) * (n)* retval.values[i];
            retval.values[i] = retval.values[i] / (retval.noNodes - 1);
        }
        freeShortestPaths(paths);
    }
	return retval;
}
struct _node{
    int n;
    struct _node *next;    
};
typedef struct _node *Node;
Node makeNode (int n, Node next){
    Node tmp = malloc(sizeof(struct _node));
    tmp->n = n;
    tmp->next = next;
    return tmp;
}
static void recCalc (ShortestPaths paths, Node stack, int *pathSVT){
    if (stack->n == paths.src){
        
        //add 1 to SVT
        Node tNode = stack->next;
        while (tNode != NULL && tNode->next != NULL){
            pathSVT[tNode->n]++;
            tNode = tNode->next;
        }
        free(stack);
        return;
    } else {
        PredNode *tmpPN = paths.pred[stack->n];
        while (tmpPN != NULL){
            recCalc (paths,  makeNode(tmpPN->v, stack), pathSVT);
            tmpPN = tmpPN->next;
        }
        free(stack);
    }
}
static int nSTPath(ShortestPaths paths, int t){
    if (t == paths.src){
        return 1;
    } else {
        PredNode *tmpPN = paths.pred[t];
        int nTotal = 0;
        while (tmpPN != NULL){
            nTotal += nSTPath (paths,  tmpPN->v);
            tmpPN = tmpPN->next;
        }
        return nTotal;
    }
}
static int calcPathSVT(ShortestPaths  paths, int *pathSVT, int t){
    for (int i = 0; i < paths.noNodes; i++){
        pathSVT[i] = 0;
    }
    recCalc(paths, makeNode(t, NULL), pathSVT);
    int pathST = nSTPath(paths, t);
    return pathST;
}
NodeValues betweennessCentrality(Graph g){

	NodeValues retval;
    retval.noNodes = numVerticies(g);
    retval.values = malloc(sizeof(double) * retval.noNodes);
    ShortestPaths  paths;
    int *pathSVT = malloc(retval.noNodes * (sizeof(int)));
    int pathST;
    for (int i = 0; i < retval.noNodes; i++){
        retval.values[i] = 0;
    }
    
    for (int s = 0; s < retval.noNodes; s++){
        paths = dijkstra(g, s);
        for (int t = 0; t < retval.noNodes; t++){
            pathST = calcPathSVT(paths, pathSVT, t);
            for (int i = 0; i < retval.noNodes; i++){
                if (pathST != 0){
                    retval.values[i] += (double)pathSVT[i] / (double)pathST;
                }
            }
        }
        freeShortestPaths(paths);
    }
    free(pathSVT);
	return retval;
}

NodeValues betweennessCentralityNormalised(Graph g){
	NodeValues b = betweennessCentrality(g);
    if (b.noNodes > 2){
        for (int i = 0; i < b.noNodes; i++){
            b.values[i] /= (double)(b.noNodes - 1) * (double)(b.noNodes - 2);
        }
    }
	return b;
}

void showNodeValues(NodeValues values){
    for (int i = 0; i < values.noNodes; i++){
        printf("%d: %.6lf\n", i, values.values[i]);
    }
}

void freeNodeValues(NodeValues values){
    free(values.values);
}
