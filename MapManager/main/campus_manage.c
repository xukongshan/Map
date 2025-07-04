#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

// 定义最大地点数和最大名称长度
#define MAX_VERTICES 100
#define MAX_NAME_LEN 50
#define MAX_ROAD_NAME_LEN 50

// 边信息结构体
typedef struct {
    int distance;       // 道路距离
    char roadName[MAX_ROAD_NAME_LEN]; // 道路名称
    int direction;      // 方向角度
} EdgeInfo;

// 图结构定义
typedef struct {
    char name[MAX_NAME_LEN]; // 地点名称
    int id;                 // 地点ID
} Vertex;

typedef struct {
    int numVertices;        // 顶点数量
    int numEdges;           // 边数量
    EdgeInfo edges[MAX_VERTICES][MAX_VERTICES]; // 边的详细信息
    Vertex vertices[MAX_VERTICES]; // 顶点数组
} Graph;

// 函数声明
void initGraph(Graph* graph);
int addVertex(Graph* graph, const char* name);
int modifyVertex(Graph* graph, const char* oldName, const char* newName);
int deleteVertex(Graph* graph, const char* name);
int findVertexByName(const Graph* graph, const char* name);
void printVertices(const Graph* graph);
int addEdge(Graph* graph, const char* start, const char* end, int distance, const char* roadName, int direction);
int modifyEdge(Graph* graph, const char* start, const char* end, int newDistance, const char* newRoadName, int newDirection);
int deleteEdge(Graph* graph, const char* start, const char* end);
void saveMapToFile(const Graph* graph, const char* filename);
int loadMapFromFile(Graph* graph, const char* filename);
void dijkstra(const Graph* graph, int start, int distances[], int predecessors[]);
void printPath(const Graph* graph, int start, int end, const int predecessors[]);
void printAllDistancesFromLocation(const Graph* graph, const char* location);
void findShortestPath(const Graph* graph, const char* start, const char* end);
void displayMenu();

