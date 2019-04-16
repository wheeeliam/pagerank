// readData.c by Christopher Shi and William Chen

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "Graph.h"
#include "BSTree.h"

#define MAXSTR 1000
#define TRUE 1
#define FALSE 0

// Searches for tok in **files - returns vertex
Vertex urlToVertex(char **files, char *tok) {
	int i = -1;

	for (i = 0; files[i] != NULL; i++) {
		if (strcmp(tok, files[i]) == 0) {
			break;
		}
	}

	return i;
}


// Given vertex number - return URL
char *vertexToUrl(char **files, Vertex v) {
	int i = 0;
	while (i < v) i++; 

	return files[i];
}

// trim: remove leading/trailing spaces from a string
// adapted from mymysh.c (cs1521 ass2)
void trim(char *str)
{
   int first, last;
   first = 0;
   while (isspace(str[first])) first++;
   last  = strlen(str)-1;
   while (isspace(str[last])) last--;
   int i, j = 0;
   for (i = first; i <= last; i++) str[j++] = str[i];
   str[j] = '\0';
}

// normalises text by
// 	- trimming whitespace
// 	- truncating miscellaneous last characters
// 	- converting capital characters into lowercase
void normalise(char *tok) {
	trim(tok);
	
	int length = strlen(tok);
	if (tok[length-1] == '.' || tok[length-1] == ',' || tok[length-1] == ';' ||
		tok[length-1] == '?' || tok[length-1] == '\n') {
			tok[length-1] = '\0';
	}

	int i;
	for (i = 0; i < length; i++) {
		if (tok[i] >= 'A' && tok[i] <= 'Z') {
			// 'a' - 'A' = 32
			tok[i] += 32;
		} 
	}
}

// Create a set (list) of urls to process 
char **getCollection(void) {
	FILE *f = fopen("collection.txt", "r");
	if (f == NULL) {
		return NULL;
	}

	int i = 0;
	char **url_list = malloc(MAXSTR * sizeof(char *));
	char filename[MAXSTR];

	// will only fill up 100x100 char matrix
	while (fscanf(f, "%s ", filename) == 1) {
		url_list[i] = malloc(strlen(filename));
		strcpy(url_list[i], filename);
		i++;
	}

	fclose(f);

	return url_list;
}

// Creates graph and traverses URL's to fill in matrix
Graph getGraph(char **files) {
	int i = 0;
	while (files[i] != NULL) {
		i++;
	}

	// Create empty graph
	Graph g = newGraph(i);
	for (i = 0; files[i] != NULL; i++) {
		char *filename = malloc(strlen(files[i]) + 4 * sizeof(char));
		strcpy(filename, files[i]);
		strcat(filename, ".txt");

		FILE *f = fopen(filename, "r");
		if (f == NULL) continue;

		char buf[MAXSTR];
		while (fgets(buf, sizeof(buf), f) != NULL) {
			if (strcmp(buf, "#start Section-1\n") == 0 ||
				strcmp(buf, "\n") == 0) {
				continue;
			}
			if (strcmp(buf, "#end Section-1\n") == 0) {
				break;
			}

			char *tok = strtok(buf, " ");
			while (tok != NULL && strcmp(tok, "\n\0") != 0) {
				if (strcmp(tok, files[i]) == 0) {
					tok = strtok(NULL, " ");
					continue;
				}
				normalise(tok);
				Vertex v = i;
				Vertex w = urlToVertex(files, tok);

				// create edge in g
				insertEdge(g, v, w, 1);

				tok = strtok(NULL, " ");
			}

		}
		fclose(f);
	}

	return g;
}

// Creates BST and traverses files to populate a inverted index
Tree getInvertedList(char **files) {
	Tree rootNode = NULL;

	int i;
	for (i = 0; files[i] != NULL; i++) {
		char *filename = malloc(strlen(files[i]) + 4 * sizeof(char));
		strcpy(filename, files[i]);
		strcat(filename, ".txt");

		FILE *f = fopen(filename, "r");
		if (f == NULL) continue;

		char buf[MAXSTR];
		while (fgets(buf, sizeof(buf), f) != NULL) {
			if (strcmp(buf, "#start Section-2\n") == 0) {
				break;
			}
		}
		fgets(buf, sizeof(buf), f);

		// in main section 2
		while (fgets(buf, sizeof(buf), f) != NULL) {
			if (strcmp(buf, "#end Section-2\n") == 0) {
				break;
			}

			char *tok = strtok(buf, " ");
			while (tok != NULL) {
				if (strcmp(tok, "\n") == 0) {
					tok = strtok(NULL, " ");
					continue;
				}
				normalise(tok);		
				
				// base case
				if (rootNode == NULL) {
					rootNode = newTree(tok, files[i]);
					tok = strtok(NULL, " ");
					continue;
				}

				if (!checkTok(rootNode, tok, files[i])) {
					treeInsert(rootNode, tok, files[i]);
				}

				tok = strtok(NULL, " ");
			}
		}

		fclose(f);
	}

	return rootNode;
}
