# 2521 Assignment 1, Text Analytics
- program which can extract one important text analytics "measure": frequency of occurrence of the most common individual words in the text
- tokenisng, stopword removal and stemming of words
- Using AVL binary search tree for an efficient dictionary data type (Dict.c)

takes one or two command line arguments (./tw Nwords File)

the first optional argument gives the number of words to be output
the second argument gives the name of a text file
If the Nwords argument is not given, the default value of 10 is used

reads text from the file, and computes word (stem) frequencies

prints a list of the top N most frequent words, from most frequent to least frequent
