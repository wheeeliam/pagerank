// readData.h by Christopher Shi and William Chen

#ifndef READ_H
#define READ_H

#include "Graph.h"
#include "BSTree.h"

Vertex urlToVertex(char **, char *);
char *vertexToUrl(char **, Vertex);
void trim(char *str);
void normalise(char *tok);	

char **getCollection(void);
Graph getGraph(char **files);
Tree getInvertedList(char **);

#endif
