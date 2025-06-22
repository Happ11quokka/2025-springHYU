#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 10
#define INF INT_MAX

typedef struct {
    int n;  // 정점의 개수
    int weight[MAX_VERTICES][MAX_VERTICES];
} GraphType;

// 그래프 초기화
void init_graph(GraphType *g) {
    int i, j;
    g->n = 0;
    for (i = 0; i < MAX_VERTICES; i++) {
        for (j = 0; j < MAX_VERTICES; j++) {
            g->weight[i][j] = (i == j) ? 0 : INF;
        }
    }
}

// 정점 삽입
void insert_vertex(GraphType *g, int v) {
    if (g->n + 1 > MAX_VERTICES) {
        printf("그래프: 정점의 개수 초과\n");
        return;
    }
    g->n++;
}

// 간선 삽입
void insert_edge(GraphType *g, int start, int end, int weight) {
    if (start >= g->n || end >= g->n) {
        printf("그래프: 정점 번호 오류\n");
        return;
    }
    g->weight[start][end] = weight;
    g->weight[end][start] = weight;  // 무방향 그래프
}

// 최소 거리를 가진 정점 찾기
int choose_vertex(int distance[], int found[], int n) {
    int i, min, minpos;
    min = INF;
    minpos = -1;
    
    for (i = 0; i < n; i++) {
        if (distance[i] < min && !found[i]) {
            min = distance[i];
            minpos = i;
        }
    }
    return minpos;
}

// 경로 출력을 위한 재귀 함수
void print_path(int predecessor[], int start, int end) {
    if (start == end) {
        printf("%d", start);
    } else if (predecessor[end] == -1) {
        printf("경로가 존재하지 않습니다.");
    } else {
        print_path(predecessor, start, predecessor[end]);
        printf(" -> %d", end);
    }
}

// 다익스트라 알고리즘
void dijkstra(GraphType *g, int start) {
    int distance[MAX_VERTICES];    // 시작 정점으로부터의 최단 거리
    int found[MAX_VERTICES];       // 방문한 정점 집합
    int predecessor[MAX_VERTICES]; // 최단 경로에서의 이전 정점
    int i, u, w;
    
    // 초기화
    for (i = 0; i < g->n; i++) {
        distance[i] = g->weight[start][i];
        found[i] = 0;
        if (distance[i] != INF && i != start) {
            predecessor[i] = start;
        } else {
            predecessor[i] = -1;
        }
    }
    
    found[start] = 1;    // 시작 정점 방문 표시
    distance[start] = 0; // 시작 정점까지의 거리는 0
    
    printf("\n=== 다익스트라 알고리즘 수행 과정 ===\n");
    printf("단계\t선택된 정점\t거리 배열\n");
    
    // 단계별 수행
    for (i = 0; i < g->n - 1; i++) {
        u = choose_vertex(distance, found, g->n);
        if (u == -1) break;  // 더 이상 갈 수 있는 정점이 없음
        
        found[u] = 1;
        printf("%d\t정점 %d\t\t", i + 1, u);
        
        // 거리 배열 출력
        for (int j = 0; j < g->n; j++) {
            if (distance[j] == INF) {
                printf("∞ ");
            } else {
                printf("%d ", distance[j]);
            }
        }
        printf("\n");
        
        // 선택된 정점을 통해 인접 정점들의 거리 갱신
        for (w = 0; w < g->n; w++) {
            if (!found[w] && g->weight[u][w] != INF) {
                if (distance[u] + g->weight[u][w] < distance[w]) {
                    distance[w] = distance[u] + g->weight[u][w];
                    predecessor[w] = u;
                }
            }
        }
    }
    
    // 결과 출력
    printf("\n=== 최단 경로 결과 (시작 정점: %d) ===\n", start);
    printf("정점\t거리\t경로\n");
    printf("-------------------------\n");
    
    for (i = 0; i < g->n; i++) {
        printf("%d\t", i);
        if (distance[i] == INF) {
            printf("∞\t연결되지 않음\n");
        } else {
            printf("%d\t", distance[i]);
            print_path(predecessor, start, i);
            printf("\n");
        }
    }
}

