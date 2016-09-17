// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "stack.h"
#include "set.h"
#include "graph.h"
#include "html.h"
#include "url_file.h"
#include "queue.h"

#define BUFSIZE 1024
#define FALSE 0
#define TRUE 1

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
    //Breadth First implementation
    //Create queue to hold URLs to visit next
    Queue toDo=newQueue();
    enterQueue(toDo, firstURL);
    //Create graph to hold URLs
    Graph visitedGraph=newGraph(maxURLs);
    //Create set to ensure URLs are not repeated 
    Set seenSet=newSet();
    insertInto(seenSet,firstURL);
    
    while(emptyQueue(toDo)==FALSE && nVertices(visitedGraph)<maxURLs){
        char *toVisit=leaveQueue(toDo);
        
        if (strstr(toVisit, "unsw.edu.au") == NULL) continue; //check if unsw site
        
        if (!(handle = url_fopen((const char *)toVisit, "r"))) {
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
              printf("Found: '%s'\n",result);
              //continue if we have already seen this URL
              if(isElem(seenSet,result)==TRUE)continue;
              //add edge if one doesn't exist
              if(isConnected(visitedGraph,toVisit,result)==FALSE){
                 if (addEdge(visitedGraph,toVisit,result)==FALSE){
                    fprintf(stderr,"Couldn't add edge between %s and %s\n",toVisit,result);
                    continue;
                 }
              }
              //add result to seen set and into the queue
              insertInto(seenSet,result);
              enterQueue(toDo,result);
			  memset(result,0,BUFSIZE);
		   }
		   
	    }
	    
	    url_fclose(handle);
	    sleep(1);
    }
    //showGraph(visitedGraph,FALSE);

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
