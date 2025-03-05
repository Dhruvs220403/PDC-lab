#include <stdio.h>
#include <limits.h>
#include <omp.h>

void floydWarshall(int V, int dist[V][V]) {
    int i, j, k;
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (k = 0; k < V; k++) {
                for (i = 0; i < V; i++) {
                    for (j = 0; j < V; j++) {
                        #pragma omp task firstprivate(i, j, k) shared(dist)
                        {
                            if (dist[i][k] + dist[k][j] < dist[i][j]) {
                                dist[i][j] = dist[i][k] + dist[k][j];
                            }
                        }
                    }
                }
                #pragma omp taskwait
            }
        }
    }
}

int main() {
    int V = 4;

    int dist[4][4] = {
        {0,   5,  1e9, 10},
        {1e9, 0,   3, 1e9},
        {1e9, 1e9, 0,   1},
        {1e9, 1e9, 1e9, 0}
    };

    floydWarshall(V, dist);

    printf("Shortest distances between every pair of vertices:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
// 	        if(dist[i][j] == 1e9) printf("INF ");
            printf("%3d ", dist[i][j]);
        }
        printf("\n");
    }

    return 0;
}