// 그래프 출력
void print_graph(GraphType *g) {
    int i, j;
    printf("\n=== 인접 행렬 ===\n");
    printf("   ");
    for (i = 0; i < g->n; i++) {
        printf("%3d", i);
    }
    printf("\n");
    
    for (i = 0; i < g->n; i++) {
        printf("%2d ", i);
        for (j = 0; j < g->n; j++) {
            if (g->weight[i][j] == INF) {
                printf("  ∞");
            } else {
                printf("%3d", g->weight[i][j]);
            }
        }
        printf("\n");
    }
}

// 간선 정보 출력
void print_edges(GraphType *g) {
    int i, j;
    printf("\n=== 간선 정보 ===\n");
    for (i = 0; i < g->n; i++) {
        for (j = i + 1; j < g->n; j++) {
            if (g->weight[i][j] != INF && g->weight[i][j] != 0) {
                printf("정점 %d - 정점 %d: 가중치 %d\n", i, j, g->weight[i][j]);
            }
        }
    }
}

// 예제 그래프 생성
void create_sample_graph(GraphType *g) {
    // 7개 정점 생성
    for (int i = 0; i < 7; i++) {
        insert_vertex(g, i);
    }
    
    // 간선 추가 (무방향 가중치 그래프)
    insert_edge(g, 0, 1, 7);
    insert_edge(g, 0, 2, 9);
    insert_edge(g, 0, 5, 14);
    insert_edge(g, 1, 2, 10);
    insert_edge(g, 1, 3, 15);
    insert_edge(g, 2, 3, 11);
    insert_edge(g, 2, 5, 2);
    insert_edge(g, 3, 4, 6);
    insert_edge(g, 4, 5, 9);
    
    printf("예제 그래프가 생성되었습니다.\n");
}

int main() {
    GraphType g;
    int choice, start, end, weight, vertex;
    
    init_graph(&g);
    
    printf("=== 다익스트라 최단 경로 알고리즘 ===\n");
    
    while (1) {
        printf("\n메뉴 선택:\n");
        printf("1. 정점 추가\n");
        printf("2. 간선 추가\n");
        printf("3. 그래프 출력\n");
        printf("4. 간선 정보 출력\n");
        printf("5. 다익스트라 알고리즘 실행\n");
        printf("6. 예제 그래프 생성\n");
        printf("7. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("추가할 정점 번호: ");
                scanf("%d", &vertex);
                insert_vertex(&g, vertex);
                printf("정점 %d가 추가되었습니다. (총 %d개 정점)\n", vertex, g.n);
                break;
                
            case 2:
                printf("시작 정점: ");
                scanf("%d", &start);
                printf("끝 정점: ");
                scanf("%d", &end);
                printf("가중치: ");
                scanf("%d", &weight);
                insert_edge(&g, start, end, weight);
                printf("간선 (%d, %d, %d)가 추가되었습니다.\n", start, end, weight);
                break;
                
            case 3:
                print_graph(&g);
                break;
                
            case 4:
                print_edges(&g);
                break;
                
            case 5:
                if (g.n == 0) {
                    printf("그래프에 정점이 없습니다.\n");
                    break;
                }
                printf("시작 정점 (0-%d): ", g.n - 1);
                scanf("%d", &start);
                if (start < 0 || start >= g.n) {
                    printf("잘못된 정점 번호입니다.\n");
                } else {
                    dijkstra(&g, start);
                }
                break;
                
            case 6:
                init_graph(&g);
                create_sample_graph(&g);
                break;
                
            case 7:
                printf("프로그램을 종료합니다.\n");
                return 0;
                
            default:
                printf("잘못된 선택입니다.\n");
        }
    }
    
    return 0;
}