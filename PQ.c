// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


struct PQRep {
    int end;
    int max;
    int t;
    ItemPQ *list;
    int *time;
};

void realloPQ(PQ p) {
    p->list = realloc(p->list, sizeof (ItemPQ) * (p->max * 2 + 1));
    p->time = realloc(p->time, sizeof (int) *(p->max * 2 + 1));
    p->max = p->max * 2;
}
PQ newPQ() {
    PQ tmp = malloc(sizeof (struct PQRep));
    tmp->end = 1;
    tmp->max = 50;
    tmp->t = 0;
    tmp->list = malloc(sizeof (ItemPQ) * 51);
    tmp->time = malloc(sizeof (int) * 51);
    
	return tmp;
}

int PQEmpty(PQ p) {
		return p->end == 1;
}
static void swapNode(PQ pq, int i, int j){
    ItemPQ tmp = pq->list[i];
    int tmpT = pq->time[i];
    pq->list[i] = pq->list[j];
    pq->time[i] = pq->time[j];
    pq->list[j] = tmp;
    pq->time[j] = tmpT;
}
static void mergeUp(PQ pq, int i){
    int n = i;
    while (n > 1 && pq->list[n].value <= pq->list[n / 2].value){
        if (pq->list[n].value == pq->list[n / 2].value){
            if (pq->time[n] > pq->time[n/2]){
                break;
            }
        }
        swapNode(pq, n, n / 2);
        n /= 2;
    }
}
static int checkIndexs(PQ pq, int i){
    int n = i;
    int l = n * 2;
    int r = n * 2 + 1;
    int retval = -1;
    if (r < pq->end){
        if (pq->list[l].value == pq->list[r].value){
            if (pq->list[n].value >= pq->list[l].value){
                if (pq->time[l] > pq->time[r]){
                    swapNode(pq, l, r);
                }
                if (pq->list[n].value == pq->list[l].value){
                    if (pq->time[i] < pq->time[l]){
                        retval = -1;
                    } else {
                        retval = l;
                    }
                } else {
                    retval = l;
                }
                
            }
        } else if (pq->list[l].value < pq->list[r].value){
            if (pq->list[n].value >= pq->list[l].value){
                if (pq->list[n].value == pq->list[l].value && 
                    pq->time[n] < pq->time[l]){
                    retval = -1;
                } else {
                    retval = l;
                }
            }
        } else {
            if (pq->list[n].value >= pq->list[r].value){
                if (pq->list[n].value == pq->list[r].value && 
                    pq->time[n] < pq->time[r]){
                    retval = -1;
                } else {
                    retval = r;
                }
            }
        }
    } else if (l < pq->end){
        if (pq->list[n].value >= pq->list[l].value){
            if (pq->list[n].value == pq->list[l].value && 
                pq->time[n] < pq->time[l]){
                retval = -1;
            } else {
                retval = l;
            }
        }
    }
    return retval;
}
static void mergeDown(PQ pq, int i){
    int n = i;
    int resIndex = -1;
    while (n < pq->end){
        resIndex = checkIndexs(pq, n);
        if (resIndex == -1){
            break;
        }
        swapNode(pq, n, resIndex);
        n = resIndex;
    }
}
void addPQ(PQ pq, ItemPQ element) {
    if (pq->end == pq->max){
        realloPQ(pq);
    }
    pq->time[pq->end] = pq->t ++;
    pq->list[pq->end++] = element;
    mergeUp(pq, pq->end - 1);
}

ItemPQ dequeuePQ(PQ pq) {
    swapNode(pq, 1, pq->end - 1);
	ItemPQ retval =  pq->list[pq->end - 1];
    pq->end--;
    mergeDown(pq, 1);
	return retval;
}

void updatePQ(PQ pq, ItemPQ element) {
    int i = 1;
    for (; i < pq->end; i++){
        if (pq->list[i].key == element.key){
            pq->list[i].value = element.value;
            pq->time[i] = pq->t++;
            mergeUp(pq, i);
            mergeDown(pq, i);
            break;
        }
    }
}

void  showPQ(PQ pq) {

}

void  freePQ(PQ pq) {
    if (pq == NULL) return;
    if (pq->list != NULL)
        free(pq->list);
    free(pq);
}
