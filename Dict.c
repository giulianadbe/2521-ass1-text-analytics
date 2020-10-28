// COMP2521 20T2 Assignment 1
// Giuliana De Bellis z5309221 June/July 2020
// Dict.c ... implementsation of Dictionary ADT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Dict.h"
#include "WFreq.h"

#define TRUE 1
#define FALSE 0

typedef struct _DictNode *Link;

typedef struct  _DictNode {
   WFreq  data;
   Link   left;
   Link   right;
   int height;
} DictNode;

struct _DictRep {
   Link tree;
   Link newest;
};

// function prototypes (helper functions)
void DictPrefix(Link tree);
void showDictNode(Link tree);
Link newNode(char *w, Dict d);
Link findInsert(Link tree, char *w, Dict d);
Link findWord(Link tree, char *w);
static int height(Link tree);
static int max(int a, int b);
static Link rotateLeft(Link node);
static Link rotateRight(Link node);
WFreq *searchCommon(Link tree, WFreq *wfs, int n);
WFreq *shiftArray(WFreq *wfs, int i, int n);
WFreq *insertArray(Link tree, WFreq *wfs, int i);

// create new empty Dictionary
Dict newDict()
{
   // allocate memory for a new _DictRep
   // point to this with new
   Dict new = malloc(sizeof(struct _DictRep));
   // set the pointer to null for now, no nodes
   new->tree = NULL;
   new->newest = NULL;
   return new;
}

// make a new node containing string w
Link newNode(char *w, Dict d) 
{
   // create new node
   Link node = malloc(sizeof(struct _DictNode));
   char *word = strdup(w);
   // set word char to be word
   node->data.word = word;
   // frequency starts at 1
   node->data.freq = 1;
   // not connected yet
   node->left = node->right = NULL;
   // no nodes below yet
   node->height = 0;
   d->newest = node;
   return node;
}

// calculate height of node
static int height(Link tree)
{
   if (tree == NULL) {
      return -1;
   } 
   else {
      return tree->height;
   }

}

// helper function to calculate height of node with two subtrees
static int max(int a, int b) {
   return a > b ? a : b;
}

// rotate given subtree left and return root of updated subtree
static Link rotateLeft(Link node)
{
   if (node == NULL) {
      return NULL;
   }
   Link child = node->right;
   if (child == NULL) {
      return node;
   }
   node->right = child->left;
   child->left = node;

   node->height = 1 + max(height(node->left), height(node->right));
   child->height = 1 + max(height(child->left), height(child->right));
   return child;
}

// rotate given subtree right and return root of the updated subtree
static Link rotateRight(Link node)
{
   if (node == NULL) {
      return NULL;
   }
   Link child = node->left;
   if (child == NULL) {
      return node;
   }
   node->left = child->right;
   child->right = node;

   node->height = 1 + max(height(node->left), height(node->right));
   child->height = 1 + max(height(child->left), height(child->right));
   return child;
}

// insert new word into Dictionary
// return pointer to the (word,freq) pair for that word
WFreq *DictInsert(Dict d, char *w)
{
   // if the tree is empty, insert at root
   Link node = NULL;

   node = findInsert(d->tree, w, d);
   d->tree = node;
   return &(d->newest->data);
}

// insertion into AVL tree, including rebalancing after each insert
Link findInsert(Link tree, char *w, Dict d) {
   if (tree == NULL) {
      return newNode(w, d);
   }
   int cmp = strcmp(w, tree->data.word);
   // if inserted word is 'less than' curr, go left
   if (cmp < 0) {
      tree->left = findInsert(tree->left, w, d);
   // if inserted word is 'greater than' curr, go right
   } else if (cmp > 0) {
      tree->right = findInsert(tree->right, w, d);
   }

   // Insert done, now rebalance
   tree->height = 1 + max(height(tree->left), height(tree->right));
   int rightH = height(tree->right);
   int leftH = height(tree->left);

   if ((leftH - rightH) > 1) { // left heavier than right
   // if word 'greater than' curr, rotate right
      if (cmp < 0) {// strcmp(w, tree->data.word) < 0
         tree = rotateRight(tree);
      } else { // if 'less than' curr, move to median first, then rot right
         tree->left = rotateLeft(tree->left);
         tree = rotateRight(tree);
      }
   } else if ((rightH - leftH) > 1) { // right heavier than left
      if (cmp > 0) { // if word 'less than' curr, rot left
         tree = rotateLeft(tree);
      } else { // if 'greater than' curr, move to median first, then rot left
         tree->right = rotateRight(tree->right);
         tree = rotateLeft(tree);
      }
   }

   return tree;
}