// 主函数
int main() {
    Graph campusMap;
    initGraph(&campusMap);
    
    int choice;
    char filename[100];
    char name[MAX_NAME_LEN], newName[MAX_NAME_LEN];
    char start[MAX_NAME_LEN], end[MAX_NAME_LEN], roadName[MAX_ROAD_NAME_LEN];
    int distance, direction;
    
    printf("欢迎使用校园交通查询导引系统！\n");
    
    while (1) {
        displayMenu();
        printf("请选择操作: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("请输入地图文件名: ");
                scanf("%s", filename);
                if (loadMapFromFile(&campusMap, filename)) {
                    printf("地图加载成功！共加载 %d 个地点和 %d 条道路。\n", 
                           campusMap.numVertices, campusMap.numEdges);
                } else {
                    printf("地图加载失败！\n");
                }
                break;
                
            case 2:
                printf("请输入保存的文件名: ");
                scanf("%s", filename);
                saveMapToFile(&campusMap, filename);
                printf("地图保存成功！\n");
                break;
                
            case 3:
                printf("请输入新地点名称: ");
                scanf("%s", name);
                if (addVertex(&campusMap, name) != -1) {
                    printf("地点添加成功！\n");
                } else {
                    printf("地点添加失败（可能已达最大数量或名称重复）！\n");
                }
                break;
                
            case 4:
                printf("请输入需要修改的地点名：");
                scanf("%s", name);
                printf("请输入修改的地名：");
                scanf("%s", newName);
                if (modifyVertex(&campusMap, name, newName)) {
                    printf("修改地名成功！\n");
                } else {
                    printf("修改地名失败！\n");
                }
                break;

            case 5:
                printf("请输入要删除的地点名称: ");
                scanf("%s", name);
                if (deleteVertex(&campusMap, name)) {
                    printf("地点删除成功！\n");
                } else {
                    printf("地点删除失败（地点不存在）！\n");
                }
                break;

            case 6:
                printf("请输入起点名称: ");
                scanf("%s", start);
                printf("请输入终点名称: ");
                scanf("%s", end);
                printf("请输入道路距离: ");
                scanf("%d", &distance);
                printf("请输入道路名称: ");
                scanf("%s", roadName);
                printf("请输入方向角度(0-360): ");
                scanf("%d", &direction);
                if (addEdge(&campusMap, start, end, distance, roadName, direction)) {
                    printf("道路添加成功！\n");
                } else {
                    printf("道路添加失败（地点不存在或距离无效）！\n");
                }
                break;
                
            case 7:
                printf("请输入起点名称: ");
                scanf("%s", start);
                printf("请输入终点名称: ");
                scanf("%s", end);
                printf("请输入新的道路距离: ");
                scanf("%d", &distance);
                printf("请输入新的道路名称: ");
                scanf("%s", roadName);
                printf("请输入新的方向角度(0-360): ");
                scanf("%d", &direction);
                if (modifyEdge(&campusMap, start, end, distance, roadName, direction)) {
                    printf("道路修改成功！\n");
                } else {
                    printf("道路修改失败（地点不存在或道路不存在）！\n");
                }
                break;

            case 8:
                printf("请输入起点名称：");
                scanf("%s", start);
                printf("请输入终点名称：");
                scanf("%s", end);
                if (deleteEdge(&campusMap, start, end)) {
                    printf("删除道路成功！\n");
                } else {
                    printf("删除道路失败！\n");
                }
                break;
                
            case 9:
                printf("\n校园所有地点列表:\n");
                printVertices(&campusMap);
                break;
                
            case 10:
                printf("请输入您当前所在位置: ");
                scanf("%s", name);
                printAllDistancesFromLocation(&campusMap, name);
                break;
                
            case 11:
                printf("请输入起点名称: ");
                scanf("%s", start);
                printf("请输入终点名称: ");
                scanf("%s", end);
                findShortestPath(&campusMap, start, end);
                break;
                
            case 12:
                printf("感谢使用校园交通查询导引系统，再见！\n");
                exit(0);
                break;
                
            default:
                printf("无效选择，请重新输入！\n");
        }
        printf("\n");
    }
    
    return 0;
}

// 初始化图
void initGraph(Graph* graph) {
    graph->numVertices = 0;
    graph->numEdges = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            // 初始化边信息
            graph->edges[i][j].distance = 0;
            strcpy(graph->edges[i][j].roadName, "");
            graph->edges[i][j].direction = -1;
        }
    }
}

// 添加地点
int addVertex(Graph* graph, const char* name) {
    if (graph->numVertices >= MAX_VERTICES) {
        return -1; // 达到最大地点数
    }
    
    // 检查地点是否已存在
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return -1; // 地点已存在
        }
    }
    
    // 添加新地点
    strcpy(graph->vertices[graph->numVertices].name, name);
    graph->vertices[graph->numVertices].id = graph->numVertices;
    graph->numVertices++;
    return graph->numVertices - 1; // 返回新地点的ID
}

// 修改地点
int modifyVertex(Graph* graph, const char* oldName, const char* newName) {
    int id = findVertexByName(graph, oldName);

    if (id == -1) {
        return 0;  // 地点不存在
    }

    for (int i = 0; i < graph->numVertices; i++) {  // 检查是否地点重名，同一地点除外
        if (i != id && strcmp(graph->vertices[i].name, newName) == 0) {
            return 0;  // 名称重复
        }
    }

    strcpy(graph->vertices[id].name, newName);
    return 1;
}

