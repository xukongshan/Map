#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

// �������ص�����������Ƴ���
#define MAX_VERTICES 100
#define MAX_NAME_LEN 50
#define MAX_ROAD_NAME_LEN 50

// ����Ϣ�ṹ��
typedef struct {
    int distance;       // ��·����
    char roadName[MAX_ROAD_NAME_LEN]; // ��·����
    int direction;      // ����Ƕ�
} EdgeInfo;

// ͼ�ṹ����
typedef struct {
    char name[MAX_NAME_LEN]; // �ص�����
    int id;                 // �ص�ID
} Vertex;

typedef struct {
    int numVertices;        // ��������
    int numEdges;           // ������
    EdgeInfo edges[MAX_VERTICES][MAX_VERTICES]; // �ߵ���ϸ��Ϣ
    Vertex vertices[MAX_VERTICES]; // ��������
} Graph;

// ��������
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

// ������
int main() {
    Graph campusMap;
    initGraph(&campusMap);
    
    int choice;
    char filename[100];
    char name[MAX_NAME_LEN], newName[MAX_NAME_LEN];
    char start[MAX_NAME_LEN], end[MAX_NAME_LEN], roadName[MAX_ROAD_NAME_LEN];
    int distance, direction;
    
    printf("��ӭʹ��У԰��ͨ��ѯ����ϵͳ��\n");
    
    while (1) {
        displayMenu();
        printf("��ѡ�����: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("�������ͼ�ļ���: ");
                scanf("%s", filename);
                if (loadMapFromFile(&campusMap, filename)) {
                    printf("��ͼ���سɹ��������� %d ���ص�� %d ����·��\n", 
                           campusMap.numVertices, campusMap.numEdges);
                } else {
                    printf("��ͼ����ʧ�ܣ�\n");
                }
                break;
                
            case 2:
                printf("�����뱣����ļ���: ");
                scanf("%s", filename);
                saveMapToFile(&campusMap, filename);
                printf("��ͼ����ɹ���\n");
                break;
                
            case 3:
                printf("�������µص�����: ");
                scanf("%s", name);
                if (addVertex(&campusMap, name) != -1) {
                    printf("�ص���ӳɹ���\n");
                } else {
                    printf("�ص����ʧ�ܣ������Ѵ���������������ظ�����\n");
                }
                break;
                
            case 4:
                printf("��������Ҫ�޸ĵĵص�����");
                scanf("%s", name);
                printf("�������޸ĵĵ�����");
                scanf("%s", newName);
                if (modifyVertex(&campusMap, name, newName)) {
                    printf("�޸ĵ����ɹ���\n");
                } else {
                    printf("�޸ĵ���ʧ�ܣ�\n");
                }
                break;

            case 5:
                printf("������Ҫɾ���ĵص�����: ");
                scanf("%s", name);
                if (deleteVertex(&campusMap, name)) {
                    printf("�ص�ɾ���ɹ���\n");
                } else {
                    printf("�ص�ɾ��ʧ�ܣ��ص㲻���ڣ���\n");
                }
                break;

            case 6:
                printf("�������������: ");
                scanf("%s", start);
                printf("�������յ�����: ");
                scanf("%s", end);
                printf("�������·����: ");
                scanf("%d", &distance);
                printf("�������·����: ");
                scanf("%s", roadName);
                printf("�����뷽��Ƕ�(0-360): ");
                scanf("%d", &direction);
                if (addEdge(&campusMap, start, end, distance, roadName, direction)) {
                    printf("��·��ӳɹ���\n");
                } else {
                    printf("��·���ʧ�ܣ��ص㲻���ڻ������Ч����\n");
                }
                break;
                
            case 7:
                printf("�������������: ");
                scanf("%s", start);
                printf("�������յ�����: ");
                scanf("%s", end);
                printf("�������µĵ�·����: ");
                scanf("%d", &distance);
                printf("�������µĵ�·����: ");
                scanf("%s", roadName);
                printf("�������µķ���Ƕ�(0-360): ");
                scanf("%d", &direction);
                if (modifyEdge(&campusMap, start, end, distance, roadName, direction)) {
                    printf("��·�޸ĳɹ���\n");
                } else {
                    printf("��·�޸�ʧ�ܣ��ص㲻���ڻ��·�����ڣ���\n");
                }
                break;

            case 8:
                printf("������������ƣ�");
                scanf("%s", start);
                printf("�������յ����ƣ�");
                scanf("%s", end);
                if (deleteEdge(&campusMap, start, end)) {
                    printf("ɾ����·�ɹ���\n");
                } else {
                    printf("ɾ����·ʧ�ܣ�\n");
                }
                break;
                
            case 9:
                printf("\nУ԰���еص��б�:\n");
                printVertices(&campusMap);
                break;
                
            case 10:
                printf("����������ǰ����λ��: ");
                scanf("%s", name);
                printAllDistancesFromLocation(&campusMap, name);
                break;
                
            case 11:
                printf("�������������: ");
                scanf("%s", start);
                printf("�������յ�����: ");
                scanf("%s", end);
                findShortestPath(&campusMap, start, end);
                break;
                
            case 12:
                printf("��лʹ��У԰��ͨ��ѯ����ϵͳ���ټ���\n");
                exit(0);
                break;
                
            default:
                printf("��Чѡ�����������룡\n");
        }
        printf("\n");
    }
    
    return 0;
}

