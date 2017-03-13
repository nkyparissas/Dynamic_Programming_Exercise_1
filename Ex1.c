/*
* Technical University of Crete
* School of ECE
* Dynamic Programming Course - Exercise 1: Shortest Path in Acyclic Weighted Graph
* Authors: Maria Apostolidou (apostolidoum@github), Nick Kyparissas (nkyparissas@github), Sofia Maragkou (NOT @github, fake vsam offspring)
* Description: 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct graph{
    int **Cost;
    int **G;
    int numberOfNodes;
} graph;

typedef struct solution{
	int **OPT;
	int *nextNode;
} solution;
 
graph fill(int selection); 

void printCostTable(graph ourGraph);

solution solve(graph ourGraph);
 
void bestCost(int startNode, solution ourSolution, graph ourGraph);

void bestPath(int startNode, solution ourSolution, graph ourGraph); 
 
void freeGraph(graph ourGraph, solution ourSolution); 

int main()
{
    int i, j, selection, startNode;
    
    graph ourGraph;
    
    do{
    	printf("Method of input:\n1. Manual\n2. From file\n3. Exit\n");
	    scanf("%d", &selection);
	    fflush(stdin);
	    
	    if ((selection == 1)||(selection == 2))
	    	ourGraph = fill(selection);
	    else if (selection == 3)
	    	exit(0);
	    else
	    	printf("Invalid input, please try again.\n");	    	
	}while(selection != 1 && selection != 2);
    
    printCostTable(ourGraph);
	
	solution ourSolution = solve(ourGraph);
	
	printf("Please give starting node: \n");
	scanf("%d", &startNode);
	
	bestCost(startNode, ourSolution, ourGraph);
	bestPath(startNode, ourSolution, ourGraph);	
	
	freeGraph(ourGraph, ourSolution);
	
	printf("\n\nPress enter to exit.\n");
	fflush(stdin);
	getchar();
	
	return 0;
}

graph fill(int selection){
	
	int i, j, numberOfVa8m;
	
	graph ourGraph;
	
	FILE *inputfile;
    inputfile = fopen("graph2.txt", "r");
	
	if (selection == 1){
		printf("Number of graph's levels: \n");
		scanf("%d", &numberOfVa8m);
	}
	else
		fscanf(inputfile, "%d", &numberOfVa8m);
 	
 	int *B = (int *)malloc(numberOfVa8m * sizeof(int));
 	int max = -1;
	
	ourGraph.numberOfNodes = 0;
 	
	//Filling array B and finding max
    for (i = 0; i < numberOfVa8m; i++){
		if (selection == 1){
			printf("Number of nodes for level %d: \n", i);
			scanf("%d", &B[i]);
		}
		else
			fscanf(inputfile, "%d", &B[i]);
		ourGraph.numberOfNodes = ourGraph.numberOfNodes + B[i];
		if (B[i]>=max)
			max = B[i];
  	}
  	
  	//Creating the graph's array (array G)
  	ourGraph.G = (int **)malloc(max * sizeof(int *));
 	for (i = 0; i < max; i++)
        assert(ourGraph.G[i] = (int *)malloc(numberOfVa8m * sizeof(int)));
  	
  	//Filling the graph
	int node = 0;
	for (i = 0; i <= numberOfVa8m; i++){
    	for(j = 0; j < max; j++){
        	if (j < B[i]) {
            	ourGraph.G[j][i] = node;
             	node++;    
         	} 
			else ourGraph.G[j][i] = -1;
     	}
	}
 	 	
	printf("Number of nodes: %d\n", ourGraph.numberOfNodes);
	free(B);
	
	//Printing array G
	printf("Graph's nodes: \n");
	for (i = 0; i < max; i++){
    	for(j = 0; j < numberOfVa8m; j++)
    		if (ourGraph.G[i][j] == -1 )
				printf("\t");
			else
				printf("%5d\t", ourGraph.G[i][j]);
    	printf("\n");
	}
	
	ourGraph.Cost = (int **)malloc(ourGraph.numberOfNodes * sizeof(int *));
 	for (i = 0; i < ourGraph.numberOfNodes; i++)
        assert(ourGraph.Cost[i] = (int *)malloc(ourGraph.numberOfNodes * sizeof(int)));
 	
	for (i = 0; i < ourGraph.numberOfNodes; i++){
    	for(j = 0; j <ourGraph.numberOfNodes; j++){
        	if (i > j ) ourGraph.Cost[i][j] = -1; //lower triangular
        	else if (i == j ) ourGraph.Cost[i][j] = 0; //Main Diagonal
        	else {
        		if (selection == 1){
					printf("Cost from node %d to node %d (if not applicable, insert -1): \n", i, j);
            		scanf("%d", &(ourGraph.Cost[i][j]));
            	}	
				else
					fscanf(inputfile, "%d", &(ourGraph.Cost[i][j]));
        	}        
    	}
	}
	
	fclose(inputfile);
	
	return ourGraph;
}  

void printCostTable(graph ourGraph){
	
	int i, j;
	
	printf("This is the costs table: \n");
	
	for (i = 0; i < ourGraph.numberOfNodes; i++){
    	for(j = 0; j < ourGraph.numberOfNodes; j++)
    		printf("%5d\t", ourGraph.Cost[i][j]);
    	printf("\n");
	} 	
	
	return;
}

solution solve(graph ourGraph){
	
	int i, j;
	solution ourSolution;
	
	ourSolution.OPT = (int **)malloc(ourGraph.numberOfNodes * sizeof(int *));
 	for (i = 0; i < ourGraph.numberOfNodes; i++)
        assert(ourSolution.OPT[i] = (int *)malloc(ourGraph.numberOfNodes * sizeof(int)));
        
    ourSolution.nextNode = (int *)malloc(ourGraph.numberOfNodes * sizeof(int ));
	
	//Create a copy of costs to work on
    for (i=0;i<ourGraph.numberOfNodes; i++){
    	for (j=0; j<ourGraph.numberOfNodes; j++){
    		ourSolution.OPT[i][j]=ourGraph.Cost[i][j];
		}
	}	
	
	//Dynamic Programming 
    for(i = ourGraph.numberOfNodes-1; i>=0; i--){
        int min = 999999999; //arbitrarily large number
     	for( j = 0; j < ourGraph.numberOfNodes; j++){
     		if (ourGraph.Cost[i][j]>0){ 
                ourSolution.OPT[i][j] = ourGraph.Cost[i][j] + ourSolution.OPT[j][ourGraph.numberOfNodes-1];
                if (ourSolution.OPT[i][j] <= min) {
                    ourSolution.OPT[i][ourGraph.numberOfNodes-1] = ourSolution.OPT[i][j];
                    min = ourSolution.OPT[i][j];
                    ourSolution.nextNode[i] = j;                    
                }
            }
        }
    }	
    
    //If you are at the last node, remain.
    ourSolution.nextNode[ourGraph.numberOfNodes-1] = ourGraph.numberOfNodes-1;
    
    // TEMPORARILY printing shortest path
    printf("Shortest path:\n");
    for (i=0; i<ourGraph.numberOfNodes; i++) printf("From node %d you have to go to %d\n", i, ourSolution.nextNode[i]);
    
    // TEMPORARILY printing OPT Array
	for (i = 0; i < ourGraph.numberOfNodes; i++){
    	for(j = 0; j < ourGraph.numberOfNodes; j++)
    		printf("%5d\t", ourSolution.OPT[i][j]);
    	printf("\n");
	} 	

	return ourSolution;
}

void bestCost(int startNode, solution ourSolution, graph ourGraph){
    printf("The optimal cost from node %d to the end: %d\n", startNode, ourSolution.OPT[startNode][ourGraph.numberOfNodes-1]);
	return;
}

void bestPath(int startNode, solution ourSolution, graph ourGraph){
    printf("The best bath from node %d: ", startNode);
    
	int visitNextNode = ourSolution.nextNode[startNode];
    
    printf("%d ", visitNextNode);
	do{
		visitNextNode =  ourSolution.nextNode[visitNextNode];
        printf("%d ", visitNextNode);
    }while(visitNextNode != ourGraph.numberOfNodes-1);
    
    return;
} 
    
void freeGraph(graph ourGraph, solution ourSolution){
	//Code for freeing the dynamically allocated memory 
    int i;
    
	for (i = 0; i < ourGraph.numberOfNodes; i++) {
		free(ourGraph.Cost[i]); 
		free(ourSolution.OPT[i]);      
    }
    
	free(ourGraph.Cost);
    free(ourSolution.OPT);
    free(ourSolution.nextNode); 
    free(ourGraph.G); 

    return;
}


