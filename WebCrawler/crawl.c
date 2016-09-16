// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include <string.h>
#include "stack.h"
#include "set.h"
#include "graph.h"
#include "html.h"
#include "url_file.h"
#include "queue.h"

#define BUFSIZE 1024
#define TRUE 1
#define FALSE 0

void setFirstURL(char *, char *);
void normalise(char *, char *, char *, char *, int);

int main(int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int  maxURLs;

	if (argc > 2) {
		strcpy(baseURL,argv[1]);
		setFirstURL(baseURL,firstURL);
		maxURLs = atoi(argv[2]);
		if (maxURLs < 40) maxURLs = 40;
	}
	else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n",argv[0]);
		exit(1);
	}
	
	Queue todo = newQueue();
	enterQueue(todo, firstURL);
	
	Graph visitedGraph = newGraph(maxURLs);
	Set seenSet = newSet();
	insertInto(seenSet, firstURL);
	
	while (emptyQueue(todo) == FALSE && nVertices(visitedGraph) < maxURLs){
	    char *toVisit = leaveQueue(todo);
	    printf("toVisit is %s\n", toVisit);

	    if (strstr(toVisit, "unsw.edu.au") == NULL) continue; //check if unsw site	    

	    if (!(handle = url_fopen(toVisit, "r"))) {
		    fprintf(stderr,"Couldn't open %s\n", next);
		    exit(1);
	    }
	    
	    while(!url_feof(handle)) {
		    url_fgets(buffer,sizeof(buffer),handle);
		    //fputs(buffer,stdout);
		    int pos = 0;
		    char result[BUFSIZE];
		    memset(result,0,BUFSIZE);
		    while ((pos = GetNextURL(buffer, toVisit, result, pos)) > 0) {
			    printf("Result: '%s'\n",result);
			    
			    if (nVertices(visitedGraph) < maxURLs || isConnected(visitedGraph, toVisit, result) == FALSE){
			        if (addEdge(visitedGraph, toVisit, result) == FALSE){
			            printf("exit\n");
			            continue;
			        }
			        //printf("to visit: %s\n, result %s\n\n", toVisit, result);
			    }
			    if (isElem(seenSet, result) == FALSE){
			        insertInto(seenSet, result);
			        enterQueue(todo, result);
			        showQueue(todo);
			    } 
			    strcpy(toVisit, result);
			    printf("toVisit is %s\n\n", toVisit);
			    memset(result,0,BUFSIZE);
		    }
	    }
	    //showGraph(visitedGraph, 0);
	    
	    url_fclose(handle);
	    sleep(1);
	}
	// You need to modify the code below to implement:
	//
	// add firstURL to the ToDo list
	// initialise Graph to hold up to maxURLs
	// initialise set of Seen URLs
	// while (ToDo list not empty and Graph not filled) {
	//    grab Next URL from ToDo list
	//    if (not allowed) continue
	//    foreach line in the opened URL {
	//       foreach URL on that line {
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	//       }
    //    }
	//    close the opened URL
	//    sleep(1)
	// }


	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
void setFirstURL(char *base, char *first)
{
	char *c;
	if ((c = strstr(base, "/index.html")) != NULL) {
		strcpy(first,base);
		*c = '\0';
	}
	else if (base[strlen(base)-1] == '/') {
		strcpy(first,base);
		strcat(first,"index.html");
		base[strlen(base)-1] = '\0';
	}
	else {
		strcpy(first,base);
		strcat(first,"/index.html");
	}
}
