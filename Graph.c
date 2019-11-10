// Graph ADT interface for Ass2 (COMP2521)
#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"

struct GraphRep{
    int n;
    AdjList *edges;
    AdjList *edgesIn;
};



Graph newGraph(int noNodes) {
	Graph tmpGraph = malloc(sizeof(struct GraphRep));
    tmpGraph->n = noNodes;
    tmpGraph->edges = malloc(sizeof(adjListNode) * (noNodes));
    tmpGraph->edgesIn = malloc(sizeof(adjListNode) * (noNodes));
    for (int i = 0; i <= noNodes; i++){
        tmpGraph->edges[i] = NULL;
        tmpGraph->edgesIn[i] = NULL;
    }
    return tmpGraph;
}

int numVerticies(Graph g) {
	return g->n;
}
AdjList createNode(Vertex dest, int weight){
    AdjList tmp = malloc(sizeof(adjListNode));
    tmp->w = dest;
    tmp->weight = weight;
    tmp->next = NULL;
    return tmp;
}
static AdjList insertNode(AdjList old, AdjList new){
    /*
    if (old == NULL){
        return new;
    }
    old->next = insertNode(old->next, new);
    return old;
    */
    new->next = old;
    return new;
    
}
void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    AdjList newEdge = createNode(dest, weight);
    g->edges[src] = insertNode(g->edges[src], newEdge);
    AdjList newEdgeIn = createNode(src, weight);
    g->edgesIn[dest] = insertNode(g->edgesIn[dest], newEdgeIn);
    
    
}

void  removeEdge(Graph g, Vertex src, Vertex dest) {
    AdjList pre = NULL;
    AdjList tmp = g->edges[src];
    while (tmp != NULL && tmp->w != dest){
        pre = tmp;
        tmp = tmp->next;
    }
    if (tmp != NULL){
        if (pre == NULL){
            g->edges[src] = tmp->next;
        } else {
            pre->next = tmp->next;
        }
        free(tmp);
    }
    pre = NULL;
    tmp = g->edgesIn[dest];
    while (tmp != NULL && tmp->w != src){
        pre = tmp;
        tmp = tmp->next;
    }
    if (tmp != NULL){
        if (pre == NULL){
            g->edgesIn[dest] = tmp->next;
        } else {
            pre->next = tmp->next;
        }
        free(tmp);
    }
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
    AdjList tmp = g->edges[src];
    while (tmp != NULL && tmp->w != dest){ 
        tmp = tmp->next;
    }
    if (tmp == NULL){
        return false;
    } else {
        return true;
    }
}

AdjList outIncident(Graph g, Vertex v) {
    return g->edges[v];
}

AdjList inIncident(Graph g, Vertex v) {
	return g->edgesIn[v];
}

void  showGraph(Graph g) {

}

void  freeGraph(Graph g) {
    if (g == NULL) return;
    if (g->edges != NULL){
        free(g->edges);
    }
    if (g->edgesIn != NULL){
        free(g->edgesIn);
    }
    free(g);
}
