// Graph.c ... implementation of Graph ADT
// by Christopher Shi and William Chen

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "Graph.h"

// Graph representation (adjacency matrix)
typedef struct GraphRep {
	int 	nV; 		// #vertices
	int 	nE; 		// #edges
	int **edges; 		// Matrix of edges
} GraphRep;

int numVertices(Graph g) {
	return g->nV;
}

int numEdges(Graph g) {
	return g->nE;
}

// Check validity of Vertex
int validV(Graph g, Vertex v) {
	// Vertex is valid if
	// 	- graph is not null
	// 	- vertex is not negative
	// 	- and less than #vertices
	return (g != NULL && v >= 0 && v < g->nV);
}

// Insert an Edge;
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt) {
	assert(g != NULL && validV(g,v) && validV(g,w));
	// Only add in new edge if theres is no current edge
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->nE++;
	}
}

// Remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w) {
	assert(g != NULL && validV(g,v) && validV(g,w));
	// Only removes an edge if there is a current edge
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->nE--;
	}
}

// Create an empty graph
Graph newGraph(int nV) {
	assert(nV > 0);
	int v, w;
	// Mallocing enough space for graph struct
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; 
	new->nE = 0;
	// Mallocing enough space for adjacency matrix (v)
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);

	// Mallocing enough space for adjacency matrix (w)
	// Initialises all matrix values to 0
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// Free memory associated with graph
void dropGraph(Graph g) {
	if (g == NULL) return;

	int i;
	for(i = 0; i < g->nV; i++) {
		free(g->edges[i]);
	}
	free(g->edges);
	free(g);
}

// Display graph, using names for vertices
void showGraph(Graph g, char **names) {
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

// Calculates an amount of inlinks to given vertex
int findInDegrees(Graph g, Vertex v) {
	int inDegree = 0;
	
	int i;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[i][v] == 1) {
			inDegree++;
		}
	}
	return inDegree;
}

// Calculates an amount of outlinks to given vertex
int findOutDegrees(Graph g, Vertex v) {
	int outDegree = 0;

	int i;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[v][i] == 1) {
			outDegree++;
		}
	}

	return outDegree;
}

// Calculate weighting(in)
double getWIn(Graph g, Vertex v,  Vertex u) {
	// Numerator = inlinks(u)
	int num = findInDegrees(g, u);
	// Denominator = inlinks of all the outlinked nodes of (v)
	int den = 0;
	int i;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[v][i] == 1) {
			den += findInDegrees(g, i);
		}
	}
	return (double) num/den;
}

// Calculate weighting(out)
double getWOut(Graph g, Vertex v, Vertex u) {
	// Numerator = outlinks(u)
	double num = 0;
	if (findOutDegrees(g,u) == 0) {
		num += 0.5;
	} else {
		num += findOutDegrees(g, u);
	}

	// Denominator = outlinks of all the outlinked nodes of (v)
	double den = 0;
	int i;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[v][i] == 1) {
			if (findOutDegrees(g, i) == 0) {
				den += 0.5;
			} else {
				den += findOutDegrees(g, i);
			}
		}
	}
	
	return num/den;
}

// Calculates Weighted PageRank for each vertex in graph g using
// 	- Damping factor (d)
// 	- Difference in PageRank sum (diffPr)
// 	- Maximum iteations (maxIterations)
// Returns float array containing PageRank values
double *calculatePageRank(Graph g, float d, float diffPr, int maxIterations) {
	// setup and initialise PageRank float array to 1/N
	double *pageRank = malloc((g->nV) * sizeof(double));
	int i;
	for (i = 0; i < g->nV; i++) {
		pageRank[i] = (double) 1 / g->nV;
	}

	int iteration = 0;
	float diff = diffPr;

	while (iteration < maxIterations && diff >= diffPr) {
		diff = 0;

		// Calculate new PageRank for each element(i) in PageRank array
		int i;
		for (i = 0; i < g->nV; i++) {
			// Store current PageRank of element(i)
			double oldPR = pageRank[i];
			double sum = 0;

			// Calculates product of (PR(j), wIN(j, i), wOUT(j, i)) and 
			// sums them together for all j which are vertices containing
			// outlinks to i.
			int j;
			for (j = 0; j < g->nV; j++) {
				if (g->edges[j][i] == 1) {
					double wIn = getWIn(g, j, i);
					double wOut = getWOut(g, j, i);
					sum += (double) (pageRank[j] * wIn * wOut);
				}
			}

			pageRank[i] = ((1-d)/(g->nV)) + d * sum;
			diff += fabs(pageRank[i] - oldPR);
		}
		iteration++;		
	}

	return pageRank;
}