// ��ʼ��ͼ
void initGraph(Graph* graph) {
    graph->numVertices = 0;
    graph->numEdges = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            // ��ʼ������Ϣ
            graph->edges[i][j].distance = 0;
            strcpy(graph->edges[i][j].roadName, "");
            graph->edges[i][j].direction = -1;
        }
    }
}

// ��ӵص�
int addVertex(Graph* graph, const char* name) {
    if (graph->numVertices >= MAX_VERTICES) {
        return -1; // �ﵽ���ص���
    }
    
    // ���ص��Ƿ��Ѵ���
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return -1; // �ص��Ѵ���
        }
    }
    
    // ����µص�
    strcpy(graph->vertices[graph->numVertices].name, name);
    graph->vertices[graph->numVertices].id = graph->numVertices;
    graph->numVertices++;
    return graph->numVertices - 1; // �����µص��ID
}

// �޸ĵص�
int modifyVertex(Graph* graph, const char* oldName, const char* newName) {
    int id = findVertexByName(graph, oldName);

    if (id == -1) {
        return 0;  // �ص㲻����
    }

    for (int i = 0; i < graph->numVertices; i++) {  // ����Ƿ�ص�������ͬһ�ص����
        if (i != id && strcmp(graph->vertices[i].name, newName) == 0) {
            return 0;  // �����ظ�
        }
    }

    strcpy(graph->vertices[id].name, newName);
    return 1;
}

// ɾ���ص�
int deleteVertex(Graph* graph, const char* name) {
    int vertexId = findVertexByName(graph, name);
    if (vertexId == -1) {
        return 0; // �ص㲻����
    }
    
    // ɾ����صĵ�·
    int deletedEdges = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->edges[vertexId][i].distance > 0) {
            // ɾ��˫���·
            graph->edges[vertexId][i].distance = 0;
            graph->edges[i][vertexId].distance = 0;
            deletedEdges++;
        }
    }
    
    // ���±�����ÿ�������ڽӾ�����ռ����λ�ã�
    graph->numEdges -= deletedEdges / 2;
    
    // ���õص�Ӷ����������Ƴ��������һ�������ƶ�����ɾ����λ��
    if (vertexId != graph->numVertices - 1) {
        // ���ƶ�����Ϣ
        strcpy(graph->vertices[vertexId].name, graph->vertices[graph->numVertices - 1].name);
        graph->vertices[vertexId].id = vertexId;
        
        // �ƶ���·��Ϣ
        for (int i = 0; i < graph->numVertices; i++) {
            // ������
            graph->edges[vertexId][i] = graph->edges[graph->numVertices - 1][i];
            // ������
            graph->edges[i][vertexId] = graph->edges[i][graph->numVertices - 1];
        }
        
        // ������һ������ĵ�·��Ϣ
        for (int i = 0; i < graph->numVertices; i++) {
            graph->edges[graph->numVertices - 1][i].distance = 0;
            graph->edges[i][graph->numVertices - 1].distance = 0;
            strcpy(graph->edges[graph->numVertices - 1][i].roadName, "");
            strcpy(graph->edges[i][graph->numVertices - 1].roadName, "");
            graph->edges[graph->numVertices - 1][i].direction = -1;
            graph->edges[i][graph->numVertices - 1].direction = -1;
        }
    } else {
        // ���ɾ���������һ�����㣬ֻ��������·��Ϣ
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

// �������Ʋ��ҵص�ID
int findVertexByName(const Graph* graph, const char* name) {
    for (int i = 0; i < graph->numVertices; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return i; // ���صص�ID
        }
    }
    return -1; // δ�ҵ�
}

// ��ӡ���еص�
void printVertices(const Graph* graph) {
    if (graph->numVertices == 0) {
        printf("���޵ص���Ϣ\n");
        return;
    }
    
    for (int i = 0; i < graph->numVertices; i++) {
        printf("%d. %s\n", i+1, graph->vertices[i].name);
    }
}

