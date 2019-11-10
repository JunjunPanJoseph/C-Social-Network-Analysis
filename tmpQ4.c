 #include <stdio.h>
 #define SIZE 100;
 int minDist(int **dist, int *unused,  int *i, int *j){
     *i = 1;
     *j = 2;
 }
 void updateDist(int **dist, int* unused, int i, int j){
     for (int k = 0; k < N; k++){
         if (!unused[k]){
            continue;
         }
         dist[i][k] = min(dist[i][k], dist[j][k]);
         dist[k][i] = min(dist[i][k], dist[j][k]);
     }
 }
 
 static Dendrogram makeNewNode(int n, Dendrogram left, Dendrogram right);
 int main(void){
    int **matrix2D;
    matrix2D = malloc(sizeof(int *) * SIZE);
    int *unused = malloc(sizeof(int) * SIZE);
    for (int i = 0; i < SIZE; i++){
        matrix2D[i] = malloc(sizeof(int)* SIZE);
        unused[i] = 1;
    }
    Dendrogram *treeNodeList = malloc(sizeof(Dendrogram) * nV);
    for (k = 1; k <= N-1; k++){
        int ci, cj;
        minDist(matrix2D, &ci, &cj);
        Dendrogram newNode = makeNewNode(0, treeNodeList[i], treeNodeList[j]);
        treeNodeList[i] = NULL;
        treeNodeList[j] = NULL;
        treeNodeList[i] = newNode;
        unused[j] = 0;
        updateDist(matrix2D,unused, i, j);
    }
    for (int k = 0; k < N; k++){
        if (unused[k])
            return treeNodeList[k];
        
    }
    
    return NULL;
     
     
 }
 