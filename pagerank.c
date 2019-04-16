// pagerank.c by Christopher Shi and William Chen

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Graph.h"
#include "readData.h"
#include "pagerank.h"

#define MAXSTR 1000

int main (int argc, char *argv[]) {
	if (argc != 4) {
		perror ("Usage: ./pagerank d diffPr maxIterations\n");
		exit(1);
	}

	// Attain input from argument line 
	float d = atof(argv[1]);
	float diffPr = atof(argv[2]);
	int maxIterations = atoi(argv[3]);

	// Transfer collection.txt urls into char array called files
	char **files = getCollection();
	char **filesDup = getCollection();

	if (files == NULL) {
		perror ("No file: collections.txt\n");
		exit(1);
	}
	
	// Setup graph
	Graph g = getGraph(files);

	// Calculating page rank
	double *pageRank = calculatePageRank(g, d, diffPr, maxIterations);

	// Sort page rank in descending order
	sortPageRank(g, pageRank, files);

	FILE *f = fopen("pagerankList.txt", "w");
	if (f == NULL) {
		perror ("Cannot write to folder\n");
		exit(1);
	}

	int i;
	for (i = 0; i < numVertices(g); i++) {
		// Storing data into file: pagerankList.txt
		fprintf(f, "%s, %d, %.7lf\n", files[i], 
			findOutDegrees(g, urlToVertex(filesDup, files[i])), pageRank[i]);
	}
	fclose(f);

	// Freeing files and filesDup arrays
	for (i = 0; i < numVertices(g); i++) {
		free(files[i]);
		free(filesDup[i]);
	}
	free(files);
	free(filesDup);

	free(pageRank);
	dropGraph(g);

	return 0;
}

// sortPageRank
void sortPageRank(Graph g, double *pageRank, char **files) {
	float temp = 0;
	char temp_files[MAXSTR];
	int i;
	for (i = 0; i < numVertices(g); i++) {
		int j;
		for (j = 1; j < numVertices(g); j++) {
			if (pageRank[j-1] < pageRank[j]) {
				// Sorting pageRank in descending order
				temp = pageRank[j-1];
				pageRank[j-1] = pageRank[j];
				pageRank[j] = temp;
				// Mimic order with files array
				strcpy(temp_files, files[j-1]);
				strcpy(files[j-1], files[j]);
				strcpy(files[j], temp_files);
			}
		}
	}
}
