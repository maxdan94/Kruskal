# Kruskal
Efficient implementation of Kruskal's algorithm using a UnionFind datastruture

## Info

Feel free to use these lines as you wish.  
Efficient implementation of Kruskal's algorithm using a UnionFind datastruture.  
https://en.wikipedia.org/wiki/Kruskal's_algorithm  
https://en.wikipedia.org/wiki/Disjoint-set_data_structure  

Should scale to at least one billion edges on a commodity machine.

## To compile

"gcc kruskal.c -O9 -o kruskal".

## To execute

"./kruskal edgelist.txt res.txt"
- "edgelist.txt" should contain one weighted edge on each line "u v w", where u and v are node IDs (unsigned int) and w is the edge weight (double). Better if node IDs go from zero to number of nodes minus 1. The input weighted graph should be connected, if not a forest will be obtained.
- "res.txt" will contain the list of the edges of the resulting tree.
- Will print the sum of the weights of a minimal spaning tree in the terminal.

## Initial contributors

Maximilien Danisch  
December 2017  
http://bit.ly/danisch  
maximilien.danisch@gmail.com