// find Word in Dictionary
// return pointer to (word,freq) pair, if found
// otherwise return NULL
WFreq *DictFind(Dict d, char *w)
{
   Link found = NULL;
   if (d->tree == NULL) {
      return NULL;
   } else { // find 
      found = findWord(d->tree, w);
      if (found != NULL) {
         return &(found->data);
      } else {
         return NULL;
      }
   }
}

// helper function for DictFind
Link findWord(Link tree, char *w) 
{
   // word not found
   if (tree == NULL) {
      return NULL;
   }
   if (strcmp(w, tree->data.word) == 0) { // found
      return tree;
   } else if (strcmp(w, tree->data.word) < 0) {
      return findWord(tree->left, w);
   } else if (strcmp(w, tree->data.word) > 0) {
      return findWord(tree->right, w);
   } 
   return tree;
}

// Helper function for findTopN --> returns pointer to WFreq pair
WFreq *searchCommon(Link tree, WFreq *wfs, int n)
{
   // if the dictionary tree is empty
   if (tree == NULL) {
      return wfs;
   }

   // loop through array of wfs and compare node data
   for (int i = 0; i < n; i++) {
      if (wfs[i].freq < tree->data.freq) {
         if (wfs[i].freq == 0) { // if freq = 0, insert
            insertArray(tree, wfs, i);
            break;
         } else { // if node freq > array freq, insert
            shiftArray(wfs, i, n);
            insertArray(tree, wfs, i);
            break;
         }
      } else if (wfs[i].freq == tree->data.freq) { // if freq are equal, compare letters
         if (strcmp(wfs[i].word, tree->data.word) > 0) {
            shiftArray(wfs, i, n);
            insertArray(tree, wfs, i);
            break;
         }
      }
   }
   searchCommon(tree->left, wfs, n);
   searchCommon(tree->right, wfs, n);
   return wfs;
}

// helper for searchCommon
// pushes array elements to insert new wfs with higher freq
WFreq *shiftArray(WFreq *wfs, int i, int n) 
{
   for (int j = n - 1; j > i; j--) {
      wfs[j] = wfs[j - 1];
   }
   return wfs;
}

// helper for searchCommon
// insert tree->data of higher freq into array of top N freq
WFreq *insertArray(Link tree, WFreq *wfs, int i) 
{
   char *word = strdup(tree->data.word);
   wfs[i].word = word;
   wfs[i].freq = tree->data.freq;
   return wfs;
}

// find top N frequently occurring words in Dict
// input: Dictionary, array of WFreqs, size of array
// returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n)
{
   // set all array values of frequencies to be 0 initially
   for (int i = 0; i < n; i++) {
      wfs[i].freq = 0;
      wfs[i].word = "~";
   }
   searchCommon(d->tree, wfs, n);

   // print array wfs
   for (int i = 0; i < n; i++) {
      printf("%7d %s\n", wfs[i].freq, wfs[i].word);
   }
   return n;
}

// display Dict root node
void showDictNode(Link tree) 
{
   // if Dict tree empty, return
   if (tree == NULL) {
      return;
   }
   printf("%d %s, ", tree->data.freq, tree->data.word);
}

// Print a dictionary in prefix order
void DictPrefix(Link tree)
{
   // if the dictionary tree is empty
   if (tree == NULL) {
      return;
   }
   showDictNode(tree);
   DictPrefix(tree->left);
   DictPrefix(tree->right);
   return;
}

// print a dictionary
void showDict(Dict d)
{
   DictPrefix(d->tree);
   printf("X. \n");
   return;
}