#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 100

typedef struct {
  int vertex;
  int weight;
} Edge;

typedef struct {
  Edge *edges[MAX_VERTICES];
  int edgeCount[MAX_VERTICES];
  int vertexCount;
} Graph;

Graph *createGraph(int numVertices);

void resizeGraph(Graph *graph, int newNumVertices);

void addVertex(Graph *graph);

void addEdge(Graph *graph, int source, int destination, int weight);

void dijkstra(Graph *graph, int source);

void generateRandomEdges(Graph *graph, int numEdges);

void showAllEdges(Graph *graph);

void sceneManagement(Graph *graph);

int main(void) {
  int numVertices = 1;
  Graph *graph = createGraph(numVertices);

  sceneManagement(graph);

  return 0;
}

Graph *createGraph(int numVertices) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->vertexCount = numVertices;

  for (int i = 0; i < numVertices; i++) {
    graph->edgeCount[i] = 0;
    graph->edges[i] = NULL;
  }

  return graph;
}

void resizeGraph(Graph *graph, int newNumVertices) {
  if (newNumVertices <= graph->vertexCount)
    return;

  graph->vertexCount = newNumVertices;

  for (int i = 0; i < newNumVertices; i++) {
    graph->edges[i] =
        (Edge *)realloc(graph->edges[i], newNumVertices * sizeof(Edge));
    for (int j = graph->edgeCount[i]; j < newNumVertices; j++) {
      graph->edges[i][j].vertex = -1;
      graph->edges[i][j].weight = 0;
    }
  }
}

void addVertex(Graph *graph) {
  int newVertex = graph->vertexCount;

  if (newVertex >= MAX_VERTICES) {
    printf("Maximum ilosc stolow\n");
    return;
  }

  graph->vertexCount++;

  // Initialize the edge count for the new vertex to 0
  graph->edgeCount[newVertex] = 0;
  graph->edges[newVertex] = NULL;
}

void addEdge(Graph *graph, int source, int destination, int weight) {
  Edge *newEdge = (Edge *)malloc(sizeof(Edge));
  newEdge->vertex = destination;
  newEdge->weight = weight;

  graph->edges[source] = (Edge *)realloc(
      graph->edges[source],
      (long unsigned)(graph->edgeCount[source] + 1) * sizeof(Edge));
  graph->edges[source][graph->edgeCount[source]] = *newEdge;
  graph->edgeCount[source]++;
}

void dijkstra(Graph *graph, int source) {
  int distance[MAX_VERTICES];
  int predecessor[MAX_VERTICES];
  int visited[MAX_VERTICES];

  for (int i = 0; i < graph->vertexCount; i++) {
    distance[i] = INT_MAX;
    predecessor[i] = -1;
    visited[i] = 0;
  }

  distance[source] = 0;

  for (int count = 0; count < graph->vertexCount - 1; count++) {
    int minDistance = INT_MAX;
    int minVertex = -1;

    for (int i = 0; i < graph->vertexCount; i++) {
      if (visited[i] == 0 && distance[i] < minDistance) {
        minDistance = distance[i];
        minVertex = i;
      }
    }

    visited[minVertex] = 1;

    for (int i = 0; i < graph->edgeCount[minVertex]; i++) {
      int vertex = graph->edges[minVertex][i].vertex;
      int weight = graph->edges[minVertex][i].weight;
      int newDistance = distance[minVertex] + weight;

      if (visited[vertex] == 0 && newDistance < distance[vertex]) {
        distance[vertex] = newDistance;
        predecessor[vertex] = minVertex;
      }
    }
  }

  printf("Stol\tDystans\tPoprzednik\n");
  for (int i = 0; i < graph->vertexCount; i++) {
    printf("%d\t\t%d\t\t\t%d\n", i, distance[i], predecessor[i]);
  }
}

void generateRandomEdges(Graph *graph, int numEdges) {
  srand(time(NULL));

  int maxWeight = 10; // Maximum weight for the random edges

  for (int i = 0; i < numEdges; i++) {
    int source = rand() % graph->vertexCount;
    int destination = rand() % graph->vertexCount;
    int weight = rand() % maxWeight + 1;

    while (source == destination) {
      destination = rand() % graph->vertexCount;
    }

    addEdge(graph, source, destination, weight);
  }
}

void showAllEdges(Graph *graph) {
  for (int i = 0; i < graph->vertexCount; i++) {
    printf("Krawedzi od stolu %d:\n", i);

    for (int j = 0; j < graph->edgeCount[i]; j++) {
      Edge *edge = &graph->edges[i][j];
      printf("  Cel: %d, Odleglosc: %d\n", edge->vertex, edge->weight);
    }

    printf("\n");
  }
}

void sceneManagement(Graph *graph) {
  int choice;
  int source, destination, weight, number;

  while (1) {
    printf("\n\nMenu Glowne\n");
    printf("1. Dodaj stol\n");
    printf("2. Dodaj droge\n");
    printf("3. Uruchom algorytm Dijkstry\n");
    printf("4. Pokaz wszystkie drogi\n");
    printf("5. Modyfikuj ilosc stolow\n");
    printf("6. Wygeneroj nowe drogi\n");
    printf("7. Wyjdz\n");
    printf("Podaj wybor: ");
    scanf("%d", &choice);
    printf("\n");

    switch (choice) {
    case 1:
      addVertex(graph);
      break;
    case 2:
      printf("Podaj startowy stol: ");
      scanf("%d", &source);
      printf("Podaj koncowy stol: ");
      scanf("%d", &destination);
      printf("Podaj odleglosc: ");
      scanf("%d", &weight);
      addEdge(graph, source, destination, weight);
      break;
    case 3:
      printf("Podaj stol od ktorego zaczac: ");
      scanf("%d", &source);
      dijkstra(graph, source);
      break;
    case 4:
      showAllEdges(graph);
      break;
    case 5:
      printf("Podaj nowa ilosc stolow: ");
      scanf("%d", &(graph->vertexCount));
      resizeGraph(graph, graph->vertexCount);
      break;
    case 6:
      printf("Podaj ilosc nowych drog do wygenerowania: ");
      scanf("%d", &number);
      generateRandomEdges(graph, number);
      break;
    case 7:
      printf("Wychodzimy z programu...\n");
      return;
    default:
      printf("Nieprawidlowy wybor! Prosze sprobuj ponownie.\n");
      break;
    }
  }
}