// 删除地点
int deleteVertex(Graph* graph, const char* name) {
    int vertexId = findVertexByName(graph, name);
    if (vertexId == -1) {
        return 0; // 地点不存在
    }
    
    // 删除相关的道路
    int deletedEdges = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->edges[vertexId][i].distance > 0) {
            // 删除双向道路
            graph->edges[vertexId][i].distance = 0;
            graph->edges[i][vertexId].distance = 0;
            deletedEdges++;
        }
    }
    
    // 更新边数（每条边在邻接矩阵中占两个位置）
    graph->numEdges -= deletedEdges / 2;
    
    // 将该地点从顶点数组中移除，将最后一个顶点移动到被删除的位置
    if (vertexId != graph->numVertices - 1) {
        // 复制顶点信息
        strcpy(graph->vertices[vertexId].name, graph->vertices[graph->numVertices - 1].name);
        graph->vertices[vertexId].id = vertexId;
        
        // 移动道路信息
        for (int i = 0; i < graph->numVertices; i++) {
            // 复制行
            graph->edges[vertexId][i] = graph->edges[graph->numVertices - 1][i];
            // 复制列
            graph->edges[i][vertexId] = graph->edges[i][graph->numVertices - 1];
        }
        
        // 清除最后一个顶点的道路信息
        for (int i = 0; i < graph->numVertices; i++) {
            graph->edges[graph->numVertices - 1][i].distance = 0;
            graph->edges[i][graph->numVertices - 1].distance = 0;
            strcpy(graph->edges[graph->numVertices - 1][i].roadName, "");
            strcpy(graph->edges[i][graph->numVertices - 1].roadName, "");
            graph->edges[graph->numVertices - 1][i].direction = -1;
            graph->edges[i][graph->numVertices - 1].direction = -1;
        }
    } else {
        // 如果删除的是最后一个顶点，只需清除其道路信息
        for (int i = 0; i < graph->numVertices; i++) {
            graph->edges[vertexId][i].distance = 0;
            graph->edges[i][vertexId].distance = 0;
            strcpy(graph->edges[vertexId][i].roadName, "");
            strcpy(graph->edges[i][vertexId].roadName, "");
            graph->edges[vertexId][i].direction = -1;
            graph->edges[i][vertexId].direction = -1;
        }
    }
    
    graph->numVertices--;
    
    return 1;
}

// 根据名称查找地点ID
int findVertexByName(const Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return i; // 返回地点ID
        }
    }
    return -1; // 未找到
}

// 打印所有地点
void printVertices(const Graph* graph) {
    if (graph->numVertices == 0) {
        printf("暂无地点信息\n");
        return;
    }
    
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d. %s\n", i+1, graph->vertices[i].name);
    }
}

// 添加道路
int addEdge(Graph* graph, const char* start, const char* end, 
            int distance, const char* roadName, int direction) {
    int startId = findVertexByName(graph, start);
    int endId = findVertexByName(graph, end);
    
    if (startId == -1 || endId == -1 || distance <= 0) {
        return 0; // 无效输入
    }
    
    // 添加双向道路（无向图）
    if (graph->edges[startId][endId].distance == 0) {
        graph->numEdges++;
    }
    
    graph->edges[startId][endId].distance = distance;
    strcpy(graph->edges[startId][endId].roadName, roadName);
    graph->edges[startId][endId].direction = direction;
    
    graph->edges[endId][startId].distance = distance;
    strcpy(graph->edges[endId][startId].roadName, roadName);
    graph->edges[endId][startId].direction = (direction + 180) % 360; // 反向道路方向
    
    return 1;
}

// 修改道路信息
int modifyEdge(Graph* graph, const char* start, const char* end, 
               int newDistance, const char* newRoadName, int newDirection) {
    int startId = findVertexByName(graph, start);
    int endId = findVertexByName(graph, end);
    
    if (startId == -1 || endId == -1 || newDistance <= 0) {
        return 0; // 无效输入
    }
    
    // 检查道路是否存在
    if (graph->edges[startId][endId].distance == 0) {
        return 0; // 道路不存在
    }
    
    // 修改双向道路信息
    graph->edges[startId][endId].distance = newDistance;
    strcpy(graph->edges[startId][endId].roadName, newRoadName);
    graph->edges[startId][endId].direction = newDirection;
    
    graph->edges[endId][startId].distance = newDistance;
    strcpy(graph->edges[endId][startId].roadName, newRoadName);
    graph->edges[endId][startId].direction = (newDirection + 180) % 360; // 反向道路方向
    
    return 1;
}

