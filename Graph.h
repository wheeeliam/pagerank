// Graph.h ... interface to Graph ADT
// by Christopher Shi and William Chen

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

// Graph representation is hidden
typedef struct GraphRep *Graph;

// Vertices denoted by integers 0..N-1
typedef int Vertex;

int numVertices(Graph g);
int numEdges(Graph g);
int validV(Graph,Vertex);

// Edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex w; } Edge;
void insertEdge(Graph, Vertex, Vertex, int);
void removeEdge(Graph, Vertex, Vertex);

// Operations on graphs
Graph newGraph(int nV);
void dropGraph(Graph);
void showGraph(Graph, char **);

int inDeogrees(Graph, Vertex);
int findOutDegrees(Graph, Vertex);
double getWIn(Graph, Vertex,  Vertex);
double getWOut(Graph, Vertex, Vertex);
double *calculatePageRank(Graph, float, float, int);

#endif
