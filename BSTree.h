// BSTree.h ... interface to binary search tree ADT`
// by Christopher Shi and William Chen

#ifndef BSTREE_H
#define BSTREE_H

#include <stdio.h>

// extern char *strdup(char *);

typedef struct BSTNode *Tree;

// ListNode ADT adapted from textbuffer.h
typedef struct _listNode {
    char *url;
    struct _listNode *next;
} _listNode;

typedef _listNode ListNode;

ListNode *createListNode(char *);
void addListNode(ListNode *head, char *url);
void showListNodes(ListNode *, FILE *);
void freeListNode(ListNode *);

Tree newTree(char *, char *);
Tree treeInsert(Tree, char *, char *);
void showInOrder(Tree, FILE *);
int checkTok(Tree, char *, char *);
void dropTree(Tree);

#endif
