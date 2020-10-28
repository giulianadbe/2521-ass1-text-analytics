// Program to test dict.c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Dict.h"
#include "WFreq.h"

#define MAX 1000

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

int main (void) 
{
    // create dictionary test
    Dict tree = newDict();
    // show empty dictionary (NULL) test
    showDict(tree);

    // Test Dict Insert
    WFreq *node1 = DictInsert(tree, "a");
    showDict(tree);
    WFreq *node2 = DictInsert(tree, "b");
    showDict(tree);
    TreeShow(tree);
    WFreq *node3 = DictInsert(tree, "c");
    printf("Returned: %d %s\n", node3->freq, node3->word);
    TreeShow(tree);
    showDict(tree);

    // Test insertion lexographically (check against print!)
    WFreq *node4 = DictInsert(tree, "d");
    showDict(tree);
    TreeShow(tree);
    WFreq *node5 = DictInsert(tree, "d");
    showDict(tree);
    TreeShow(tree);
    WFreq *node6 = DictInsert(tree, "f");
    showDict(tree);
    // printf("Found: %s of frequency %d\n", node6->word, node6->freq);
    WFreq *node7 = DictInsert(tree, "a");
    showDict(tree);
    TreeShow(tree);
    WFreq *node8 = DictInsert(tree, "b");
    showDict(tree);
    TreeShow(tree);
    WFreq *node9 = DictInsert(tree, "z");
    showDict(tree);
    TreeShow(tree);
    // Test Dict Find
    WFreq *find = NULL;
    find = DictFind(tree, "hello");
    // printf("Found: %s of frequency %d\n", find->word, find->freq);
    
    printf("---------- FILE TESTS ----------\n");
    FILE *in;
    Dict filew;
    char line[MAX];

    filew = newDict();
    in = fopen("test.txt", "r");

    int len;
    while (fgets(line, sizeof(line), in) != NULL) {
        len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = 0;
        }
        DictInsert(filew, line);
    }
    find = DictFind(filew, "z");
    printf("Found: %s of frequency %d\n", find->word, find->freq);
    printf("\n");
    printf("---------- N ARRAY TESTS ----------\n");
    WFreq *results;
    int n = 3;
    results = malloc(n * sizeof(WFreq));
    n = findTopN(tree, results, n);

    printf("\n");
    printf("---------- character from string ----------\n");

    char *lines = "hello my name is";
    char word[100];
    int i = 0;
    int j = 0;
    int line_len = strlen(lines);
    printf("%d\n", line_len);
    while (i < line_len) {
        if (isWordChar(lines[i])) {
            word[j] = tolower(lines[i]);
            j++;
        } else {
            word[j] = '\0';
            j = 0;
            printf("Word: %s\n", word);
            printf("%d\n", i);
        }
        i++;
    }

}