// BSTree.c ... implementation of binary search tree ADT
// by Christopher Shi and William Chen

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"

#define TRUE 1
#define FALSE 0

// BSTree ADT adapted from BSTree.c (CS2521 lab10)
typedef struct BSTNode {
	char *word;
    ListNode *urlList;

	Tree left;
    Tree right;
} BSTNode;

// Create a new ListNode and returns it
ListNode *createListNode(char *url) {
    ListNode *urlList = malloc(sizeof(ListNode));
    assert(urlList != NULL);

    urlList->url = strdup(url);
    urlList->next = NULL;

    return urlList;
}

// Adds a new ListNode containing *url to the end of head
void addListNode(ListNode *head, char *url) {
    ListNode *curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    if (strcmp(curr->url, url) == 0) return;

    curr->next = createListNode(url);
}

// Prints all listnodes of a particular urlList head
void showListNodes(ListNode *head, FILE *f) {
    ListNode *curr = head;
    while (curr != NULL) {
        fprintf (f, "%s ", curr->url);
        curr = curr->next;
    }
    fprintf (f, "\n");
}

// Frees list node
void freeListNode(ListNode *head) {
    ListNode *prev = head;  
    while (head != NULL) {
        prev = head;
        head = head->next;
        free(prev->url);;
        free(prev);
        prev = NULL;
    }
}

// Makes a new node given a token and url
Tree newTree(char *token, char *url) {
    Tree new = malloc(sizeof(BSTNode));
    assert(new != NULL);

    new->word = strdup(token);
    new->urlList = createListNode(url);
    new->left = new->right = NULL;

    return new;
}

// Inserts a given node into the BST
Tree treeInsert(Tree node, char *token, char *url) {
    if (node == NULL) {
        return newTree(token, url);
    } else if (strcmp(token, node->word) < 0) {
        node->left = treeInsert(node->left, token, url);
    } else if (strcmp(token, node->word) > 0) {
        node->right = treeInsert(node->right, token, url);
    }

    return node;
}

// Shows all elements of the BST using in-order traversal
void showInOrder(Tree node, FILE *f) {
    if (node == NULL) return;
    showInOrder(node->left, f);
    fprintf (f, "%s  ", node->word);
    showListNodes(node->urlList, f);
    showInOrder(node->right, f);
}

// Checks if a given token is in the tree
// Adds in extra urls if it is
int checkTok(Tree node, char *token, char *url) {
    if (node == NULL) return FALSE;        
    if (strcmp(token, node->word) == 0) {
        addListNode(node->urlList, url);
        return TRUE;
    }

    int found_left = checkTok(node->left, token, url);
    int found_right = checkTok(node->right, token, url);

    if (found_left == TRUE || found_right == TRUE) {
        return TRUE;
    }

    return FALSE;
}

// Free memory associated with BSTree
void dropTree(Tree node) {
    if (node == NULL) return;
    dropTree(node->left);
    dropTree(node->right);
    freeListNode(node->urlList);
    free(node);
}