// ��ӵ�·
int addEdge(Graph* graph, const char* start, const char* end, 
            int distance, const char* roadName, int direction) {
    int startId = findVertexByName(graph, start);
    int endId = findVertexByName(graph, end);
    
    if (startId == -1 || endId == -1 || distance <= 0) {
        return 0; // ��Ч����
    }
    
    // ���˫���·������ͼ��
    if (graph->edges[startId][endId].distance == 0) {
        graph->numEdges++;
    }
    
    graph->edges[startId][endId].distance = distance;
    strcpy(graph->edges[startId][endId].roadName, roadName);
    graph->edges[startId][endId].direction = direction;
    
    graph->edges[endId][startId].distance = distance;
    strcpy(graph->edges[endId][startId].roadName, roadName);
    graph->edges[endId][startId].direction = (direction + 180) % 360; // �����·����
    
    return 1;
}

// �޸ĵ�·��Ϣ
int modifyEdge(Graph* graph, const char* start, const char* end, 
               int newDistance, const char* newRoadName, int newDirection) {
    int startId = findVertexByName(graph, start);
    int endId = findVertexByName(graph, end);
    
    if (startId == -1 || endId == -1 || newDistance <= 0) {
        return 0; // ��Ч����
    }
    
    // ����·�Ƿ����
    if (graph->edges[startId][endId].distance == 0) {
        return 0; // ��·������
    }
    
    // �޸�˫���·��Ϣ
    graph->edges[startId][endId].distance = newDistance;
    strcpy(graph->edges[startId][endId].roadName, newRoadName);
    graph->edges[startId][endId].direction = newDirection;
    
    graph->edges[endId][startId].distance = newDistance;
    strcpy(graph->edges[endId][startId].roadName, newRoadName);
    graph->edges[endId][startId].direction = (newDirection + 180) % 360; // �����·����
    
    return 1;
}

// ɾ����·
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

// �����ͼ���ļ�
void saveMapToFile(const Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷����ļ�: %s\n", filename);
        return;
    }
    
    // д��ص�����
    fprintf(file, "%d\n", graph->numVertices);
    
    // д�����еص�����
    for (int i = 0; i < graph->numVertices; i++) {
        fprintf(file, "%s\n", graph->vertices[i].name);
    }
    
    // ����ʵ�ʱ�����ֻ����һ������ıߣ�
    int edgeCount = 0;
    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = i + 1; j < graph->numVertices; j++) {
            if (graph->edges[i][j].distance > 0) {
                edgeCount++;
            }
        }
    }
    
    // д�������
    fprintf(file, "%d\n", edgeCount);
    
    // д�����Ϣ��ֻ����i<j�ıߣ�
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

// ���ļ����ص�ͼ
int loadMapFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    // ��ʼ��ͼ
    initGraph(graph);
    
    // ��ȡ�ص�����
    fscanf(file, "%d", &graph->numVertices);
    fgetc(file); // �������з�
    
    // ��ȡ���еص�����
    for (int i = 0; i < graph->numVertices; i++) {
        fgets(graph->vertices[i].name, MAX_NAME_LEN, file);
        // ȥ�����з�
        graph->vertices[i].name[strcspn(graph->vertices[i].name, "\n")] = '\0';
        graph->vertices[i].id = i;
    }
    
    // ��ȡ������
    int edgeCount;
    fscanf(file, "%d", &edgeCount);
    fgetc(file); // �������з�
    
    // ��ȡÿ���ߵ���Ϣ
    char startName[MAX_NAME_LEN], endName[MAX_NAME_LEN], roadName[MAX_ROAD_NAME_LEN];
    int distance, direction;
    
    for (int i = 0; i < edgeCount; i++) {
        fscanf(file, "%s %s %d %s %d", 
               startName, endName, &distance, roadName, &direction);
        
        // ��ӱߣ����Զ���ӷ���ߣ�
        addEdge(graph, startName, endName, distance, roadName, direction);
    }
    
    fclose(file);
    return 1;
}