// 删除道路
int deleteEdge(Graph* graph, const char* start, const char* end) {
    int startId = findVertexByName(graph, start);
    int endId = findVertexByName(graph, end);

    if (startId == -1 || endId == -1) {
        return 0;
    }

    if (graph->edges[startId][endId].distance == 0) {
        return 0;
    }

    graph->edges[startId][endId].distance = 0;
    strcpy(graph->edges[startId][endId].roadName, "");
    graph->edges[startId][endId].direction = -1;
    
    graph->edges[endId][startId].distance = 0;
    strcpy(graph->edges[endId][startId].roadName, "");
    graph->edges[endId][startId].direction = -1;

    graph->numEdges--;

    return 1;
}

// 保存地图到文件
void saveMapToFile(const Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件: %s\n", filename);
        return;
    }
    
    // 写入地点数量
    fprintf(file, "%d\n", graph->numVertices);
    
    // 写入所有地点名称
    for (int i = 0; i < graph->numVertices; i++) {
        fprintf(file, "%s\n", graph->vertices[i].name);
    }
    
    // 计算实际边数（只保存一个方向的边）
    int edgeCount = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = i + 1; j < graph->numVertices; j++) {
            if (graph->edges[i][j].distance > 0) {
                edgeCount++;
            }
        }
    }
    
    // 写入边数量
    fprintf(file, "%d\n", edgeCount);
    
    // 写入边信息（只保存i<j的边）
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = i + 1; j < graph->numVertices; j++) {
            if (graph->edges[i][j].distance > 0) {
                fprintf(file, "%s %s %d %s %d\n",
                        graph->vertices[i].name,
                        graph->vertices[j].name,
                        graph->edges[i][j].distance,
                        graph->edges[i][j].roadName,
                        graph->edges[i][j].direction);
            }
        }
    }
    
    fclose(file);
}

// 从文件加载地图
int loadMapFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    // 初始化图
    initGraph(graph);
    
    // 读取地点数量
    fscanf(file, "%d", &graph->numVertices);
    fgetc(file); // 跳过换行符
    
    // 读取所有地点名称
    for (int i = 0; i < graph->numVertices; i++) {
        fgets(graph->vertices[i].name, MAX_NAME_LEN, file);
        // 去除换行符
        graph->vertices[i].name[strcspn(graph->vertices[i].name, "\n")] = '\0';
        graph->vertices[i].id = i;
    }
    
    // 读取边数量
    int edgeCount;
    fscanf(file, "%d", &edgeCount);
    fgetc(file); // 跳过换行符
    
    // 读取每条边的信息
    char startName[MAX_NAME_LEN], endName[MAX_NAME_LEN], roadName[MAX_ROAD_NAME_LEN];
    int distance, direction;
    
    for (int i = 0; i < edgeCount; i++) {
        fscanf(file, "%s %s %d %s %d", 
               startName, endName, &distance, roadName, &direction);
        
        // 添加边（会自动添加反向边）
        addEdge(graph, startName, endName, distance, roadName, direction);
    }
    
    fclose(file);
    return 1;
}

// Dijkstra算法实现
void dijkstra(const Graph* graph, int start, int distances[], int predecessors[]) {
    int visited[MAX_VERTICES] = {0};
    
    // 初始化距离和前驱
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = -1;
    }
    distances[start] = 0;
    
    for (int count = 0; count < graph->numVertices - 1; count++) {
        // 找到未访问顶点中距离最小的
        int minDist = INT_MAX;
        int minIndex = -1;
        
        for (int v = 0; v < graph->numVertices; v++) {
            if (!visited[v] && distances[v] < minDist) {
                minDist = distances[v];
                minIndex = v;
            }
        }
        
        if (minIndex == -1) break; // 无有效顶点退出
        visited[minIndex] = 1; // 标记该顶点以访问
        
        // 更新相邻顶点的距离
        for (int v = 0; v < graph->numVertices; v++) {
            if (!visited[v] && 
                graph->edges[minIndex][v].distance > 0 && 
                distances[minIndex] != INT_MAX &&  // 防溢出
                distances[minIndex] + graph->edges[minIndex][v].distance < distances[v]) {
                
                distances[v] = distances[minIndex] + graph->edges[minIndex][v].distance;
                predecessors[v] = minIndex;
            }
        }
    }
}

