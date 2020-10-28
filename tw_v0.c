// COMP2521 20T2 Assignment 1
// tw.c ... compute top N most frequent words in file F
// Usage: ./tw [Nwords] File

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "Dict.h"
#include "WFreq.h"
#include "stemmer.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100
#define TRUE    1
#define FALSE   0

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

int main( int argc, char *argv[])
{
   // TODO ... add any variables you need

   FILE  *in;         // currently open file
   Dict   stopwords;  // dictionary of stopwords
   Dict   wfreqs;     // dictionary of words from book
   WFreq *results;    // array of top N (word,freq) pairs
                      // (dynamically allocated)

   char *fileName;    // name of file containing book text
   int   nWords;      // number of top frequency words to show

   char   line[MAXLINE];  // current input line
   char   word[MAXWORD];  // current word

   // process command-line args
   switch (argc) {
   case 2:
      nWords = 10;
      fileName = argv[1];
      break;
   case 3:
      nWords = atoi(argv[1]);
      if (nWords < 10) nWords = 10;
      fileName = argv[2];
      break;
   default:
      fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   // build stopword dictionary
   in = fopen("stopwords", "r");

   if (in == NULL) {
      fprintf(stderr, "Can't open stopwords\n");
      exit(EXIT_FAILURE);
   }
   stopwords = newDict();
   int len;
   while (fgets(line, sizeof(line), in) != NULL) {
      // remove \n
      len = strlen(line);
      if (line[len - 1] == '\n') {
         line[len - 1] = 0;
      }
      // insert into dictionary of stopwords
      DictInsert(stopwords, line);
   }

   // scan File, up to start of text
   in = fopen(fileName, "r");

   if (in == NULL) {
      fprintf(stderr, "Can't open %s\n", fileName);
      exit(EXIT_FAILURE);
   }
   wfreqs = newDict();

   int valid = FALSE;
   while (fgets(line, sizeof(line), in) != NULL && valid == FALSE) {
      // check if start of book
      if (strstr(line, STARTING) == NULL) {
         continue;
      } else {
         valid = TRUE;
         break;
      }
   }

   // if start wasnt found, not PG
   if (valid == FALSE) {
      fprintf(stderr, "Not a project Gutenberg book\n");
      exit(EXIT_FAILURE);
   }
   
   // scan File reading words and accumualting counts
   while (fgets(line, sizeof(line), in) != NULL) {

      // check if end of book
      if (strstr(line, ENDING) != NULL) {
         break;
      }  

      // insert into dictionary of words
      // For each word in a line check the following:
      char *token = strtok(line, " ");
      while (token != NULL) {
         // split line at spaces and store in word array
         char *w = strdup(token);
         if (!isWordChar(*w)) {
            w++;
         }
         if (!isWordChar(*w)) {
            w++;
         }
         if (!isWordChar(*w)) {
            w++;
         }
         if (!isWordChar(*w)) {
            w++;
         }
         if (!isWordChar(*w)) {
            w++;
         }
         if (!isWordChar(*w)) {
            w++;
         }
         strcpy(word, w);

         // ensure word is valid

         int word_len = strlen(word);

         for (int i = 0; i < word_len; i++) {
            if (!isWordChar(word[i])) {
               /* if (i == 0) {
                  for (int j = 0; j < word_len-1; j++) {
                     word[j] = word[j + 1];
                  }
                  word[word_len - 1] = 0;
               
               } else {
               */
               word[i] = 0;
               // }
            }
         }

         // set lower case for valid words
         word_len = strlen(word);
         if (word_len > 1) {
            for (int i = 0; word[i]; i++) {
               word[i] = tolower(word[i]);
            }

            // printf("%s ", word);

            // if word is not a stop word
            if (DictFind(stopwords, word) == NULL) {
               // apply stemmer to word
               int k = stem(word, 0, word_len-1);
               word[k+1] = 0;
               // if not in dictionary, add
               WFreq *wrd = NULL;
               wrd = DictFind(wfreqs, word);
               if (wrd == NULL) {
                  DictInsert(wfreqs, word);
               } else {
                  wrd->freq++;
               }
            }
         }
         token = strtok(NULL, " ");
      }
   }

   // compute and display the top N words
   // ----------------------------------
   results = malloc(nWords * sizeof(WFreq)); // n places allocated
   nWords = findTopN(wfreqs, results, nWords);

   // done !
   return EXIT_SUCCESS;
}