// Dijkstra�㷨ʵ��
void dijkstra(const Graph* graph, int start, int distances[], int predecessors[]) {
    int visited[MAX_VERTICES] = {0};
    
    // ��ʼ�������ǰ��
    for (int i = 0; i < graph->numVertices; i++) {
        distances[i] = INT_MAX;
        predecessors[i] = -1;
    }
    distances[start] = 0;
    
    for (int count = 0; count < graph->numVertices - 1; count++) {
        // �ҵ�δ���ʶ����о�����С��
        int minDist = INT_MAX;
        int minIndex = -1;
        
        for (int v = 0; v < graph->numVertices; v++) {
            if (!visited[v] && distances[v] < minDist) {
                minDist = distances[v];
                minIndex = v;
            }
        }
        
        if (minIndex == -1) break; // ����Ч�����˳�
        visited[minIndex] = 1; // ��Ǹö����Է���
        
        // �������ڶ���ľ���
        for (int v = 0; v < graph->numVertices; v++) {
            if (!visited[v] && 
                graph->edges[minIndex][v].distance > 0 && 
                distances[minIndex] != INT_MAX &&  // �����
                distances[minIndex] + graph->edges[minIndex][v].distance < distances[v]) {
                
                distances[v] = distances[minIndex] + graph->edges[minIndex][v].distance;
                predecessors[v] = minIndex;
            }
        }
    }
}

// ��ӡ·��
void printPath(const Graph* graph, int start, int end, const int predecessors[]) {
    if (end == start) {
        printf("%s", graph->vertices[start].name);
    } else if (predecessors[end] == -1) {
        printf("û��·��");
    } else {
        printPath(graph, start, predecessors[end], predecessors);
        printf(" -> %s", graph->vertices[end].name);
    }
}

// ��ӡ�ӵ�ǰλ�õ����еص�ľ���
void printAllDistancesFromLocation(const Graph* graph, const char* location) {
    int start = findVertexByName(graph, location);
    if (start == -1) {
        printf("δ�ҵ��õص�: %s\n", location);
        return;
    }
    
    int distances[MAX_VERTICES];
    int predecessors[MAX_VERTICES];
    
    dijkstra(graph, start, distances, predecessors);
    
    printf("\n�� %s �����ص����:\n", location);
    printf("%-20s %-10s %s\n", "�ص�", "����(��)", "·��");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < graph->numVertices; i++) {
        if (i != start) {
            printf("%-20s %-10d ", graph->vertices[i].name, distances[i]);
            printPath(graph, start, i, predecessors);
            printf("\n");
        }
    }
}

// ������������·��
void findShortestPath(const Graph* graph, const char* startName, const char* endName) {
    int start = findVertexByName(graph, startName);
    int end = findVertexByName(graph, endName);
    
    if (start == -1) {
        printf("δ�ҵ����: %s\n", startName);
        return;
    }
    if (end == -1) {
        printf("δ�ҵ��յ�: %s\n", endName);
        return;
    }
    
    int distances[MAX_VERTICES];
    int predecessors[MAX_VERTICES];
    
    dijkstra(graph, start, distances, predecessors);
    
    if (distances[end] == INT_MAX) {
        printf("�� %s �� %s û��·��\n", startName, endName);
        return;
    }
    
    printf("\n���·����Ϣ:\n");
    printf("���: %s\n�յ�: %s\n", startName, endName);
    printf("�ܾ���: %d ��\n", distances[end]);
    printf("·��: ");
    printPath(graph, start, end, predecessors);
    
    // ��ӡ��ϸ·����Ϣ
    printf("\n\n��ϸ·������:\n");
    int current = end;
    int path[MAX_VERTICES];
    int pathLength = 0;
    
    // �洢·��
    while (current != -1) {
        path[pathLength++] = current;
        current = predecessors[current];
    }
    
    // �����ӡ·��
    for (int i = pathLength - 1; i > 0; i--) {
        int from = path[i];
        int to = path[i-1];
        printf("%d. �� %s �� %s\n", pathLength - i, graph->vertices[from].name, graph->vertices[to].name);
        printf("   ��·: %s, ����: %d��, ����: %d��\n", 
               graph->edges[from][to].roadName,
               graph->edges[from][to].distance,
               graph->edges[from][to].direction);
    }
    printf("\n");
}

// ��ʾ�˵�
void displayMenu() {
    printf("\n===== У԰��ͨ��ѯ����ϵͳ =====\n");
    printf("1. ���ص�ͼ�ļ�\n");
    printf("2. �����ͼ�ļ�\n");
    printf("3. ��ӵص�\n");
    printf("4. �޸ĵص�����\n");
    printf("5. ɾ���ص�\n");  
    printf("6. ��ӵ�·\n");
    printf("7. �޸ĵ�·\n");
    printf("8. ɾ����·\n");
    printf("9. ��ʾ���еص�\n");
    printf("10. ��ѯ��ǰλ�õ����ص����\n");
    printf("11. ��ѯ��������·��\n");
    printf("12. �˳�ϵͳ\n");
    printf("===============================\n");
}