// 打印路径
void printPath(const Graph* graph, int start, int end, const int predecessors[]) {
    if (end == start) {
        printf("%s", graph->vertices[start].name);
    } else if (predecessors[end] == -1) {
        printf("没有路径");
    } else {
        printPath(graph, start, predecessors[end], predecessors);
        printf(" -> %s", graph->vertices[end].name);
    }
}

// 打印从当前位置到所有地点的距离
void printAllDistancesFromLocation(const Graph* graph, const char* location) {
    int start = findVertexByName(graph, location);
    if (start == -1) {
        printf("未找到该地点: %s\n", location);
        return;
    }
    
    int distances[MAX_VERTICES];
    int predecessors[MAX_VERTICES];
    
    dijkstra(graph, start, distances, predecessors);
    
    printf("\n从 %s 到各地点距离:\n", location);
    printf("%-20s %-10s %s\n", "地点", "距离(米)", "路径");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < graph->numVertices; i++) {
        if (i != start) {
            printf("%-20s %-10d ", graph->vertices[i].name, distances[i]);
            printPath(graph, start, i, predecessors);
            printf("\n");
        }
    }
}

// 查找两点间最短路径
void findShortestPath(const Graph* graph, const char* startName, const char* endName) {
    int start = findVertexByName(graph, startName);
    int end = findVertexByName(graph, endName);
    
    if (start == -1) {
        printf("未找到起点: %s\n", startName);
        return;
    }
    if (end == -1) {
        printf("未找到终点: %s\n", endName);
        return;
    }
    
    int distances[MAX_VERTICES];
    int predecessors[MAX_VERTICES];
    
    dijkstra(graph, start, distances, predecessors);
    
    if (distances[end] == INT_MAX) {
        printf("从 %s 到 %s 没有路径\n", startName, endName);
        return;
    }
    
    printf("\n最短路径信息:\n");
    printf("起点: %s\n终点: %s\n", startName, endName);
    printf("总距离: %d 米\n", distances[end]);
    printf("路径: ");
    printPath(graph, start, end, predecessors);
    
    // 打印详细路径信息
    printf("\n\n详细路径导航:\n");
    int current = end;
    int path[MAX_VERTICES];
    int pathLength = 0;
    
    // 存储路径
    while (current != -1) {
        path[pathLength++] = current;
        current = predecessors[current];
    }
    
    // 反向打印路径
    for (int i = pathLength - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i-1];
        printf("%d. 从 %s 到 %s\n", pathLength - i, graph->vertices[from].name, graph->vertices[to].name);
        printf("   道路: %s, 距离: %d米, 方向: %d度\n", 
               graph->edges[from][to].roadName,
               graph->edges[from][to].distance,
               graph->edges[from][to].direction);
    }
    printf("\n");
}

// 显示菜单
void displayMenu() {
    printf("\n===== 校园交通查询导引系统 =====\n");
    printf("1. 加载地图文件\n");
    printf("2. 保存地图文件\n");
    printf("3. 添加地点\n");
    printf("4. 修改地点名称\n");
    printf("5. 删除地点\n");  
    printf("6. 添加道路\n");
    printf("7. 修改道路\n");
    printf("8. 删除道路\n");
    printf("9. 显示所有地点\n");
    printf("10. 查询当前位置到各地点距离\n");
    printf("11. 查询两点间最短路径\n");
    printf("12. 退出系统\n");
    printf("===============================\n");
}
